#include "common.h"

#define READ_ADDRESS	(uint16_t)0xBD
#define WRITE_ADDRESS	(uint16_t)0xBC
#define PI				(double)3.14159265
#define RADTODEG 		(180/PI)



uint8_t WriteBuffer[4] ;
uint8_t ReadBuffer[10];
int16_t Reading[3];
int16_t x;
int16_t y;
int16_t z;
double Phi;
uint8_t Flag;
void  ReadSensor(void);
void Init_3DMagneticSensor(void)
{

	uint8_t InitWrite[4]={0x00, 0x05,0x00,0x04};
	HAL_I2C_Master_Receive(&i2c1,READ_ADDRESS, ReadBuffer,10,100);
	WriteBuffer[0] = 0x00;
	WriteBuffer[1] = ReadBuffer[7] & 0x78;
	WriteBuffer[2] = ReadBuffer[8];
	WriteBuffer[3] = ReadBuffer[9] & 0x0F;
	for(int i =0; i<4; i++)
	{
		WriteBuffer[i] |= InitWrite[i];
	}
	HAL_I2C_Master_Transmit(&i2c1,WRITE_ADDRESS, WriteBuffer,4,100);
}
void GoToSleep(void)
{
  uint8_t Sleep[2] ={0}; 
	HAL_I2C_Master_Transmit(&i2c1,WRITE_ADDRESS, Sleep,2,100);
}

void findAngle(double x, double y)
{
	if(x> 0){
		Phi = atan2(y,x)*RADTODEG;
	}else if(x ==0){
		if(y<0){
			Phi = -90;
		}else{
			Phi = 90;
		}
	}else if(x<0){
		if(y>=0){
			Phi = (atan2(y,x)+PI)*RADTODEG;
		}else{
			Phi = (atan2(y,x)-PI)*RADTODEG;
		}
	}
}
void SensorAlive(void)
{
  
}

void ReadSensor(void)
{
  static uint8_t OldFRM = 0;
  static uint8_t CurrentFRM = 0;
	HAL_I2C_Master_Receive(&i2c1,READ_ADDRESS, ReadBuffer,6,100);
	Reading[0] = (ReadBuffer[0]<<4) | ((ReadBuffer[4] &0xF0)>>4);
	x = Reading[0];
	if(Reading[0] & 0x800)
	{
		Reading[0] &=0x7FF;
		Reading[0] -=2048;
		x = Reading[0];
	}
	Reading[1] = (ReadBuffer[1]<<4) | ((ReadBuffer[4]&0x0F));
	y = Reading[1];
	if(Reading[1] & 0x800)
	{
		Reading[1] &=0x7FF;
		Reading[1] -=2048;
		y = Reading[1];
	}
	Reading[2] = (ReadBuffer[2]<<4) | ((ReadBuffer[5]&0x0F));
	z = Reading[2];
	if(Reading[2] & 0x800)
	{
		Reading[2] &=0x7FF;
		Reading[2] -=2048;
		z = Reading[2];
	}

  CurrentFRM = ReadBuffer[3] & 0x0C;

  if(CurrentFRM == OldFRM)
  {
    //ADC hang up we need to reset the sensor
    printf("Sensor is Stuck resetting .. \n");
    HAL_I2C_Master_Transmit(&i2c1,0x00,0x00,1,100);
    Init_3DMagneticSensor();
    CurrentFRM = 0;
  }
  OldFRM = CurrentFRM;

}

void MagneticSensorTask(void)
{
  if(MangeticConverstionReady()){

        ReadSensor();
        findAngle(x,y);
        printf("Phi = %f\n" , Phi);
        HAL_Delay(500);
   }

}

void CmdRead(int mode){

        ReadSensor();
        findAngle(x,y);
        printf("Phi = %f\n" , Phi);
        return;
}
ADD_CMD("read", CmdRead,"                read sensor");
