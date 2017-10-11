#include "common.h"

I2C_HandleTypeDef i2c1;
void i2cInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __I2C2_CLK_ENABLE();


  i2c1.Instance = I2C1;
  i2c1.Init.Timing = 0x10808DD3; //400kHz
  i2c1.Init.OwnAddress1        = 0x00;
  i2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  i2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  i2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  i2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  i2c1.Init.OwnAddress2        = 0x00;
  i2c1.Init.OwnAddress2Masks   = I2C_OA2_NOMASK;
  if(HAL_I2C_Init(&i2c1) !=HAL_OK)
  {
    printf("Error in I2C initializaion\n");
  }
  
  __GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}


uint32_t MangeticConverstionReady(void)
{
  if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9)){
    while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9));
    return 1;
  }
  return 0;
}

