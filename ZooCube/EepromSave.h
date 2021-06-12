#ifndef EEPROM_SAVE_H
#define EEPROM_SAVE_H

#include <EEPROM.h>

bool EEPROM_ENABLE = true;

unsigned int  EEPROM_SIZE = 0;
unsigned int  EEPROM_INDEX = 0;

void EEPROM_Init(int size)
{ 
  EEPROM_INDEX = 0;
  EEPROM_SIZE = size;
  
  EEPROM.begin(EEPROM_SIZE);
}

void EEPROM_writeByte(int number, int index=-1)
{   
  if(index==-1){
    EEPROM.write(EEPROM_INDEX, number);
    EEPROM_INDEX+=1;
  }else{
    EEPROM.write(index, number);
  }
  EEPROM.commit();
  
}
int EEPROM_readByte(int address)
{
  return EEPROM.read(address);
}

void EEPROM_writeInt(int number)
{ 
  EEPROM.write(EEPROM_INDEX, number >> 8);
  EEPROM.write(EEPROM_INDEX + 1, number & 0xFF);
  EEPROM.commit();
  EEPROM_INDEX+=2;
}
int EEPROM_readInt(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

void EEPROM_writeLong(long number)
{ 
  EEPROM.write(EEPROM_INDEX, (number >> 24) & 0xFF);
  EEPROM.write(EEPROM_INDEX + 1, (number >> 16) & 0xFF);
  EEPROM.write(EEPROM_INDEX + 2, (number >> 8) & 0xFF);
  EEPROM.write(EEPROM_INDEX + 3, number & 0xFF);
  EEPROM.commit();
  EEPROM_INDEX+=4;
}
long EEPROM_readLong(int address)
{
  return ((long)EEPROM.read(address) << 24) +
         ((long)EEPROM.read(address + 1) << 16) +
         ((long)EEPROM.read(address + 2) << 8) +
         (long)EEPROM.read(address + 3);
}

void EEPROM_clearAll()
{
   unsigned char data = 0;
   for (unsigned int Count=0; Count<EEPROM_SIZE; Count++)
   {  
       EEPROM.write(Count, data);
   }
}

#endif
