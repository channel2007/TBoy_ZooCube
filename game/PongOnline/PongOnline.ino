//========================================================
// 功能：Pong連線遊戲 - 主程式.
//
//========================================================
#include <TFT_eSPI.h>
#include <SPI.h>

#include "Free_Fonts.h" 
#include "esp32_wifi_tcp.h"
#include "Joystick.h"
// 0:FPS.
// 1:計算FPS.
// 2:閃爍字.
#include "ClockSystem.h"

#define PADDLE_WIDTH 32           // 板子寬長.
#define PADDLE_HEIGHT 10

#define BALL_WIDTH 4              // 球寬長.
#define BALL_HEIGHT 4

#define WIN_SCORE 10              // 贏幾局結束遊戲.

// 遊戲模式.
typedef enum{
  GAME_SELECT=0,    // 選1P、2P.  
  GAME_CONNECTING,  // 等待連線.
  GAME_BEGIN,       // 準備開始遊戲.
  GAME_PLAY,        // 開始遊戲.
  GAME_OVER         // 遊戲結束.
} GameMode;

// box結構.
typedef struct {
    int x;
    int y;
    int w;
    int h;    
} Box;

// 球結構.
typedef struct {
    double x;
    double y;
    double prevX;
    double prevY;
    int w;
    int h;        
} Ball;

// 建立wifi物件.
esp32_wifi_tcp tcp( true, "ESP32-AP", "12345678");
// SPI.
TFT_eSPI tft = TFT_eSPI(); 
// 備頁.
TFT_eSprite doubleBuffer = TFT_eSprite(&tft);

// 搖桿1P.
Joystick joystick_1P(2, 15, 13, 12, 32, 33, 35, 0);
// 時脈系統.
ClockSystem clockSystem;

// 板子(1P、2P).
Box board1P;
Box board2P;
// 球.
Ball ball;

// fps.
uint8_t fps=0;
uint8_t fpsTemp=0;

// 是否為Server模式(1P).
bool isServer = true;
// 閃爍字.
bool flashFont = true;

// 分數.
uint8_t score1P = 0;
uint8_t score2P = 0;
// 球-移動速度.
double speed = 2.0;
// 球-設定初始反射角度.
double reflection_angle = 45.0;  

// 行進時間(時間越長球速越快).
int travelTime = 0;
// 無敵.
bool invincible= false;
// 自動玩遊戲.
bool autoPlay = false;
// 板子移動速度.
int boardSpeed = 4;

// 字串.
char buf[64]; 

// 設定初始狀態.
GameMode gameMode = GAME_SELECT;

//-----------------------------------------------------
// 拆解字串.
//-----------------------------------------------------
void split(const char *data, char separator, char result[][100], int *resultCount, int maxResultCount) {
  int len = strlen(data);
  int start = 0;
  int end = 0;
  *resultCount = 0;
  for (int i = 0; i < len; i++) {
    if (data[i] == separator) {
      end = i;
      int length = end - start;
      if (length > 0) {
        strncpy(result[*resultCount], &data[start], length);
        result[*resultCount][length] = '\0';
        (*resultCount)++;
        if (*resultCount >= maxResultCount) {
          break;
        }
      }
      start = i + 1;
    }
  }  
  if (*resultCount < maxResultCount) {
    int length = len - start;
    if (length > 0) {
      strncpy(result[*resultCount], &data[start], length);
      result[*resultCount][length] = '\0';
      (*resultCount)++;
    }
  }
}

//-----------------------------------------------------
// 判斷球與板子碰撞.
//-----------------------------------------------------
bool didCollide(Box p, Ball b){
  // 首先檢查球是否在板子的左邊或右邊之外，如果是，則不可能發生碰撞
  if ((b.x + BALL_WIDTH / 2.0) < p.x || (b.x - BALL_WIDTH / 2.0) > (p.x + PADDLE_WIDTH)) {
    return false;
  }
  // 檢查球是否在板子的上邊或下邊之外，如果是，則不可能發生碰撞
  if ((b.y + BALL_HEIGHT / 2.0) < p.y || (b.y - BALL_HEIGHT / 2.0) > (p.y + PADDLE_HEIGHT)) {
    return false;
  }
  // 如果以上條件都不成立，則表示球與板子有重疊，即發生了碰撞
  return true;
}

//-----------------------------------------------------
// 更新球移動.
//-----------------------------------------------------
void updateBall(Ball* position, double* speed, double* reflection_angle) {
    // 遊戲中才進入處理球移動.
    if(gameMode != GAME_PLAY)
      return;
    
    // 計算球在x軸和y軸上的速度成分
    double dx = *speed * cos(*reflection_angle);
    double dy = *speed * sin(*reflection_angle);

    // 備份球舊座標(判斷碰撞用).
    position->prevX = position->x; 
    position->prevY = position->y;

    // 更新球的位置
    position->x += dx;
    position->y += dy;

    // 檢查是否碰到邊界，並反彈
    if (position->x < 4  || position->x > 127) {
      // 球碰到左右邊界，將反射角度反轉
      *reflection_angle = M_PI - *reflection_angle;
    }
    if (position->y < 0 || position->y > 240) {
      // 球碰到上下邊界，將反射角度反轉
      *reflection_angle = -(*reflection_angle);
      // 不是無敵才進入執行.
      if(!invincible){
        // 1P加分.
        if(position->y < 0){
          score1P++;        
        // 2P加分.
        }else if(position->y > 240){
          score2P++;
        }  
        // 初始球位置.
        position->x = position->prevX = 70; 
        position->y = position->prevY = 120;
        
        // 放慢球速.
        *speed = 2.0;
        //Serial.println((String)position->x+":"+position->y);      
      }      
    }
}

//-----------------------------------------------------
// 更新畫面.
//-----------------------------------------------------
void updateScreen(){
  // 牆壁-左.
  doubleBuffer.fillRect (  0, 0,  4,  240, 0xFFFF);    
  // 牆壁-右.
  doubleBuffer.fillRect (131, 0,  4,  240, 0xFFFF);
  // 牆壁-中.      
  for(int i=4; i<131; i+=8){
    doubleBuffer.fillRect (i, 118, 4,  4, 0xFFFF);
  }
  // 顯示分數.
  doubleBuffer.setFreeFont(FSB18);
  doubleBuffer.setTextColor(TFT_WHITE, TFT_BLACK);      
  sprintf(buf, "%d", score2P);
  doubleBuffer.drawCentreString(buf, 105,  80, GFXFF); // 2P.
  sprintf(buf, "%d", score1P);
  doubleBuffer.drawCentreString(buf, 105, 135, GFXFF); // 1P.  

  // 板子1P. 
  doubleBuffer.fillRect ( board1P.x, board1P.y, board1P.w,  board1P.h, 0xFFFF);
  // 板子2P. 
  doubleBuffer.fillRect ( board2P.x, board2P.y, board2P.w,  board2P.h, 0xFFFF);   
}

//-----------------------------------------------------
// 顯示連線訊息.
//-----------------------------------------------------
void connecting(){
  // 用全黑清除螢幕
  doubleBuffer.fillScreen(TFT_BLACK);
  doubleBuffer.setFreeFont(FSB9);
  doubleBuffer.setTextColor(TFT_WHITE, TFT_RED);
  doubleBuffer.drawCentreString("connecting...", 66, 40, GFXFF);
  // 更新畫面.
  updateScreen();  
}

//-----------------------------------------------------
// 板子超界修正.
//-----------------------------------------------------
void boardOverstep(){  
  if(board1P.x<4) 
    board1P.x = 4;          
  if(board2P.x<4) 
    board2P.x = 4;        
  if(board1P.x>99) 
    board1P.x = 99;          
  if(board2P.x>99) 
    board2P.x = 99;
}

//-----------------------------------------------------
// 移動板子.
//-----------------------------------------------------
void boardMove(){
  // 左.
  if (joystick_1P.GetButtonLeft(false)) {
    // 1P.
    if(isServer){
      board1P.x -= boardSpeed;
    // 2P.
    }else{
      board2P.x -= boardSpeed;
    }
    
  // 右.
  }else if (joystick_1P.GetButtonRight(false)) {
    // 1P.
    if(isServer){
      board1P.x += boardSpeed;
    // 2P.
    }else{
      board2P.x += boardSpeed;
    }        

  // 開關無敵.
  }else if (joystick_1P.GetButtonStart(true)) {
    invincible = !invincible;
    
  // 開關自動玩.
  }else if (joystick_1P.GetButtonSelect(true)) {
    autoPlay = !autoPlay;
        
  }

  // 快速移動板子.
  if (joystick_1P.GetButtonA(false)){
    boardSpeed = 12;
  }else{
    boardSpeed = 4;
  }
  
  // 板子超界修正.
  boardOverstep();  
}

//-----------------------------------------------------
// 1P、2P相互溝通.
//-----------------------------------------------------
void communicate(){
  // Server模式(1P).
  if(isServer){
    // 【Server傳送指令給Client】1P板子座標 - c1|x|bx|by|score1P|score2P|gameMode|
    sprintf(buf, "c1|%d|%f|%f|%d|%d|%d|", board1P.x, ball.x, ball.y, score1P, score2P, gameMode);
    tcp.send(buf);
          
    // 更新球.
    updateBall(&ball, &speed, &reflection_angle);
      
    // 如果球與板子發生碰撞就處理反彈.       
    if( didCollide(board1P, ball) || didCollide(board2P, ball)){
      // 反彈球.
      reflection_angle = -(reflection_angle);
      // 復原座標.
      ball.x = ball.prevX; ball.y = ball.prevY;
      // 開球時放慢速度，等碰到板子後恢復正常速度.
      if(speed == 2.0) speed = 4.0;
    }    
    //Serial.println(str);

    // 自動玩.
    if(autoPlay){
      board1P.x = ball.x -(PADDLE_WIDTH - BALL_WIDTH)/2;
    }
           
  // Client模式(2P).
  }else{
    //【Client傳送指令給Server】2P板子座標 - s1|x|
    sprintf(buf, "s1|%d|", board2P.x);
    tcp.send(buf);

    // 自動玩.
    if(autoPlay){
      board2P.x = ball.x-(PADDLE_WIDTH - BALL_WIDTH)/2;
    }    
  }
  // 板子超界修正.
  boardOverstep();  
}

//-----------------------------------------------------
// 重新開始遊戲.
//-----------------------------------------------------
void resetGame(){
  // 球.
  ball.x = ball.prevX = 70; ball.y = ball.prevY = 120;
  // 分數.
  score1P = 0;
  score2P = 0;
  // 球-移動速度.
  speed = 2.0;
  // 行進時間(時間越長球速越快).
  travelTime = 0;
  // 板子移動速度.
  boardSpeed = 4;  
}

//-----------------------------------------------------
// 解指令字串.
//-----------------------------------------------------
void decodingReceived(String received){
  char result[10][100];   // 用來存放分割後的字串，最多容納 10 個字串，每個字串長度最多 100 個字元.
  int resultCount = 0;    // 儲存分割後的字串數量.
  int n = 0;
  double d = 0.0;
  
  if(received=="")
    return;
    
  // 需要多一個字元來容納結尾的 null 字元
  char charArray[received.length() + 1];  

  // 將 String 轉換為 char 字元陣列
  received.toCharArray(charArray, sizeof(charArray));
  // 解指令字串.
  split( charArray, '|', result, &resultCount, 10);

  //【Client(2p)傳送指令給Server(1p)】板子座標.
  // 指令格式：s1|x|
  if(String(result[0])=="s1"){
    n = String(result[1]).toInt();
    board2P.x = n;
    
  //【Server(1p)傳送指令給Client(2p)】板子座標、球資料.
  // 指令格式：c1|x|bx|by|score1P|score2P|gameMode|
  }else if(String(result[0])=="c1"){
    // x.
    n = String(result[1]).toInt();
    board1P.x = n;
    // bx.
    d = String(result[2]).toDouble();
    ball.x = d;    
    // by.
    d = String(result[3]).toDouble();
    ball.y = d;        
    // score1P.
    n = String(result[4]).toInt();
    score1P = n;
    // score2P.
    n = String(result[5]).toInt();
    score2P = n;
    // gameMode.
    n = String(result[6]).toInt();
    gameMode = (GameMode)n;   
  }  
}

//-----------------------------------------------------
// 初始.
//-----------------------------------------------------
void setup() {
  Serial.begin(115200);
  
  // 初始化LCD
  tft.begin();                  
  tft.setSwapBytes(true);

  // 螢幕方向(0:正 2:反).
  tft.setRotation(0);
  
  // 初始時脈系統.
  clockSystem.initClock();
 
  // 建立備頁(精靈).
  doubleBuffer.setColorDepth(16);
  doubleBuffer.createSprite(135, 240);      
    
  // 板子1P.
  board1P.x = 54; board1P.y =  16; board1P.w = PADDLE_WIDTH;board1P.h = PADDLE_HEIGHT;
  // 板子2P.
  board2P.x = 54; board2P.y = 216; board2P.w = PADDLE_WIDTH;board2P.h = PADDLE_HEIGHT;
  // 球.  
  ball.x = ball.prevX = 70; ball.y = ball.prevY = 120;
  ball.w = BALL_WIDTH; ball.h = BALL_HEIGHT;
}

//-----------------------------------------------------
// 主迴圈.
//-----------------------------------------------------
void loop() {  
  // 計算FPS.
  if (clockSystem.checkClock(1, 1000)) {
    fps=fpsTemp;
    fpsTemp=0; 
    travelTime++;   
  }

  // FPS 60.
  if (clockSystem.checkClock(0, 17)) {
    fpsTemp++;
        
    // 解指令字串.
    decodingReceived(tcp.receive());    
    // 清除螢幕.
    doubleBuffer.fillScreen(TFT_BLACK);
    
    // 選1P、2P.
    if(gameMode == GAME_SELECT){
      //--------------------------------------------
      // 顯示.
      //--------------------------------------------    
      // 更新畫面.
      updateScreen();
      
      // 顯示字.
      doubleBuffer.setFreeFont(FSSBO24);
      doubleBuffer.setTextColor(TFT_WHITE, TFT_BLACK);
      doubleBuffer.drawCentreString("1P", 50,  35, GFXFF);
      doubleBuffer.drawCentreString("2P", 50, 160, GFXFF);
    
      // 顯示選到的模式(1P Server ， 2P Client).
      doubleBuffer.setTextColor(TFT_WHITE, TFT_RED);
      if(isServer)
        doubleBuffer.drawCentreString("1P", 50,  35, GFXFF);
      else
        doubleBuffer.drawCentreString("2P", 50, 160, GFXFF);
    
      //--------------------------------------------
      // 輸入.
      //--------------------------------------------
      // 上.
      if (joystick_1P.GetButtonUp(true)) {
        isServer = true;
      // 下.
      }else if (joystick_1P.GetButtonDown(true)) {
        isServer = false;
      }
      // A.
      if (joystick_1P.GetButtonA(true)||joystick_1P.GetButtonB(true)){

        // 連線訊息.
        connecting();
        // 更新備頁.
        doubleBuffer.pushSprite(0, 0, 0x07E0);

        // 進入等待連線.
        gameMode = GAME_CONNECTING;
        
        // 設定建立Server或Client.
        tcp.isServer = isServer;
        tcp.begin();
      }    

    // 等待連線.
    }else if(gameMode == GAME_CONNECTING){    
      // 連線訊息.
      connecting();     
      
      // 設定進入準備開始遊戲.
      if(tcp.isConnect()){
        // tcp.send("conn");
        gameMode = GAME_BEGIN;
      }
            
    // 準備開始遊戲.
    }else if(gameMode == GAME_BEGIN){
      // 更新畫面.
      updateScreen();  
      
      // 顯示訊息.
      doubleBuffer.setFreeFont(FSB9);
      if(flashFont)
        doubleBuffer.setTextColor(TFT_WHITE);
      else
        doubleBuffer.setTextColor(TFT_BLACK);

      // 1P.
      if(isServer){
        // A.
        if (joystick_1P.GetButtonA(true)){
          // 球.  
          ball.x = 70; ball.y = 120;
          // 開始遊戲.
          gameMode = GAME_PLAY;
        }
        doubleBuffer.drawCentreString("press A start", 66, 40, GFXFF);        
      // 2P.
      }else{
        doubleBuffer.drawCentreString("waiting...", 66, 40, GFXFF);
      }
                   
      // 閃爍字.
      if (clockSystem.checkClock(2, 1000))
        flashFont =!flashFont;

      // 判斷輸入移動板子.
      boardMove();
      
      // 1P、2P相互溝通.
      communicate();
                  
    // 開始遊戲.
    }else if(gameMode == GAME_PLAY){    
      // 更新畫面.
      updateScreen();
      // 判斷輸入移動板子.
      boardMove();
      // 1P、2P相互溝通.
      communicate();

      // 顯示球. 
      doubleBuffer.fillRect ( ball.x, ball.y, ball.w,  ball.h, 0xFFFF);

      // 遊戲結束.
      if(score1P >= WIN_SCORE || score2P >= WIN_SCORE)
        gameMode = GAME_OVER;

      // 增加球移動速度.
      if((travelTime%10)==0){
        travelTime++;
        speed++;        
        if(speed>8)
          speed = 8;
        //Serial.println(speed);
      }
        
    // 遊戲結束.
    }else if(gameMode == GAME_OVER){
      // 更新畫面.
      updateScreen();      
      // 1P.
      if(isServer){
        // A.
        if (joystick_1P.GetButtonA(true)){
          // 重新開始遊戲.  
          resetGame();          
          // 開始遊戲.
          gameMode = GAME_BEGIN;
        }        
      }
      // 判斷輸入移動板子.
      boardMove();
      // 1P、2P相互溝通.
      communicate();

      // 顯示字.
      doubleBuffer.setFreeFont(FSSO12);
      doubleBuffer.setTextColor(TFT_WHITE, TFT_RED);
      if(score1P >= WIN_SCORE){
        doubleBuffer.drawCentreString("LOSE", 50,  45, GFXFF);
        doubleBuffer.drawCentreString("WIN", 50, 170, GFXFF);
      }else{
        doubleBuffer.drawCentreString("WIN", 50,  45, GFXFF);
        doubleBuffer.drawCentreString("LOSE", 50, 170, GFXFF);        
      }
    }
    
    /*
    // FPS.
    sprintf(buf, "FPS:%d", fps);
    doubleBuffer.setFreeFont(FSB9);
    doubleBuffer.setTextColor(TFT_WHITE);    
    doubleBuffer.drawCentreString(buf, 40, 100, GFXFF);    
    */    
    // 將備頁(精靈)複製到顯示區.
    doubleBuffer.pushSprite(0, 0, 0x07E0);
  }     

}
