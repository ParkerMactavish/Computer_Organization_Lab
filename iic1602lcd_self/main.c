/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without
modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice,
this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors
may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#include <stdio.h>

#include "embARC.h"
#include "embARC_debug.h"
#include "iic1602lcd.h"

pLCD_t lcd_obj;

const char degreeC[3] = {0xDF, 'C', '\0'};

/**
 * \brief	Test hardware board without any peripheral
 */
int main(void)
{
  lcd_obj = LCD_Init(0);

  iic_ptr->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
  iic_ptr->iic_control(IIC_CMD_MST_SET_TAR_ADDR, 0x3e);

  // uint8_t displayFunction = LCD_2LINE;

  // arc_delay_us(50000);

  // displayFunction |= LCD_FUNCTIONSET;

  iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void *)(IIC_MODE_STOP));
  uint8_t data[2] = {
      0x80,
      0};
  // iic_ptr->iic_write(data, 2);

  arc_delay_us(4500);

  uint8_t displaycontrol = LCD_DISPLAYON;
  data[1] = displaycontrol | LCD_DISPLAYCONTROL;
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  iic_ptr->iic_write(data, 2);

  data[1] = LCD_CLEARDISPLAY;
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  iic_ptr->iic_write(data, 2);
  arc_delay_us(2000);

  uint8_t displayMode = LCD_ENTRYLEFT;
  data[1] = displayMode | LCD_ENTRYMODESET;
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  iic_ptr->iic_write(data, 2);

  uint8_t strData[2];
  strData[0] = 0x40;
  strData[1] = 'A';
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  iic_ptr->iic_write(strData, 2);
  iic_ptr->iic_write(strData, 2);

  iic_ptr->iic_close();
  iic_ptr->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
  iic_ptr->iic_control(IIC_CMD_MST_SET_TAR_ADDR, 0x62);
  iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void *)(IIC_MODE_STOP));

  // data[0] = 0x40;
  // data[1] = 'A';
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  // iic_ptr->iic_write(data, 2);
  // arc_delay_us(2000);

  uint8_t regData[2] = {0, 0};
  // iic_ptr->iic_write(regData, 1);
  // iic_ptr->iic_write((regData + 1), 1);
  iic_ptr->iic_write(regData, 2);
  arc_delay_us(2000);

  regData[0] = 0x08;
  regData[1] = 0x55;
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  // iic_ptr->iic_write(regData, 1);
  // iic_ptr->iic_write((regData + 1), 1);
  iic_ptr->iic_write(regData, 2);
  arc_delay_us(2000);

  regData[0] = 0x01;
  regData[1] = 0x20;
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  // iic_ptr->iic_write(regData, 1);
  // iic_ptr->iic_write((regData + 1), 1);
  iic_ptr->iic_write(regData, 2);
  arc_delay_us(2000);

  regData[0] = 0x04;
  regData[1] = 0xff;
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  // iic_ptr->iic_write(regData, 1);
  // iic_ptr->iic_write((regData + 1), 1);
  iic_ptr->iic_write(regData, 2);
  arc_delay_us(2000);

  regData[0] = 0x03;
  regData[1] = 0x00;
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  // iic_ptr->iic_write(regData, 1);
  // iic_ptr->iic_write((regData + 1), 1);
  iic_ptr->iic_write(regData, 2);
  arc_delay_us(2000);

  regData[0] = 0x02;
  regData[1] = 0x00;
  // iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
  // iic_ptr->iic_write(regData, 1);
  // iic_ptr->iic_write((regData + 1), 1);
  iic_ptr->iic_write(regData, 2);
  arc_delay_us(2000);

  board_delay_ms(10000, 0);
  iic_ptr->iic_close();
  iic_ptr->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
  iic_ptr->iic_control(IIC_CMD_MST_SET_TAR_ADDR, 0x3e);
  iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void *)(IIC_MODE_STOP));

  data[0] = 0x80;
  data[1] &= ~LCD_DISPLAYON;
  data[1] |= LCD_DISPLAYCONTROL;
  iic_ptr->iic_write(data, 2);

  board_delay_ms(1000, 0);
  data[1] |= LCD_DISPLAYON | LCD_BLINKON;
  data[1] &= ~LCD_CURSORON;
  data[1] |= LCD_DISPLAYCONTROL;
  iic_ptr->iic_write(data, 2);
  while (1)
  {
  }

  return E_SYS;
}
