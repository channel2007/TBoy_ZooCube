#include <TFT_eSPI.h> #include <SPI.h>

#include "Free_Fonts.h" 
#include "ClockSystem.h"
#include "Joystick.h"
#include "Music.h"
#include "GamePlay.h"

TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite doubleBuffer = TFT_eSprite(&tft);

Joystick joystick_1P(2, 15, 13, 12, 32, 33, 35, 0);
ClockSystem clockSystem;
GamePlay gamePlay = GamePlay();

typedef enum{
  MENU_MAIN,        
  MENU_OPTIONS,     
  GAME_PLAY,        
  GAME_PAUSE,       
  GAME_OPTIONS,     
  GAME_OVER,      
}GameMode;

char buf[16]; 

uint8_t fps=0;
uint8_t fpsTemp=0;

uint8_t obstacle=0;
uint8_t obstacleGameOver=0;

uint8_t sel=0;
bool  releaseBnt=true;

GameMode gameMode = MENU_MAIN;
GameMode gameModeTemp = MENU_MAIN;
  
void musicEffect(void *pvParam){
  while(1){
        if(gamePlay.gameData.MusicSwitch){
      if(gameMode >= MENU_MAIN && gameMode <= MENU_OPTIONS)
        playChannel();
      else if(gameMode >= GAME_PLAY && gameMode <= GAME_OVER)
        playTetris();
    }        
    tone(BUZZER_DUMMY_PIN, 1, 1000);
    noTone(BUZZER_DUMMY_PIN);    
  }
}

void setup() {
  
  tft.begin();                          
  tft.setSwapBytes(true);

  doubleBuffer.setColorDepth(16);  
  doubleBuffer.createSprite(135, 240);    

  clockSystem.initClock();
  
  gamePlay.copyToGamingZone( gamePlay.cubeX, gamePlay.cubeY, gamePlay.cubeId, gamePlay.cubeState, true);
  
  EEPROM_Init(22);  
  

  gamePlay.GameLoad();

        
  gamePlay.cubeLineMax = gamePlay.gameData.RankingLineMax[0];

  if(gamePlay.gameData.ScreenRotation == 0)
    tft.setRotation(0);
  else
    tft.setRotation(2);

  xTaskCreate(musicEffect, "musicEffect", 1000, NULL, 1, NULL);    
}

void loop() {
    uint8_t cubeID = gamePlay.nextCubeId;  
  uint8_t x=0,y=0,w=0,h=0;
  float cubeMax,cubeNow;

    if (clockSystem.checkClock(1, 1000)) {
    fps=fpsTemp;
    fpsTemp=0;
  }

    if (clockSystem.checkClock(0, 33)) {
    fpsTemp++;
    
    if(gameMode == MENU_MAIN){      
      if (joystick_1P.GetButtonUp(true)) {
        if(sel>=1)
          sel--;
                
      }else if (joystick_1P.GetButtonDown(true)) {
        if(sel<1)
          sel++;
      }
      
      if (joystick_1P.GetButtonA(true) || joystick_1P.GetButtonStart(true)) {
        if(sel==0){
          
          gamePlay.gameReset();          
          
          playMusic = false;
          gameMode = GAME_PLAY;
        }else if(sel==1){
          gameModeTemp = gameMode;
          gameMode = MENU_OPTIONS;
        }
        sel=0;  
      }
    
    }else if(gameMode == MENU_OPTIONS || gameMode == GAME_OPTIONS){
      if (joystick_1P.GetButtonUp(true)) {
        if(sel>=1)
          sel--;
      
      }else if (joystick_1P.GetButtonDown(true)) {
        if(sel<3)
          sel++;
      }      
      
      if (joystick_1P.GetButtonA(true) || joystick_1P.GetButtonStart(true)) {
        
        if(sel==0){          
          gamePlay.gameData.MusicSwitch = !gamePlay.gameData.MusicSwitch;
          
          if(!gamePlay.gameData.MusicSwitch)
            playMusic = false;                      
          if(EEPROM_ENABLE)
            EEPROM_writeByte(gamePlay.gameData.MusicSwitch, 2);                    
        }else if(sel==1){
          gamePlay.gameData.SoundSwitch = !gamePlay.gameData.SoundSwitch;                              
          if(EEPROM_ENABLE)
            EEPROM_writeByte(gamePlay.gameData.SoundSwitch, 3);
          
        
        }else if(sel==2){
          if(gamePlay.gameData.ScreenRotation==0){
            gamePlay.gameData.ScreenRotation = 2;
            tft.setRotation(2);              
          }else{
            gamePlay.gameData.ScreenRotation = 0;
            tft.setRotation(0);
          }
          if(EEPROM_ENABLE)
            EEPROM_writeByte(gamePlay.gameData.ScreenRotation, 1);
        }else if(sel==3){
          gamePlay.clearGamingZone(0);     
          gamePlay.update();
          playMusic = false;          
          obstacle=0;
          obstacleGameOver=0;          
          gameMode = MENU_MAIN;          
          sel=0;       
        }        
      }
      
      if (joystick_1P.GetButtonB(true) || joystick_1P.GetButtonSelect(true)) {
        gameMode = gameModeTemp;
        sel=0;
      }    

    
    }else if(gameMode == GAME_PLAY){
      if(gamePlay.specialSec==0){
        if (joystick_1P.GetButtonUp() && releaseBnt==0) {
        }
        if (joystick_1P.GetButtonDown()) {
          
          
          if (!gamePlay.cubeClear && clockSystem.checkClock(2,  0))
            gamePlay.moveCube(1);          
        }
        if (joystick_1P.GetButtonLeft()) {
          
          if (clockSystem.checkClock(2, 100))
            gamePlay.moveCube(2);    
        }
        if (joystick_1P.GetButtonRight()) {
          
          if (clockSystem.checkClock(2, 100))
            gamePlay.moveCube(3);    
        }
        if (joystick_1P.GetButtonStart(true)) {
          gameMode = GAME_PAUSE;
        }
        if (joystick_1P.GetButtonSelect(true)) {
          gameModeTemp = gameMode;
          gameMode = GAME_OPTIONS;
        }
    
        if (joystick_1P.GetButtonA(true)) {
          if(!gamePlay.cubeClear){
            gamePlay.cubeChange(true);
            gamePlay.copyToGamingZone( gamePlay.cubeX, gamePlay.cubeY, gamePlay.cubeId, gamePlay.cubeState, true);          
          }        
        }
        if (joystick_1P.GetButtonB(true)) {
          if(!gamePlay.cubeClear){
            gamePlay.cubeChange(false);
            gamePlay.copyToGamingZone( gamePlay.cubeX, gamePlay.cubeY, gamePlay.cubeId, gamePlay.cubeState, true);          
          }        
        }
        
      
      }else{
        
        if (joystick_1P.GetButtonLeft()) {
          
          if(gamePlay.specialID==0){
            if (clockSystem.checkClock(2, 100))
              gamePlay.specialMoveL();
          
          }else if(gamePlay.specialID==1){   
            if (clockSystem.checkClock(2, 200))
              gamePlay.specialMoveL();            
          }
        }
        
        if (joystick_1P.GetButtonRight()) {
          
          if(gamePlay.specialID==0){
            if (clockSystem.checkClock(2, 100))
              gamePlay.specialMoveR();
          
          }else if(gamePlay.specialID==1){
            if (clockSystem.checkClock(2, 200))
              gamePlay.specialMoveR();                 
          }          
        }
        
        if (joystick_1P.GetButtonA(true)) {
          gamePlay.specialFireCube();
        }
        if (joystick_1P.GetButtonB(true)) {
          gamePlay.specialFireCube();
        }
      }
  
      if (clockSystem.checkClock(3, gamePlay.cubeDownSpeed) && gamePlay.specialSec==0){
        if(!gamePlay.cubeClear){
          if(gamePlay.moveCube(1)){          
            if(gamePlay.cubeY==16)
              obstacle += 1;              
          
          }else{
            
            obstacle += 1;
          }
          
          
          if(obstacle>=2){
            
            if(gamePlay.cubeY==-3)
              obstacleGameOver +=1;              
            
            if(obstacleGameOver>=2){
              gamePlay.gameOver();
              gameMode = GAME_OVER;
              
            
            }else{
              gamePlay.cubeStop();
              obstacle = 0;                          
            }            
          }
        }
      }      
      gamePlay.update();
      if (gamePlay.specialID==0 && gamePlay.specialSec>0) {
        if(clockSystem.checkClock(5, 500)){
          
          gamePlay.specialSec--;          
          
          if(gamePlay.specialMoveD()){
            gamePlay.specialSec = 0;
          }
        }
      }else if (gamePlay.specialID==1 && gamePlay.specialSec>0) {
          if(clockSystem.checkClock(5, 1000)){
            
            gamePlay.specialSec--;
            
            if(gamePlay.specialSec==1){
              gamePlay.specialMalletDrop = true;
            }
          }
      }      
      
      if (clockSystem.checkClock(4,  10))
        gamePlay.updateScore();
    }else if(gameMode == GAME_PAUSE){
      if (joystick_1P.GetButtonStart(true)) {
        clockSystem.initClock();
        gameMode = GAME_PLAY;
      }

    }else if(gameMode == GAME_OPTIONS){
      if (joystick_1P.GetButtonSelect(true)) {
        clockSystem.initClock();        
        gameMode = GAME_PLAY;
      }
      
    }else if(gameMode == GAME_OVER){
      if (joystick_1P.GetButtonStart(true)) {
        obstacle=0;
        obstacleGameOver=0;
        
        gamePlay.gameReset();
        clockSystem.initClock();        
        gameMode = GAME_PLAY;
      }    
      if (joystick_1P.GetButtonSelect(true)) {
        gameModeTemp = gameMode;
        gameMode = GAME_OPTIONS;
      }      
    }

    doubleBuffer.pushImage(0, 0, 135, 240, gamePlay.getBackground());

    if(gameMode != GAME_PAUSE && gameMode != GAME_OPTIONS){
      for(uint8_t i=0; i< CUBE_MAX;i++){      
        if(gamePlay.cubeData[i].img != NULL)
          doubleBuffer.pushImage( gamePlay.cubeData[i].x,  gamePlay.cubeData[i].y, gamePlay.cubeData[i].w, gamePlay.cubeData[i].h, gamePlay.cubeData[i].img);    
      }
    }
    
    for(uint8_t i=0; i< CUBE_BOOM_MAX; i++){
      if(gamePlay.cubeBoomData[i].use){
        gamePlay.updateCubeBoom(i);                
        doubleBuffer.pushImage( gamePlay.cubeBoomData[i].x,  gamePlay.cubeBoomData[i].y, 13, 13, gamePlay.cubeBoomData[i].img);        
      }
    }

    for(uint8_t i=0; i<7; i++){
      cubeMax = (float)gamePlay.cubeMax[i]/12.0f;
      cubeNow = (float)gamePlay.cubeNow[i]/cubeMax;
      x=121;y=135+(i*15);w=cubeNow;h=10;
      
      doubleBuffer.fillRect (   x,  y,  w,  h, 0xF800);    
      w=12;
      doubleBuffer.drawLine (   x,  y,x+w,  y, 0xFFFF);
      doubleBuffer.drawLine (   x,y+h,x+w,y+h, 0xFFFF);    
      doubleBuffer.drawLine (   x,  y,  x,y+h, 0xFFFF);
      doubleBuffer.drawLine ( x+w,  y,x+w,y+h, 0xFFFF);           
    }
    doubleBuffer.setFreeFont(FSB9);
    doubleBuffer.setTextColor(TFT_WHITE);
    sprintf(buf, "%05d", gamePlay.cubeScore);
    doubleBuffer.drawCentreString(buf, 30, 20, GFXFF);
    sprintf(buf, "%03d", gamePlay.cubeLineNow);
    doubleBuffer.drawCentreString(buf, 80, 20, GFXFF);    
    sprintf(buf, "%03d", gamePlay.cubeLineMax);
    doubleBuffer.drawCentreString(buf,117, 20, GFXFF);
    
    sprintf(buf, "%01d", gamePlay.cubeSpeed);
    doubleBuffer.drawCentreString(buf, 119,  86, GFXFF);
    sprintf(buf, "%01d", gamePlay.cubeStage);
    doubleBuffer.drawCentreString(buf, 119, 115, GFXFF);
    
    if(gameMode == MENU_MAIN){      
      doubleBuffer.setFreeFont(FSB9);
      doubleBuffer.setTextColor(TFT_WHITE);
      doubleBuffer.drawCentreString(  "START", 50,116, GFXFF);
      doubleBuffer.drawCentreString("OPTIONS", 50,144, GFXFF);
      
      doubleBuffer.setTextColor(TFT_RED);
      if(sel==0){
        doubleBuffer.drawCentreString(  "START", 50,116, GFXFF);
      }else if(sel==1){
        doubleBuffer.drawCentreString("OPTIONS", 50,144, GFXFF);
      }      

    }else if(gameMode == GAME_PLAY){
      if(gamePlay.specialSec >0 && gamePlay.specialID==1){        
        doubleBuffer.pushImage( gamePlay.specialMallet.x, gamePlay.specialMallet.y, gamePlay.specialMallet.w, gamePlay.specialMallet.h, gamePlay.specialMallet.img);
      }      
      
    }else if(gameMode == GAME_PAUSE){
      doubleBuffer.setFreeFont(FSB9);
      doubleBuffer.setTextColor(TFT_WHITE);
      doubleBuffer.drawCentreString("PAUSE", 55, 45, GFXFF);

      for(uint8_t i=0; i<7; i++){
        cubeMax = (float)gamePlay.cubeMax[i]/60.0f;
        cubeNow = (float)gamePlay.cubeNow[i]/cubeMax;      
        x=30;y=68+(i*24);w=cubeNow;h=18;
        
        doubleBuffer.fillRect (   x,  y,  w,  h, 0xF800);    
        w=65;
        doubleBuffer.drawLine (   x,  y,x+w,  y, 0xFFFF);
        doubleBuffer.drawLine (   x,y+h,x+w,y+h, 0xFFFF);    
        doubleBuffer.drawLine (   x,  y,  x,y+h, 0xFFFF);
        doubleBuffer.drawLine ( x+w,  y,x+w,y+h, 0xFFFF);           

        doubleBuffer.setFreeFont(FSB9);
        sprintf(buf, "%d/%d", gamePlay.cubeNow[i], gamePlay.cubeMax[i]);
        doubleBuffer.drawCentreString(buf, x+32, y+2, GFXFF);
        if(i==0)          
          doubleBuffer.pushImage( 6,  y, 18, 18, cube18x18_1);
        else if(i==1)
          doubleBuffer.pushImage( 6,  y, 18, 18, cube18x18_2);
        else if(i==2)
          doubleBuffer.pushImage( 6,  y, 18, 18, cube18x18_3);        
        else if(i==3)
          doubleBuffer.pushImage( 6,  y, 18, 18, cube18x18_4);
        else if(i==4)
          doubleBuffer.pushImage( 6,  y, 18, 18, cube18x18_5);
        else if(i==5)
          doubleBuffer.pushImage( 6,  y, 18, 18, cube18x18_6);
        else if(i==6)
          doubleBuffer.pushImage( 6,  y, 18, 18, cube18x18_7);
      }     

    }else if(gameMode == GAME_OVER){
      doubleBuffer.setFreeFont(FSB9);
      doubleBuffer.setTextColor(TFT_WHITE);
      doubleBuffer.drawCentreString("GAME", 50, 96, GFXFF);
      doubleBuffer.drawCentreString("OVER", 50,124, GFXFF);      

    }else if(gameMode == GAME_OPTIONS || gameMode == MENU_OPTIONS){
      
      doubleBuffer.setFreeFont(FSB9);
      doubleBuffer.setTextColor(TFT_WHITE);
      doubleBuffer.drawCentreString("OPTIONS", 52, 45, GFXFF);

      doubleBuffer.setFreeFont(FM9);
      doubleBuffer.setTextColor(0xC658);      
      
      if(gamePlay.gameData.MusicSwitch)
        doubleBuffer.drawCentreString(  "MUSIC ON",  50,66, GFXFF);
      else 
        doubleBuffer.drawCentreString(  "MUSIC OFF", 50,66, GFXFF);
      
      if(gamePlay.gameData.SoundSwitch)
        doubleBuffer.drawCentreString( "SOUND ON",   50,84, GFXFF);
      else
        doubleBuffer.drawCentreString( "SOUND OFF",  50,84, GFXFF);
      
      if(gamePlay.gameData.ScreenRotation==0)        
        doubleBuffer.drawCentreString("ROTATION0",  50,102, GFXFF);
      else
        doubleBuffer.drawCentreString("ROTATION2",  50,102, GFXFF);
      
      doubleBuffer.drawCentreString("MAIN MENU",  50,120, GFXFF);
      
      doubleBuffer.setTextColor(TFT_RED);      
      if(sel==0){
        
        if(gamePlay.gameData.MusicSwitch)
          doubleBuffer.drawCentreString(  "MUSIC ON", 50,66, GFXFF);
        else 
          doubleBuffer.drawCentreString(  "MUSIC OFF",50,66, GFXFF);        
      }else if(sel==1){
        
        if(gamePlay.gameData.SoundSwitch)
          doubleBuffer.drawCentreString( "SOUND ON",  50,84, GFXFF);
        else
          doubleBuffer.drawCentreString( "SOUND OFF", 50,84, GFXFF);
      }else if(sel==2){
        
        if(gamePlay.gameData.ScreenRotation==0)        
          doubleBuffer.drawCentreString("ROTATION0", 50,102, GFXFF);
        else
          doubleBuffer.drawCentreString("ROTATION2", 50,102, GFXFF);
      }else if(sel==3){
        doubleBuffer.drawCentreString("MAIN MENU",  50,120, GFXFF);        
      }

      
      doubleBuffer.setFreeFont(FSB9);
      doubleBuffer.setTextColor(TFT_WHITE);
      doubleBuffer.drawCentreString("RANKING", 50, 150, GFXFF);

      doubleBuffer.setFreeFont(FM9);
      doubleBuffer.setTextColor(0xC658);      
      sprintf(buf, "1 %05d", gamePlay.gameData.RankingScore[0]);
      doubleBuffer.drawCentreString( buf, 50, 170, GFXFF);
      sprintf(buf, "2 %05d", gamePlay.gameData.RankingScore[1]);
      doubleBuffer.drawCentreString( buf, 50, 188, GFXFF);
      sprintf(buf, "3 %05d", gamePlay.gameData.RankingScore[2]);
      doubleBuffer.drawCentreString( buf, 50, 206, GFXFF);      
    }
    
    /*
    // Debugs:顯示區除錯.
    doubleBuffer.setFreeFont(FF1);
    doubleBuffer.setTextColor(TFT_WHITE);    
    for(uint8_t y=0; y<20; y++){
      for(uint8_t x=0; x<10; x++){  
        if(gamePlay.gamingZone[x][y]!=0){
          sprintf(buf, "%d", gamePlay.gamingZone[x][y]);
          doubleBuffer.drawCentreString(buf, 6+(x*10),  34+(y*10), GFXFF);
        }
      }
    }    
    */

    for(uint8_t i=0; i< CUBE_BASIC_MAX; i++){
      
      if (clockSystem.checkClock(6, 1000)){        
        for(uint8_t j=0; j< CUBE_BASIC_MAX; j++){          
          gamePlay.cubeFlash[j].w++;
          if(gamePlay.cubeFlash[j].w>1)
            gamePlay.cubeFlash[j].w = 0;            
        }          
      }
      
      if(gamePlay.cubeNow[i] >= (gamePlay.cubeMax[i]-3)){        
        if(gamePlay.cubeFlash[i].w == 1)
          doubleBuffer.pushImage( gamePlay.cubeFlash[i].x, gamePlay.cubeFlash[i].y, 13, 13, gamePlay.cubeFlash[i].img);
      }
    }
    
    if(gamePlay.specialID==1 && gamePlay.specialSec>0){
      sprintf(buf, "%01d", gamePlay.specialSec-1);
      doubleBuffer.drawCentreString(buf, 119,  54, GFXFF);      
    }else{    
      doubleBuffer.pushImage( gamePlay.cubeNextData[cubeID].x, gamePlay.cubeNextData[cubeID].y, gamePlay.cubeNextData[cubeID].w,  gamePlay.cubeNextData[cubeID].h, gamePlay.cubeNextData[cubeID].img);
    }
          
    /*
    // FPS.
    sprintf(buf, "FPS:%d", fps);
    doubleBuffer.setFreeFont(FSB9);
    doubleBuffer.setTextColor(TFT_WHITE);    
    doubleBuffer.drawCentreString(buf, 0, 0, GFXFF);    
    */

    // 將備頁(精靈)複製到顯示區.
    // 24 bit 0x00FF00            
    doubleBuffer.pushSprite(0, 0, 0x07E0);    
  }

}
