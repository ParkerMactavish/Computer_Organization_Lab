#include "iic1602lcd.h"

#include <stdint.h>

#include "dev_iic.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

static int32_t set_backlight_internel(BACKLIGHT_STATE_T state);
static int32_t set_cursor_internel(uint8_t col, uint8_t row);
static int32_t print_internel(const char* str, int32_t len);
static void setBacklight(uint8_t value);
static int32_t iic_write(uint8_t value);
static void write4bits(uint8_t value, uint8_t mode);
static void pulseEnable(uint8_t data);
static void display();
static void clear();
static void send(uint8_t value, uint8_t mode);
static void command(uint8_t value);

static IIC1602LCD_OBJ iic1602lcd_context;
static int32_t iic1602_initialized = 0;
static DEV_IIC_PTR iic_ptr = NULL;

static uint8_t _En, _Rw, _Rs;
static uint8_t _data_pins[4];
static uint8_t _polarity;
static uint8_t _backlightPinMask, _backlightStsMask;
static uint8_t _displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
static uint8_t _displaycontrol, _displaymode;

IIC1602LCD_OBJ_PTR
iic1602lcd_getobj(IIC1602LCD_CONFIG_T config) {
    int32_t ercd = E_OK;
    iic1602lcd_context.config = config;
    iic_ptr = iic_get_dev(IIC1602LCD_IIC_DEV_ID);
    CHECK_EXP_NOERCD(iic_ptr, error_exit);
    ercd = iic_ptr->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
    if (ercd != E_OK) goto error_exit;
    ercd = iic_ptr->iic_control(IIC_CMD_MST_SET_TAR_ADDR,
                                (void*)(iic1602lcd_context.config.iic_addr));
    if (ercd != E_OK) goto error_exit;

    _En = (1 << config.En);
    _Rw = (1 << config.Rw);
    _Rs = (1 << config.Rs);
    _data_pins[0] = (1 << config.d4);
    _data_pins[1] = (1 << config.d5);
    _data_pins[2] = (1 << config.d6);
    _data_pins[3] = (1 << config.d7);

    _backlightPinMask = 0;
    _backlightStsMask = LCD_NOBACKLIGHT;
    _backlightPinMask = (1 << config.backlighPin);
    _polarity = config.pol;
    iic_write(LCD_FUNCTIONSET | LCD_2LINE);
    arc_delay_us(4500);

    iic_write(LCD_FUNCTIONSET | LCD_2LINE);
    
    arc_delay_us(150);

    iic_write(LCD_FUNCTIONSET | LCD_2LINE);
    
    setBacklight(BACKLIGHT_STATE_OFF);

    iic_write(0);

    board_delay_ms(100, 1);

    send(0x03, FOUR_BITS);
    arc_delay_us(4500);
    send(0x03, FOUR_BITS);
    arc_delay_us(150);
    send(0x03, FOUR_BITS);
    arc_delay_us(150);
    send(0x02, FOUR_BITS);
    arc_delay_us(150);

    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);
    arc_delay_us(60);  // wait more

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear the LCD
    clear();

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);

    set_backlight_internel(BACKLIGHT_STATE_ON);

    iic1602_initialized = 1;
    iic1602lcd_context.set_backlight = set_backlight_internel;
    iic1602lcd_context.set_cursor = set_cursor_internel;
    iic1602lcd_context.print = print_internel;
    iic1602lcd_context.clear = clear;
    return &iic1602lcd_context;

error_exit:
    return NULL;
}

static int32_t set_backlight_internel(BACKLIGHT_STATE_T state) {
    if (state == BACKLIGHT_STATE_ON) {
        setBacklight(255);
    } else {
        setBacklight(0);
    }
    return 0;
}

static int32_t set_cursor_internel(uint8_t col, uint8_t row) {
    const uint8_t row_offsetsDef[] = {0x00, 0x40, 0x14, 0x54};

    if (row >= 2) row = 1;

    command(LCD_SETDDRAMADDR | (col + row_offsetsDef[row]));
    return 0;
}

static int32_t print_internel(const char* str, int32_t len) {
    for (int i = 0; i < len; i++) {
        send(str[i], LCD_DATA);
    }

    return 0;
}

static void setBacklight(uint8_t value) {
    if (_backlightPinMask != 0x0) {
        if (((_polarity == BACKLIGHT_POL_POSITIVE) && (value > 0)) ||
            ((_polarity == BACKLIGHT_POL_NEGATIVE) && (value == 0))) {
            _backlightStsMask = _backlightPinMask & LCD_BACKLIGHT;
        } else {
            _backlightStsMask = _backlightPinMask & LCD_NOBACKLIGHT;
        }
        iic_write(_backlightStsMask);
    }
}

static int32_t iic_write(uint8_t value) {
    // if (!iic1602_initialized) return -1;

    iic_ptr->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
    iic_ptr->iic_write(&value, 1);

    return 0;
}

static void write4bits(uint8_t value, uint8_t mode) {
    uint8_t pinMapValue = 0;

    for (uint8_t i = 0; i < 4; i++) {
        if ((value & 0x1) == 1) {
            pinMapValue |= _data_pins[i];
        }
        value = (value >> 1);
    }

    if (mode == LCD_DATA) {
        mode = _Rs;
    }

    pinMapValue |= mode | _backlightStsMask;
    pulseEnable(pinMapValue);
}

static void pulseEnable(uint8_t data) {
    iic_write(data | _En);   // En HIGH
    iic_write(data & ~_En);  // En LOW
}

static void display() {
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

static void clear() {
    command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    arc_delay_us(HOME_CLEAR_EXEC);  // this command is time consuming
}

static void send(uint8_t value, uint8_t mode) {
    if (mode == FOUR_BITS) {
        write4bits((value & 0x0F), COMMAND);
    } else {
        write4bits((value >> 4), mode);
        write4bits((value & 0x0F), mode);
    }
}

static void command(uint8_t value) { send(value, COMMAND); }