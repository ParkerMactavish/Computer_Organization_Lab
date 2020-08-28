#ifndef IIC1602LCD_H
#define IIC1602LCD_H

#include <stdint.h>

#define IIC1602LCD_IIC_DEV_ID 0

/* RGB */
// color define
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08
/* End RGB */

#define LCD_NOBACKLIGHT 0x00
#define LCD_BACKLIGHT 0xFF

#define COMMAND 0
#define LCD_DATA 1
#define FOUR_BITS 2

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

#define HOME_CLEAR_EXEC 2000

#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

typedef enum BACKLIGHT_POL {
    BACKLIGHT_POL_POSITIVE,
    BACKLIGHT_POL_NEGATIVE
} BACKLIGHT_POL_T;

typedef enum BACKLIGHT_STATE {
    BACKLIGHT_STATE_OFF,
    BACKLIGHT_STATE_ON
} BACKLIGHT_STATE_T;

typedef struct iic1602lcd_config_s {
    uint8_t iic_addr;
    uint8_t En;
    uint8_t Rw;
    uint8_t Rs;
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
    uint8_t d7;
    uint8_t backlighPin;
    BACKLIGHT_POL_T pol;
} IIC1602LCD_CONFIG_T;

typedef struct iic1602lcd_obj_s {
    IIC1602LCD_CONFIG_T config;
    int32_t (*set_backlight)(BACKLIGHT_STATE_T state);
    int32_t (*set_cursor)(uint8_t col, uint8_t row);
    int32_t (*print)(const char* str, int32_t len);
    void (*clear)(void);
} IIC1602LCD_OBJ, *IIC1602LCD_OBJ_PTR;

IIC1602LCD_OBJ_PTR iic1602lcd_getobj(IIC1602LCD_CONFIG_T config);

#endif