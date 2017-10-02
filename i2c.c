#include "common.h"
#include "stm32f3xx_hal.h"

I2C_HandleTypeDef i2c2;
void i2cInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  __GPIOA_CLK_ENABLE();
  __I2C2_CLK_ENABLE();


  i2c2.Instance = I2C2;
  i2c2.Init.Timing = 0x00E0257A; //400kHz
  i2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  i2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  i2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  i2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  if(HAL_I2C_Init(&i2c2) !=HAL_OK)
  {
    printf("Error in I2C initializaion\n");
  }
  
  GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
 
}

