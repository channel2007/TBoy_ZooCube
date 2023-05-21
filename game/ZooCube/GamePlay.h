#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "EepromSave.h"
#include "imgCube.c"

#include "imgBackground1.c"
#include "imgBackground2.c"
#include "imgBackground3.c"
#include "imgBackground4.c"
#include "imgBackground5.c"

#define CUBE_MAX        200       
#define CUBE_BOOM_MAX   40        
#define CUBE_BASIC_MAX   7        
#define SPECIA_DATA_MAX 20        


const uint8_t CUBE_I[2][4] = {{12,13,14,15},{0,4,8,12}};
const uint8_t CUBE_O[1][4] = {{8,9,12,13}};
const uint8_t CUBE_T[4][4] = {{9,12,13,14},{4,8,9,12},{8,9,10,13},{5,8,9,13}};
const uint8_t CUBE_L1[4][4] = {{10,12,13,14},{4,8,12,13},{8,9,10,12},{4,5,9,13}};
const uint8_t CUBE_L2[4][4] = {{8,12,13,14},{4,5,8,12},{8,9,10,14},{5,9,12,13}};
const uint8_t CUBE_N1[2][4] = {{8,9,13,14},{5,8,9,12}};
const uint8_t CUBE_N2[2][4] = {{9,10,12,13},{4,8,9,13}};

struct CubeData{
  uint8_t x;                        
  uint8_t y;                        
  uint8_t w;                        
  uint8_t h;                        
  const unsigned short *img;        
};

struct CubeBoomData{
  bool use;                       
  float x;                        
  float y;                        
  float sDecY;                    
  float sX;                       
  float sY;                       
  const unsigned short *img;      
};

struct PreviousCube{
  uint8_t x;                        
  short   y;                        
  uint8_t id;                       
  uint8_t state;                    
};

struct GameData{
  byte Ver;                         
  byte ScreenRotation;              
  bool MusicSwitch;                 
  bool SoundSwitch;                 
  unsigned short RankingLineMax[3]; 
  unsigned long RankingScore[3];    
};

struct SpecialData{
  bool use;                         
  uint8_t x;                        
  uint8_t y;                        
  const unsigned short *img;        
};

class GamePlay
{
public:
  
  CubeData cubeData[CUBE_MAX];
  CubeBoomData cubeBoomData[CUBE_BOOM_MAX];  
  CubeData cubeNextData[CUBE_BASIC_MAX];
  CubeData cubeFlash[CUBE_BASIC_MAX];
  CubeData specialMallet;
  
  bool specialMalletDrop = false;
  GameData gameData;

  SpecialData specialData[SPECIA_DATA_MAX];
  uint8_t gamingZone[10][20];
  uint8_t nextCubeId=0;
  short   cubeX =  4;
  short   cubeY = -3;
  uint8_t cubeId = 0;                       
  uint8_t cubeState = 0;                    

  unsigned long int cubeScore = 0;    
  uint8_t cubeScoreTemp = 0;    
  unsigned short cubeLineMax = 0;
  unsigned short cubeLineNow = 0;
  unsigned short cubeSpeed = 1;
  unsigned short cubeDownSpeed = 500;  
  short cubeStage = 1;
  unsigned short cubeNow[CUBE_BASIC_MAX]    = {0,0,0,0,0,0,0};
  unsigned short cubeMax[CUBE_BASIC_MAX]    = {0,0,0,0,0,0,0};
  unsigned short cubeUpgrade[CUBE_BASIC_MAX]= {20,25,35,40,20,25,20};
   
  bool cubeClear = false;    
  uint8_t specialID = 0;  
  unsigned short specialSec = 0;  
  uint8_t specialBegin = 0;  
  uint8_t specialTemp = 0;

  GamePlay() {
    Serial.begin(9600);
    
    gameData.Ver = 0;
    gameData.ScreenRotation = 0;                
    gameData.MusicSwitch = false;                
    gameData.SoundSwitch = true;                
    for(uint8_t i=0; i<3; i++){
      gameData.RankingLineMax[i] = (3-i) * 1;  
      gameData.RankingScore[i]   = (3-i) * 10; 
    }      
                 
    for(uint8_t i=0; i< CUBE_MAX; i++){
      
      cubeData[i].x = 1+(10*(i%10));
      cubeData[i].y = 226-(10*(i/10));
      cubeData[i].w = 13;
      cubeData[i].h = 13;
    }

    for(uint8_t i=0; i< CUBE_BOOM_MAX; i++){
      cubeBoomData[i].use = false;
      cubeBoomData[i].img = NULL;
    }

    for(uint8_t i=0; i< CUBE_BASIC_MAX; i++){
      cubeFlash[i].x = 107;
      cubeFlash[i].y = 134 + (i*15);
      cubeFlash[i].w = 1;              
      cubeFlash[i].h = 0;
      cubeFlash[i].img = getImg(10);
    }
    
    specialMallet.x = 41;
    specialMallet.y = 36;
    specialMallet.w = 32;
    specialMallet.h = 22;
    specialMallet.img = mallet;

    cubeNextData[0].x = 109; cubeNextData[0].y = 56;
    cubeNextData[0].w = 22;  cubeNextData[0].h = 7;   
    cubeNextData[0].img = cubeM01;
    
    cubeNextData[1].x = 116; cubeNextData[1].y = 54;
    cubeNextData[1].w = 12;  cubeNextData[1].h = 12;
    cubeNextData[1].img = cubeM02;
    
    cubeNextData[2].x = 111; cubeNextData[2].y = 53;
    cubeNextData[2].w = 17;  cubeNextData[2].h = 12;
    cubeNextData[2].img = cubeM03;
    
    cubeNextData[3].x = 111; cubeNextData[3].y = 53;
    cubeNextData[3].w = 17;  cubeNextData[3].h = 12;
    cubeNextData[3].img = cubeM04;
    
    cubeNextData[4].x = 111; cubeNextData[4].y = 53;
    cubeNextData[4].w = 17;  cubeNextData[4].h = 12;
    cubeNextData[4].img = cubeM05;
    
    cubeNextData[5].x = 111; cubeNextData[5].y = 53;
    cubeNextData[5].w = 17;  cubeNextData[5].h = 12;
    cubeNextData[5].img = cubeM06;
    
    cubeNextData[6].x = 111; cubeNextData[6].y = 53;
    cubeNextData[6].w = 16;  cubeNextData[6].h = 12;
    cubeNextData[6].img = cubeM07;    

    previousCube.state = 99;
    
    setNextCube();    
    gameReset();
  }

  void GameLoad(){
    if(!EEPROM_ENABLE)
      return;

    gameData.Ver = EEPROM_readByte(0);
    if(gameData.Ver==0){
      gameData.Ver = 1;
      GameSave();
      return;
    }      
    gameData.ScreenRotation = EEPROM_readByte(1);     
    gameData.MusicSwitch = (bool)EEPROM_readByte(2);  
    gameData.SoundSwitch = (bool)EEPROM_readByte(3);  

    gameData.RankingLineMax[0] = EEPROM_readInt(4);   
    gameData.RankingLineMax[1] = EEPROM_readInt(6);   
    gameData.RankingLineMax[2] = EEPROM_readInt(8);   
    
    gameData.RankingScore[0]   = EEPROM_readLong(10); 
    gameData.RankingScore[1]   = EEPROM_readLong(14); 
    gameData.RankingScore[2]   = EEPROM_readLong(18); 
  }

  void GameSave(){
    if(!EEPROM_ENABLE)
      return;
    
    EEPROM_writeByte(gameData.Ver);
    EEPROM_writeByte(gameData.ScreenRotation);
    EEPROM_writeByte(gameData.MusicSwitch);
    EEPROM_writeByte(gameData.SoundSwitch);
    
    EEPROM_writeInt(gameData.RankingLineMax[0]);
    EEPROM_writeInt(gameData.RankingLineMax[1]);
    EEPROM_writeInt(gameData.RankingLineMax[2]);
    
    EEPROM_writeLong(gameData.RankingScore[0]);
    EEPROM_writeLong(gameData.RankingScore[1]);
    EEPROM_writeLong(gameData.RankingScore[2]);        
  }

  void decodingCubeSingle(uint8_t id, uint8_t state) {          
      
      for(uint8_t x=0; x<4; x++){
        for(uint8_t y=0; y<4; y++){
          cubeSingle[x][y] = 0;
        }
      }
      
      for(uint8_t i=0; i<4; i++){
        
        if(id==0 && state < 2){        
          cubeSingle[CUBE_I[state][i]/4][CUBE_I[state][i]%4] = 1;          
        
        }else if(id==1&& state < 1){
          cubeSingle[CUBE_O[state][i]/4][CUBE_O[state][i]%4] = 4;                  
        
        }else if(id==2 && state < 4){
          cubeSingle[CUBE_T[state][i]/4][CUBE_T[state][i]%4] = 6;
        
        }else if(id==3 && state < 4){
          cubeSingle[CUBE_L1[state][i]/4][CUBE_L1[state][i]%4] = 3;
        
        }else if(id==4 && state < 4){
          cubeSingle[CUBE_L2[state][i]/4][CUBE_L2[state][i]%4] = 2;
        
        }else if(id==5 && state < 2){  
          cubeSingle[CUBE_N1[state][i]/4][CUBE_N1[state][i]%4] = 7;
        
        }else if(id==6 && state < 2){
          cubeSingle[CUBE_N2[state][i]/4][CUBE_N2[state][i]%4] = 5;
        }        
      }      
  }

  void setNextCube(){
    nextCubeId = random(7);
  }

  void setPreviousCube(bool cls){
    
    if(cls){
      
      for(uint8_t x=0; x<4; x++){
        for(uint8_t y=0; y<4; y++){      
          
          if(previousCube.y<0)
            if((previousCube.y+y)<0)
              continue;
          
          if(cubeSingle[y][x] != 0)
            gamingZone[previousCube.x+x][previousCube.y+y] = 0;
        }
      }      
    
    }else{
      
      for(uint8_t x=0; x<4; x++){
        for(uint8_t y=0; y<4; y++){
          
          if(previousCube.y<0)
            if((previousCube.y+y)<0)
              continue;
                    
          if(cubeSingle[y][x] != 0)
            gamingZone[previousCube.x+x][previousCube.y+y] = cubeSingle[y][x];
        }
      }      
    }
  }

  bool copyToGamingZone( short ix, short iy, uint8_t id, uint8_t state, bool move){
    
    if(previousCube.state != 99)
      setPreviousCube(true);
    
    decodingCubeSingle(id, state);
    
    for(uint8_t x=0; x<4; x++){
      for(uint8_t y=0; y<4; y++){
        
        if(iy<0)
          if((iy+y)<0)
            continue;
                    
        if(cubeSingle[y][x] != 0){
          if((ix+x)>9 ||  gamingZone[ix+x][iy+y] != 0){
            
            if(previousCube.state != 99){
              
              decodingCubeSingle(id, cubeState);
              
              setPreviousCube(false);
            }
            return false;
          }
        }
        
      }
    }          
        
    previousCube.id = id;
    previousCube.state = state;
    previousCube.x  = ix;
    previousCube.y  = iy;    

    setPreviousCube(false);    
    
    if(!move)
      previousCube.state = 99;    
      
    return true;
  }

  bool cubeStop(){    
    if(copyToGamingZone( cubeX, cubeY, cubeId, cubeState, false)){
      
      ifClearCube();
      
      cubeX =  4;
      cubeY = -3;
      cubeId = nextCubeId;
      cubeState = 0;
      
      setNextCube();
    }
    return cubeClear;
  }

  void cubeChange(bool cw){
    short cubeStateTemp = cubeState;

    if(cubeId == 0 || cubeId == 5 || cubeId == 6){
      
      if((cubeId == 5 || cubeId == 6) && cubeX>7)
        cubeX = 7;      
      cubeStateTemp++;
      if(cubeStateTemp>1)
        cubeStateTemp=0;        
    
    }else if(cubeId == 1){
        cubeStateTemp=0;
    
    }else if(cubeId == 2 || cubeId == 3 || cubeId == 4){
      
      if(cubeX>7)
        cubeX = 7;
      
      if(cw){
        cubeStateTemp++;
        if(cubeStateTemp>3)
          cubeStateTemp=0;
      }else{
        cubeStateTemp--;
        if(cubeStateTemp<0)
          cubeStateTemp=2;        
      }
    }        
    
    if(copyToGamingZone( cubeX, cubeY, cubeId, cubeStateTemp, true))
      cubeState = cubeStateTemp;    
  }

  bool moveCube(byte dir){
    
    if(dir==0){
      if(cubeY>-3){
        cubeY--;
        if(!copyToGamingZone( cubeX, cubeY, cubeId, cubeState, true)){
          cubeY++;
          return false;
        }
      }
    
    }else if(dir==1){
      if(cubeY<16){
        cubeY++;        
        if(!copyToGamingZone( cubeX, cubeY, cubeId, cubeState, true)){
          cubeY--;
          return false;
        }
      }
    
    }else if(dir==2){
      if(cubeX>0){
        cubeX--;  
        if(!copyToGamingZone( cubeX, cubeY, cubeId, cubeState, true)){
          cubeX++;
          return false;
        }
      }
    
    }else if(dir==3){
      if(cubeX<9){
        cubeX++;  
        if(!copyToGamingZone( cubeX, cubeY, cubeId, cubeState, true)){
          cubeX--;
          return false;
        }
      }
    }
    return true;
  }

  void ifClearCube(){
    uint8_t i;
    uint8_t s=0;
    
    for(uint8_t y=0; y<20; y++){
      i=0;
      for(uint8_t x=0; x<10; x++){
        
        if(gamingZone[x][y]!=0 && gamingZone[x][y]!=12&& gamingZone[x][y]!=13)
          i++;
      }
      
      if(i==10){
        s++;        
        
        cubeClear = true;
        cubeLineNow++;
        setDownSpeed();          
        
        for(uint8_t j=0; j<10; j++){          
          
          if(specialSec==0){
            
            cubeNow[gamingZone[j][y]-1] += 1;                      
          }          
          setCubeBoom((y*10)+j);          
          gamingZone[j][y] = 10;          
        }        
      }
    }
    
    if(cubeClear){
      
      if(s==1)
        addScore(10);
      else if(s==2)
        addScore(30);
      else if(s==3)
        addScore(60);
      else if(s==4)
        addScore(120);       
      
      clearCube();
    }    
  }

  void clearCube(){    
    bool sp=false;
    for(uint8_t i=0; i<SPECIA_DATA_MAX; i++){
      if(specialData[i].use)
        gamingZone[specialData[i].x][specialData[i].y]=0;      
    }
      
    for(uint8_t x=0; x<10; x++){  
      for(uint8_t y=0; y<20; y++){    
        if(gamingZone[x][y]==10){
          
          for(int i=y; i>0; i--)            
            gamingZone[x][i] = gamingZone[x][i-1];          
        }
      }      
    }    
        
    for(uint8_t i=0; i<7; i++){  
      if((cubeNow[i] >= cubeMax[i]) && specialSec==0 ){
        special(i+1);
        sp = true;
      }      
    }
    
    cubeClear = false;
    
    if(sp)
      ifClearCube();
    
    if(specialSec == 0)
      playSound(0);    

  }

  void  clearGamingZone(int id){    
    for(uint8_t y=0; y<20; y++){
      for(uint8_t x=0; x<10; x++){  
        gamingZone[x][y] = id;        
      }
    }    
  }

  void gameReset(){
    
    cubeSpeed = 1;
    cubeDownSpeed = 500;
    cubeStage = 1;
    cubeLineNow = 0;
    cubeScore = 0;
    cubeX =  4;
    cubeY = -3;
    
    cubeMax[0] = 10;
    cubeMax[1] = 30;
    cubeMax[2] = 40;
    cubeMax[3] = 50;
    cubeMax[4] = 30;
    cubeMax[5] = 20;
    cubeMax[6] = 25;  
    for(uint8_t i=0; i<7; i++)
      cubeNow[i] = 0;
    
    clearGamingZone(0);
  }
  
  void gameOver(){ 
    
    if(cubeLineNow>cubeLineMax){
      cubeLineMax = cubeLineNow;        
    }       
    
    for(uint8_t i=0; i<3; i++){
      
      if(cubeScore>gameData.RankingScore[i]){
        
        if(i==0){          
          
          gameData.RankingScore[2] ^= gameData.RankingScore[1];
          gameData.RankingScore[1] ^= gameData.RankingScore[2];
          gameData.RankingScore[2] ^= gameData.RankingScore[1];
          gameData.RankingScore[1] ^= gameData.RankingScore[0];
          gameData.RankingScore[0] ^= gameData.RankingScore[1];
          gameData.RankingScore[1] ^= gameData.RankingScore[0];
          gameData.RankingScore[0] = cubeScore;
          
          gameData.RankingLineMax[2] ^= gameData.RankingLineMax[1];
          gameData.RankingLineMax[1] ^= gameData.RankingLineMax[2];
          gameData.RankingLineMax[2] ^= gameData.RankingLineMax[1];
          gameData.RankingLineMax[1] ^= gameData.RankingLineMax[0];
          gameData.RankingLineMax[0] ^= gameData.RankingLineMax[1];
          gameData.RankingLineMax[1] ^= gameData.RankingLineMax[0];
          gameData.RankingLineMax[0] = cubeLineNow;          
        
        }else if(i==1){
          
          gameData.RankingScore[2] ^= gameData.RankingScore[1];
          gameData.RankingScore[1] ^= gameData.RankingScore[2];
          gameData.RankingScore[2] ^= gameData.RankingScore[1];
          gameData.RankingScore[1] = cubeScore;
          
          gameData.RankingLineMax[2] ^= gameData.RankingLineMax[1];
          gameData.RankingLineMax[1] ^= gameData.RankingLineMax[2];
          gameData.RankingLineMax[2] ^= gameData.RankingLineMax[1];
          gameData.RankingLineMax[1] = cubeLineNow;
        
        }else if(i==2){
          gameData.RankingScore[2] = cubeScore;
          gameData.RankingLineMax[2] = cubeLineNow;
        }        
        
        GameSave();        
        break;
      }      
    }
    
    for(uint8_t y=0; y<20; y++){
      for(uint8_t x=0; x<10; x++){  
        if(gamingZone[x][y]!=0)
          gamingZone[x][y] = 10;        
      }
    }
  }

  void addScore(uint8_t s){
    cubeScoreTemp+=s;
  }

  void updateScore(){
    if(cubeScoreTemp>0){
      cubeScoreTemp--;
      cubeScore+=1;
      if(cubeScore>99999)
        cubeScore=99999;          
    }      
  }
    
  void setCubeBoom(uint8_t pos){
    uint8_t p = (CUBE_MAX-1) - pos;        
    
    float ranX = (random(10, 200)+10) * 0.01f;
    float ranY = (random(10, 200)+10) * 0.01f;
    
    
    for(uint8_t i=0; i< CUBE_BOOM_MAX; i++){
      if(!cubeBoomData[i].use){
        
        cubeBoomData[i].x = cubeData[p].x;   
        cubeBoomData[i].y = cubeData[p].y;   
                       
        cubeBoomData[i].sX =   1.0f-ranX;    
        cubeBoomData[i].sY = -15.0f;         
        cubeBoomData[i].sDecY = ranY+0.5f;   

        
        if(cubeData[p].img==NULL)
          cubeBoomData[i].img = getImg(7);
        else
          cubeBoomData[i].img = cubeData[p].img;          
        
        cubeBoomData[i].use = true;  
        
        addScore(1);          
        break;
      }      
    }     
      
  }

  void updateCubeBoom(uint8_t pos){ 
    cubeBoomData[pos].x  += cubeBoomData[pos].sX;
    cubeBoomData[pos].y  += cubeBoomData[pos].sY;
    cubeBoomData[pos].sY += cubeBoomData[pos].sDecY;
    
    if(cubeBoomData[pos].y>240)
      cubeBoomData[pos].use = false;
  }

  const unsigned short* getBackground(){
    if(cubeSpeed>8)
      return background5;
    else if(cubeSpeed>6)
      return background4;
    else if(cubeSpeed>4)
      return background3;      
    else if(cubeSpeed>2)
      return background2;
    else
      return background1;      
  }

  const unsigned short* getImg(uint8_t id){    
    if(id == 1){
      return cube01;
    }else if(id == 2){
      return cube02;
    }else if(id == 3){
      return cube03;
    }else if(id == 4){
      return cube04;
    }else if(id == 5){
      return cube05;
    }else if(id == 6){
      return cube06;
    }else if(id == 7){
      return cube07;
    }else if(id == 8){
      return cube08;
    }else if(id == 9){
      return cube09;
    }else if(id ==10){
      return cube10;        
    }else if(id ==11){
      return cube11;
    }else if(id ==12){
      return cube07;
    }else if(id ==13){
      return cube01;      
    }
    return NULL;    
  }

  void specialInit(uint8_t id){
    
    if(id==0){
      specialID = 0;
      specialSec = 1000;
      
      for(uint8_t i=0; i<SPECIA_DATA_MAX; i++){
        specialData[i].use = false;
        specialData[i].x = 5;
        specialData[i].y = 0;
        specialData[i].img = cubeM07;        
      }
      
      specialData[0].use = true;
      gamingZone[specialData[0].x][specialData[0].y]= 12;

    
    }else if(id==1){
      
      specialMalletDrop = false;
      
      specialID = 1;
      
      specialSec = 10;
      
      specialMallet.x = 41;
      specialMallet.y = 36;

    }else if(id==2){
      
      specialID = 2;
      
      specialSec = 1000;
      
      specialTemp = 0;

    
    }else if(id==3){
      
      specialID = 3;
      
      specialSec = 1000;
      
      specialTemp = 0;

    
    }else if(id==4){
      
      specialID = 4;
      
      specialSec = 1000;
      
      specialTemp = 0;

    
    }else if(id==5){
      
      specialID = 5;
      
      specialSec = 1000;
      
      specialBegin = 0;
      specialTemp = 30;

    
    }else if(id==6){
      
      specialID = 6;
      
      specialSec = 1000;
      
      specialTemp = 0;
      
      for(uint8_t i=0; i<10; i++){
        specialData[i].use = false;
        specialData[i].x = i;
        specialData[i].y = 0;
        specialData[i].img = cubeM01;        
      }
      
      specialFireCube();
    
    }
  }
  
  void specialUpdate(){
    int x1,y1;
    uint8_t i,j;
    
    if(specialSec>0){
      if(specialID == 0){
        if(specialData[0].use)
          gamingZone[specialData[0].x][specialData[0].y] = 12;
        
        for(uint8_t i=1; i<SPECIA_DATA_MAX; i++){
          if(specialData[i].use){
            
            if(gamingZone[specialData[i].x][specialData[i].y+1]==0){            
              
              gamingZone[specialData[i].x][specialData[i].y] = 0;
              
              specialData[i].x = specialData[i].x;
              specialData[i].y = specialData[i].y+1;
              gamingZone[specialData[i].x][specialData[i].y] = 12;
              
              if(specialData[i].y==19){
                
                gamingZone[specialData[i].x][specialData[i].y] = 7;
                specialData[i].use = false;
                
                ifClearCube();              
              }
              
            }else{
              
              gamingZone[specialData[i].x][specialData[i].y] = 7;
              specialData[i].use = false;
              
              ifClearCube();
            }
          }
        }
                  
      }else if(specialID == 1){
        
        if(specialMalletDrop){
          specialMallet.y += 10;                    
          
          if(specialMallet.y > 226){
            specialMalletDrop=false;
            specialSec = 0;
          }          
          
          x1 = (specialMallet.x-1)/10;
          y1 = (specialMallet.y-46)/10;
          
          gamingZone[x1][y1] = 0;
          gamingZone[x1+1][y1] = 0;
          gamingZone[x1+2][y1] = 0;          
        }
        
      }else if(specialID == 2){                      
        for(uint8_t y=0; y<20; y++){
          i = 19-y;
          for(uint8_t x=0; x<10; x++){
            if(gamingZone[x][i]==0){
              gamingZone[x][i] = 5;
              
              specialTemp++;
              if(specialTemp==10){              
                
                specialSec = 0; 
                
                ifClearCube();
                return;
              }
              return;
            }
          }
        }  
        
      }else if(specialID == 3){        
        i = 19-(specialTemp/10);

        j=0;
        for(uint8_t x=0; x<10; x++){
          if(gamingZone[x][i]==0)
            j++;
        }
        if(j==10){
          specialTemp=30;
        }        
        
        
        if(specialTemp==28){ 
          gamingZone[8][i] = 4;
          gamingZone[9][i] = 4;
        }else if(specialTemp==30){
          
          specialSec = 0; 
          
          ifClearCube();              
          return;
        }            
        gamingZone[specialTemp%10][i] = 4;
        
        specialTemp++;          
        
      }else if(specialID == 4){
        if ((specialTemp/10)==0){
          i = 19;
        }else if ((specialTemp/10)==1){
          i = 17;
        }else if ((specialTemp/10)==2){
          i = 15;                    
        }
        
        j=0;
        for(uint8_t x=0; x<10; x++){
          if(gamingZone[x][i]==0)
            j++;
        }
        if(j==10){
          specialTemp=30;
        }        
        
        if(specialTemp==28){ 
          gamingZone[8][i] = 3;
          gamingZone[9][i] = 3;
        }else if(specialTemp==30){
          
          specialSec = 0; 
          
          ifClearCube();              
          return;
        }            
        gamingZone[specialTemp%10][i] = 3;
        
        specialTemp++;
        
      }else if(specialID == 5){                        
        
        if(specialBegin==0){
          for(uint8_t y=0; y<20; y++){
            for(uint8_t x=0; x<10; x++){
              if(gamingZone[x][y]!=0){
                specialBegin = (y*10);
                return;
              }
            }          
          }                    
        }                
        
        if(specialBegin==0){
          
          specialSec = 0;           
        
        }else{
          gamingZone[specialBegin%10][(specialBegin/10)] = 2;
          specialBegin++;
          specialTemp--;
          
          if(specialBegin==197 || specialTemp==2){
            gamingZone[specialBegin%10][(specialBegin/10)] = 2;
            gamingZone[(specialBegin+1)%10][(specialBegin/10)] = 2;
            gamingZone[(specialBegin+2)%10][(specialBegin/10)] = 2;
          
          }if(specialBegin==199 || specialTemp==0){
            
            specialSec = 0;             
            
            ifClearCube();                          
          }
        }        
        
      }else if(specialID == 6){
        
        for(uint8_t i=0; i<10; i++){
          if(specialData[i].use){
            
            if(gamingZone[specialData[i].x][specialData[i].y+1]==0){            
              
              gamingZone[specialData[i].x][specialData[i].y] = 0;
              
              specialData[i].x = specialData[i].x;
              specialData[i].y = specialData[i].y+1;
              gamingZone[specialData[i].x][specialData[i].y] = 13;
              
              if(specialData[i].y==19){
                
                gamingZone[specialData[i].x][specialData[i].y] = 1;
                specialData[i].use = false;
                
                specialTemp++;                                
                if(specialTemp>=10){
                  
                  specialSec = 0;
                  
                  ifClearCube();
                }
              }
              
            
            }else{
              
              gamingZone[specialData[i].x][specialData[i].y] = 1;
              specialData[i].use = false;
              
              specialTemp++;                                
              if(specialTemp>=10){
                
                specialSec = 0;
                
                ifClearCube();
              }
            }            
          }
        }        
      }
      
    }
  }

  bool specialFireCube(){
    
    if(specialID == 0){
      
      if(specialData[0].y>=19)
        return true;
      
      
      for(uint8_t i=1; i<SPECIA_DATA_MAX; i++){
        if(!specialData[i].use){
          
          specialData[i].x = specialData[0].x;
          specialData[i].y = specialData[0].y+1;
          
          if(gamingZone[specialData[i].x][specialData[i].y]!=0)
            return true;
          
          gamingZone[specialData[i].x][specialData[i].y]=12;
          
          specialData[i].use = true;
          break;
        }
      }    
      
    
    }else if(specialID == 1){
      specialMalletDrop=true;

    
    }else if(specialID == 6){
      
      for(uint8_t i=0; i<10; i++){
        if(!specialData[i].use){
          
          specialData[i].x = i;
          specialData[i].y = 0;
          
          if(gamingZone[specialData[i].x][specialData[i].y]!=0)
            return true;
          
          gamingZone[specialData[i].x][specialData[i].y]=13;
          
          specialData[i].use = true;          
        }
      }
    }
    return false;
  }

  void specialMoveL(){    
    
    if(specialID == 0){
      gamingZone[specialData[0].x][specialData[0].y] = 0;
      if(specialData[0].x>0){
        if(gamingZone[specialData[0].x-1][specialData[0].y] == 0)
          specialData[0].x--;      
      }
      gamingZone[specialData[0].x][specialData[0].y] = 12;
      
    
    }else if(specialID == 1){
      if(!specialMalletDrop){
        if(specialMallet.x > 10)
          specialMallet.x -= 10;
      }
    }    
  }

  void specialMoveR(){
    
    if(specialID == 0){
      gamingZone[specialData[0].x][specialData[0].y] = 0;
      if(specialData[0].x<9){
        if(gamingZone[specialData[0].x+1][specialData[0].y] == 0)
          specialData[0].x++;
      }
      gamingZone[specialData[0].x][specialData[0].y] = 12;
      
    
    }else if(specialID == 1){
      if(!specialMalletDrop){
        if(specialMallet.x < 70)
          specialMallet.x += 10;        
      }
    }        
  }

  bool specialMoveD(){
    
    if(specialID == 0){
      gamingZone[specialData[0].x][specialData[0].y] = 0;
      if(specialData[0].y<19){
        if(gamingZone[specialData[0].x][specialData[0].y+1] == 0){
          specialData[0].y++;
          if(specialData[0].y==19)
            return true;
        }else{
          return true;
        }
      }
      gamingZone[specialData[0].x][specialData[0].y] = 12;      
    }            
    return false;
  }

  void special(uint8_t id){
    uint8_t i,j;

    
    i = id-1;
    cubeNow[i] = 0;
    cubeMax[i] += cubeUpgrade[i];
    if(cubeMax[i]>999)
      cubeMax[i] = 999;
    
    
    if(id==1){
      specialInit(6);
    
    }else if(id==2){
      specialInit(5);
    
    }else if(id==3){
      specialInit(4);
    
    }else if(id==4){
      specialInit(3);
    
    }else if(id==5){      
      specialInit(2);      
    
    }else if(id==6){
      specialInit(1);            
    
    }else if(id==7){
      specialInit(0);
    }
        
  }

  void setDownSpeed(){    
    if((cubeLineNow%10)==0){
      
      cubeStage++;
      cubeSpeed++;
      if(cubeSpeed>9)
        cubeSpeed = 1;        
      if(cubeSpeed==1)
        cubeDownSpeed = 500;
      else if(cubeSpeed==2)
        cubeDownSpeed = 450;
      else if(cubeSpeed==3)
        cubeDownSpeed = 400;
      else if(cubeSpeed==4)
        cubeDownSpeed = 350;
      else if(cubeSpeed==5)
        cubeDownSpeed = 300;
      else if(cubeSpeed==6)
        cubeDownSpeed = 250;
      else if(cubeSpeed==7)
        cubeDownSpeed = 200;
      else if(cubeSpeed==8)
        cubeDownSpeed = 100;
      else if(cubeSpeed==9)
        cubeDownSpeed =  50;
    }        
  }
  
  void update() {   
    uint8_t id = 0;
    for(uint8_t i=0; i< CUBE_MAX;i++){      
      id = gamingZone[i%10][19-(i/10)];
      cubeData[i].img = getImg(id);
    }

    specialUpdate();    
  }
    
private:  
  uint8_t cubeSingle[4][4];  
  PreviousCube previousCube;
  
};

#endif
