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

    LCD_DISPLAY_CONTROL = 0x08U,
    LCD_DISPLAY_ON = 0x04U,
    LCD_CURSOR_ON = 0x02U,
    LCD_BLINK_ON = 0x01U,

    LCD_LINE0 = 0x80U,
    LCD_LINE1 = 0xC0U,

    LCD_OTHER
};

static uint8_t displayControl = 0U;
static uint8_t displayColumns = 0U;

static void Lcd_local_pinWrite(uint8_t data)
{
    /* D4-D7 */
    Lcd_pinWrite(0U, (data & displayD4) != 0U);
    Lcd_pinWrite(1U, (data & displayD5) != 0U);
    Lcd_pinWrite(2U, (data & displayD6) != 0U);
    Lcd_pinWrite(3U, (data & displayD7) != 0U);
}

static void Lcd_setRs(uint8_t value)
{
    Lcd_pinWrite(5U, value);
}

static void Lcd_setEn(uint8_t value)
{
    Lcd_pinWrite(4U, value);
}

static void Lcd_clockEn(void)
{
    Lcd_setEn(0U);
    Lcd_setEn(1U);
}

static void Lcd_command4bit(uint8_t cmd)
{
    Lcd_delayUs(DELAY_WAIT);

    Lcd_setRs(0U);

    Lcd_local_pinWrite(cmd & 0x0FU);
    Lcd_clockEn();
}

void Lcd_command(uint8_t cmd)
{
    Lcd_delayUs(DELAY_WAIT);

    Lcd_setRs(0U);

    Lcd_local_pinWrite((cmd >> 4U) & 0x0FU);
    Lcd_clockEn();

    Lcd_local_pinWrite(cmd & 0x0FU);
    Lcd_clockEn();
}

void Lcd_data(uint8_t dat)
{
    Lcd_delayUs(DELAY_WAIT);

    Lcd_setRs(1U);

    Lcd_local_pinWrite((dat >> 4U) & 0x0FU);
    Lcd_clockEn();

    Lcd_local_pinWrite(dat & 0x0FU);
    Lcd_clockEn();
}

void Lcd_clear(void)
{
    Lcd_command(LCD_CLEAR);
    Lcd_delayUs(DELAY_1U64);
}

void Lcd_home(void)
{
    Lcd_command(LCD_HOME);
    Lcd_delayUs(DELAY_1U64);
}

void Lcd_display(void)
{
    displayControl |= LCD_DISPLAY_ON;
    Lcd_command(displayControl);
    Lcd_delayUs(DELAY_0U40);
}

void Lcd_noDisplay(void)
{
    displayControl &= ~LCD_DISPLAY_ON;
    Lcd_command(displayControl);
    Lcd_delayUs(DELAY_0U40);
}

void Lcd_cursor(void)
{
    displayControl |= LCD_CURSOR_ON;
    Lcd_command(displayControl);
    Lcd_delayUs(DELAY_0U40);
}

void Lcd_noCursor(void)
{
    displayControl &= ~LCD_CURSOR_ON;
    Lcd_command(displayControl);
    Lcd_delayUs(DELAY_0U40);
}

void Lcd_blink(void)
{
    displayControl |= LCD_BLINK_ON;
    Lcd_command(displayControl);
    Lcd_delayUs(DELAY_0U40);
}

void Lcd_noBlink(void)
{
    displayControl &= ~LCD_BLINK_ON;
    Lcd_command(displayControl);
    Lcd_delayUs(DELAY_0U40);
}

void Lcd_setCursor(uint8_t line, uint8_t column)
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
            addr += LCD_LINE0 + displayColumns;
            break;
        case 3U:
            addr += LCD_LINE1 + displayColumns;
            break;
        default:
            addr += LCD_LINE0;
    }
    Lcd_command(addr);
    Lcd_delayUs(DELAY_0U40);
}

void Lcd_init(uint8_t lines, uint8_t columns)
{
    displayControl = LCD_DISPLAY_CONTROL;
    displayColumns = columns;
    (void) lines;

    /* Reset sequence */
    Lcd_setRs(1U);
    Lcd_local_pinWrite(0x0FU);
    Lcd_setEn(1U);

    Lcd_delayUs(DELAY_INIT + 15000U);

    Lcd_command4bit(0x03U);
    Lcd_delayUs(DELAY_INIT + 4100U);

    Lcd_command4bit(0x03U);
    Lcd_delayUs(DELAY_INIT + 100U);

    Lcd_command4bit(0x03U);
    Lcd_delayUs(DELAY_INIT + 4100U);

    Lcd_command4bit(0x02U);
    Lcd_delayUs(DELAY_0U40);

    Lcd_command(0x28U); /* Config 4 bits, 2 lines, 5x7 font. */

    Lcd_noDisplay();
    Lcd_clear();
    Lcd_display();
}

void Lcd_writeByte(uint8_t ch)
{
    Lcd_data(ch);
    Lcd_delayUs(DELAY_0U40);
}

void Lcd_write(const void *str)
{
    const uint8_t *s = (const uint8_t *)str;
    while(*s != 0U)
    {
        Lcd_writeByte(*s++);
    }
}

void Lcd_writeBuff(const void *buff, uint16_t length)
{
    const uint8_t *b = (const uint8_t *)buff;
    while(length-- != 0)
    {
        Lcd_writeByte(*b++);
    }
}
