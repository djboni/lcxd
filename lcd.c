/*
 Copyright 2016 Djones A. Boni

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "lcd.h"
#include <stdint.h>

#define DELAY_1U64 1640U
#define DELAY_0U40 40U
#define DELAY_WAIT 0U
#define DELAY_INIT 0U

enum LcdPins_t {
    displayD4 = 1U,
    displayD5 = 2U,
    displayD6 = 4U,
    displayD7 = 8U,

    displayE  = 16U,
    displayRS = 32U
};

enum LcdCmd_t {
    LCD_CLEAR = 0x01U,

    LCD_HOME = 0x02U,

    LCD_DISPLAY_OFF = 0x08U,
    LCD_DISPLAY_ON = 0x0CU,
    LCD_CURSOR_ON = 0x0EU,
    LCD_BLINK_ON = 0x0FU,
    LCD_CURSOR_OFF = LCD_DISPLAY_ON,
    LCD_BLINK_OFF = LCD_CURSOR_ON,

    LCD_LINE0 = 0x80U,
    LCD_LINE1 = 0xC0U,
    LCD_LINE2 = 0x94U,
    LCD_LINE3 = 0xD4U,

    LCD_OTHER
};

static void Lcd_pinWrite(Lcd *lcd, uint8_t data)
{
    /* D4-D7 */
    lcd->pinWrite(0U, (data & displayD4) != 0U);
    lcd->pinWrite(1U, (data & displayD5) != 0U);
    lcd->pinWrite(2U, (data & displayD6) != 0U);
    lcd->pinWrite(3U, (data & displayD7) != 0U);
}

static void Lcd_setRs(Lcd *lcd, uint8_t value)
{
    lcd->pinWrite(5U, value);
}

static void Lcd_setEn(Lcd *lcd, uint8_t value)
{
    lcd->pinWrite(4U, value);
}

static void Lcd_clockEn(Lcd *lcd)
{
    Lcd_setEn(lcd, 0U);
    Lcd_setEn(lcd, 1U);
}

static void Lcd_command4bit(Lcd *lcd, uint8_t cmd)
{
    lcd->delayUs(DELAY_WAIT);

    Lcd_setRs(lcd, 0U);

    Lcd_pinWrite(lcd, cmd & 0x0FU);
    Lcd_clockEn(lcd);
}

void Lcd_command(Lcd *lcd, uint8_t cmd)
{
    lcd->delayUs(DELAY_WAIT);

    Lcd_setRs(lcd, 0U);

    Lcd_pinWrite(lcd, (cmd >> 4U) & 0x0FU);
    Lcd_clockEn(lcd);

    Lcd_pinWrite(lcd, cmd & 0x0FU);
    Lcd_clockEn(lcd);
}

void Lcd_data(Lcd *lcd, uint8_t dat)
{
    lcd->delayUs(DELAY_WAIT);

    Lcd_setRs(lcd, 1U);

    Lcd_pinWrite(lcd, (dat >> 4U) & 0x0FU);
    Lcd_clockEn(lcd);

    Lcd_pinWrite(lcd, dat & 0x0FU);
    Lcd_clockEn(lcd);
}

void Lcd_clear(Lcd *lcd)
{
    Lcd_command(lcd, LCD_CLEAR);
    lcd->delayUs(DELAY_1U64);
}

void Lcd_home(Lcd *lcd)
{
    Lcd_command(lcd, LCD_HOME);
    lcd->delayUs(DELAY_1U64);
}

void Lcd_display(Lcd *lcd)
{
    Lcd_command(lcd, LCD_DISPLAY_ON);
    lcd->delayUs(DELAY_0U40);
}

void Lcd_noDisplay(Lcd *lcd)
{
    Lcd_command(lcd, LCD_DISPLAY_OFF);
    lcd->delayUs(DELAY_0U40);
}

void Lcd_cursor(Lcd *lcd)
{
    Lcd_command(lcd, LCD_CURSOR_ON);
    lcd->delayUs(DELAY_0U40);
}

void Lcd_noCursor(Lcd *lcd)
{
    Lcd_command(lcd, LCD_CURSOR_OFF);
    lcd->delayUs(DELAY_0U40);
}

void Lcd_blink(Lcd *lcd)
{
    Lcd_command(lcd, LCD_BLINK_ON);
    lcd->delayUs(DELAY_0U40);
}

void Lcd_noBlink(Lcd *lcd)
{
    Lcd_command(lcd, LCD_BLINK_OFF);
    lcd->delayUs(DELAY_0U40);
}

void Lcd_setCursor(Lcd *lcd, uint8_t line, uint8_t column)
{
    uint8_t addr = column;
    switch(line)
    {
        case 0U:
            addr += LCD_LINE0;
            break;
        case 1U:
            addr += LCD_LINE1;
            break;
        case 2U:
            addr += LCD_LINE2;
            break;
        case 3U:
            addr += LCD_LINE3;
            break;
        default:
            addr += LCD_LINE0;
    }
    Lcd_command(lcd, addr);
    lcd->delayUs(DELAY_0U40);
}

void Lcd_init(Lcd *lcd,
        void (*funcPinWrite)(uint8_t pin, uint8_t value),
        void (*funcDelayUs)(uint32_t delay_us))
{
    lcd->pinWrite = funcPinWrite;
    lcd->delayUs = funcDelayUs;

    /* Reset sequence */
    Lcd_setRs(lcd, 1U);
    Lcd_pinWrite(lcd, 0x0FU);
    Lcd_setEn(lcd, 1U);

    lcd->delayUs(DELAY_INIT + 15000U);

    Lcd_command4bit(lcd, 0x03U);
    lcd->delayUs(DELAY_INIT + 4100U);

    Lcd_command4bit(lcd, 0x03U);
    lcd->delayUs(DELAY_INIT + 100U);

    Lcd_command4bit(lcd, 0x03U);
    lcd->delayUs(DELAY_INIT + 4100U);

    Lcd_command4bit(lcd, 0x02U);
    lcd->delayUs(DELAY_0U40);

    Lcd_command(lcd, 0x28U); /* Config 4 bits, 2 lines, 5x7 font. */

    Lcd_noDisplay(lcd);
    Lcd_clear(lcd);
    Lcd_display(lcd);
}

void Lcd_writeByte(Lcd *lcd, uint8_t ch)
{
    Lcd_data(lcd, ch);
    lcd->delayUs(DELAY_0U40);
}

void Lcd_write(Lcd *lcd, const void *str)
{
    const uint8_t *s = (const uint8_t *)str;
    while(*s != 0U)
    {
        Lcd_writeByte(lcd, *s++);
    }
}

void Lcd_writeBuff(Lcd *lcd, const void *buff, uint16_t length)
{
    const uint8_t *b = (const uint8_t *)buff;
    while(length-- != 0)
    {
        Lcd_writeByte(lcd, *b++);
    }
}
