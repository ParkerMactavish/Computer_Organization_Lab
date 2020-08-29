#include "iic1602lcd.h"

#include <stdint.h>

#include "dev_iic.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

/* Internal Setup Function */
static void set_CursorPos_(uint8_t Col, uint8_t Row);
static void print_(const char *Str, uint8_t Len);
static void write(const char Chr);
static void clear_();
static void home_();
/* End Internal Setup Function */

/* Internal Toggle Function */
static void set_Display_(ON_OFF_t Value);
static void set_Blink_(ON_OFF_t Value);
static void set_Cursor_(ON_OFF_t Value);
static void set_ScrollDir_(L_R_t Value);
static void set_CharStarting_(L_R_t Value);
static void set_AutoScroll_(ON_OFF_t Value);
/* End Internal Toggle Function */

/* Internal RGB Function */
static void blink_LED_(ON_OFF_t Value);
static void set_Color_(COLOR_t Color);
static void set_RGBs_(uint8_t R, uint8_t G, uint8_t B);
static void set_RGB_(COLOR_t Color, uint8_t Value);
/* End Internal RGB Function */

/* Utility Functions */
static void i2c_sendBytes(uint8_t *pData, uint8_t Num);
static void command(uint8_t Value);
static void i2c_setRGBReg(uint8_t Addr, uint8_t Data);
/* End Utility Functions */

static int32_t iic1602_initialized = 0;
/* LCD Private Variable */
DEV_IIC_PTR iic_ptr = NULL;
static LCD_t lcd;
static uint8_t ColorComponent[4][3] = {
    {0xff, 0xff, 0xff},
    {0xff, 0x00, 0x00},
    {0x00, 0xff, 0x00},
    {0x00, 0x00, 0xff}};
static uint8_t displayFunction_, displayControl_, displayMode_;
/* End LCD Private Variable */

pLCD_t LCD_Init(int32_t iic_id)
{
    iic_ptr = iic_get_dev(iic_id);
    // if (iic_ptr == NULL)
    // {
    //     return NULL;
    // }
    displayFunction_ = LCD_2LINE;

    arc_delay_us(50000); // Waiting for booting up

    command(LCD_FUNCTIONSET | displayFunction_);
    arc_delay_us(4500); // Wait more than 4.1ms

    displayControl_ = 0;
    set_Display_(ON);

    // clear_();

    // displayMode_ = LCD_ENTRYLEFT & LCD_ENTRYDECREMENT_;

    // command(LCD_ENTRYMODESET | displayMode_);

    // i2c_setRGBReg(REG_MODE1, 0);
    // i2c_setRGBReg(REG_OUTPUT, 0xFF);
    // i2c_setRGBReg(REG_MODE2, 0x20);

    // set_Color_(WHITE);

    /* Assigning Public Functions*/
    lcd.set_CursorPos = set_CursorPos_;
    lcd.print = print_;
    lcd.clear = clear_;
    lcd.home = home_;

    lcd.set_Display = set_Display_;
    lcd.set_Blink = set_Blink_;
    lcd.set_Cursor = set_Cursor_;
    lcd.set_ScrollDir = set_ScrollDir_;
    lcd.set_CharStarting = set_CharStarting_;
    lcd.set_AutoScroll = set_AutoScroll_;

    lcd.blink_LED = blink_LED_;
    lcd.set_Color = set_Color_;
    lcd.set_RGBs = set_RGBs_;
    lcd.set_RGB = set_RGB_;
    /* End Assigning Public Functions*/

    return &lcd;
}

static void set_CursorPos_(uint8_t Col, uint8_t Row)
{
    uint8_t _col = (Row == 0) ? (Col | 0x80) : (Col | 0xC0);
    command(_col);
}

static void print_(const char *Str, uint8_t Len)
{
    for (int i = 0; i < Len; i++)
    {
        write(Str[i]);
    }
}

static void write(const char Chr)
{
    uint8_t Data[2] = {0x40, Chr};
    i2c_sendBytes(Data, 2);
}

static void clear_()
{
    command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
    arc_delay_us(2000);        // extra waiting time
}

static void home_()
{
    command(LCD_RETURNHOME); // set curso position to zero
    arc_delay_us(2000);      // extra waiting time
}

static void set_Display_(ON_OFF_t Value)
{
    if (Value == ON)
    {
        displayControl_ |= LCD_DISPLAYON;
    }
    else
    {
        displayControl_ &= LCD_DISPLAYOFF_;
    }
    command(LCD_DISPLAYCONTROL | displayControl_);
}

static void set_Cursor_(ON_OFF_t Value)
{
    if (Value == ON)
    {
        displayControl_ |= LCD_CURSORON;
    }
    else
    {
        displayControl_ &= LCD_CURSOROFF_;
    }
    command(LCD_DISPLAYCONTROL | displayControl_);
}

static void set_Blink_(ON_OFF_t Value)
{
    if (Value == ON)
    {
        displayControl_ |= LCD_CURSORON;
    }
    else
    {
        displayControl_ &= LCD_CURSOROFF_;
    }
    command(LCD_DISPLAYCONTROL | displayControl_);
}

static void set_ScrollDir_(L_R_t Value)
{
    if (Value == L)
    {
        command((LCD_CURSORSHIFT | LCD_DISPLAYMOVE) & LCD_MOVELEFT_);
    }
    else
    {
        command((LCD_CURSORSHIFT | LCD_DISPLAYMOVE) | LCD_MOVERIGHT);
    }
}

static void set_CharStarting_(L_R_t Value)
{
    if (Value == L)
    {
        displayMode_ |= LCD_ENTRYLEFT;
    }
    else
    {
        displayMode_ &= LCD_ENTRYRIGHT_;
    }
    command(LCD_ENTRYMODESET | displayMode_);
}

static void set_AutoScroll_(ON_OFF_t Value)
{
    if (Value == ON)
    {
        displayMode_ |= LCD_ENTRYINCREMENT;
    }
    else
    {
        displayMode_ &= LCD_ENTRYDECREMENT_;
    }
    command(LCD_ENTRYMODESET | displayMode_);
}

static void blink_LED_(ON_OFF_t Value)
{
    if (Value == ON)
    {
        i2c_setRGBReg(0x07, 0x17);
        i2c_setRGBReg(0x06, 0x7F);
    }
    else
    {
        i2c_setRGBReg(0x07, 0x00);
        i2c_setRGBReg(0x06, 0xff);
    }
}

static void set_Color_(COLOR_t Color)
{
    set_RGBs_(ColorComponent[Color][0],
              ColorComponent[Color][1],
              ColorComponent[Color][2]);
}

static void set_RGBs_(uint8_t R, uint8_t G, uint8_t B)
{
    set_RGB_(RED, R);
    set_RGB_(GREEN, G);
    set_RGB_(BLUE, B);
}

static void set_RGB_(COLOR_t Color, uint8_t Value)
{
    switch (Color)
    {
    case RED:
        i2c_setRGBReg(REG_RED, Value);
        break;
    case GREEN:
        i2c_setRGBReg(REG_GREEN, Value);
        break;
    case BLUE:
        i2c_setRGBReg(REG_BLUE, Value);
        break;
    }
}

static void i2c_sendBytes(uint8_t *pData, uint8_t Num)
{
    iic_ptr->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
    iic_ptr->iic_control(IIC_CMD_MST_SET_TAR_ADDR, LCD_ADDRESS);
    iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void *)(IIC_MODE_STOP));
    iic_ptr->iic_write(pData, Num);
    arc_delay_us(200);
    iic_ptr->iic_close();
    for (int i = 0; i < Num; i++)
    {
        EMBARC_PRINTF("%X\n", pData[i]);
    }
}

static void command(uint8_t Value)
{
    uint8_t data[2] = {LCD_SETDDRAMADDR, Value};
    i2c_sendBytes(data, 2);
}

static void i2c_setRGBReg(uint8_t Addr, uint8_t Data)
{
    iic_ptr->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
    iic_ptr->iic_control(IIC_CMD_MST_SET_TAR_ADDR, RGB_ADDRESS);
    iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void *)(IIC_MODE_STOP));
    uint8_t regData[2] = {Addr, Data};
    iic_ptr->iic_write(regData, 2);
    iic_ptr->iic_close();
}