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
#include <stdio.h>
#include <stdarg.h>

#define DELAY_1U64 1640U
#define DELAY_0U40 40U
#define DELAY_WAIT 0U
#define DELAY_INIT 0U

enum LcdCmd_t {
  LCD_CLEAR = 0x01U,

  LCD_HOME = 0x02U,

  LDC_ENTRY_MODE_SET = 0x04U,
  LCD_INCREMENT = 0x02U,
  LCD_SHIFT = 0x01U,

  LCD_DISPLAY_CONTROL = 0x08U,
  LCD_DISPLAY_CONTROL_MASK = 0x0FU,
  LCD_DISPLAY_ON = 0x04U,
  LCD_CURSOR_ON = 0x02U,
  LCD_BLINK_ON = 0x01U,

  LCD_CURSOR_DISPLAY_SHIFT = 0x10U,
  LCD_SHIFT_DISPLAY = 0x80U,
  LCD_SHIFT_RIGHT = 0x40U,

  LCD_FUNCTION_SET = 0x20U,
  LCD_8BIT_DATA = 0x10U,
  LCD_2LINES = 0x08U,
  LCD_FONT5x10 = 0x04U,

  LCD_CGRAM_ADDR = 0x40U,
  LCD_CGRAM_ADDR_MASK = 0x7FU,

  LCD_DDRAM_ADDR = 0x80U,
  LCD_DDRAM_ADDR_MASK = 0xFFU,
  LCD_LINE0 = LCD_DDRAM_ADDR,
  LCD_LINE1 = LCD_DDRAM_ADDR | 0x40U,

  /* Pins */
  LCD_RS = 0x10,
  LCD_E = 0x20
};

static uint8_t displayControl = 0U;
static uint8_t displayColumns = 0U;

static void Lcd_command4bit(uint8_t cmd) {
  Lcd_delayUs(DELAY_WAIT);

  Lcd_pinsWrite((cmd & 0x0F) | LCD_E);
  Lcd_pinsWrite((cmd & 0x0F));
}

void Lcd_command(uint8_t cmd) {
  Lcd_delayUs(DELAY_WAIT);

  Lcd_pinsWrite((cmd >> 4) | LCD_E);
  Lcd_pinsWrite((cmd >> 4));

  Lcd_pinsWrite((cmd & 0x0F) | LCD_E);
  Lcd_pinsWrite((cmd & 0x0F));
}

void Lcd_data(uint8_t dat) {
  Lcd_delayUs(DELAY_WAIT);

  Lcd_pinsWrite((dat >> 4) | LCD_RS | LCD_E);
  Lcd_pinsWrite((dat >> 4) | LCD_RS);

  Lcd_pinsWrite((dat & 0x0F) | LCD_RS | LCD_E);
  Lcd_pinsWrite((dat & 0x0F) | LCD_RS);
}

void Lcd_clear(void) {
  Lcd_command(LCD_CLEAR);
  Lcd_delayUs(DELAY_1U64);
}

void Lcd_home(void) {
  Lcd_command(LCD_HOME);
  Lcd_delayUs(DELAY_1U64);
}

void Lcd_display(void) {
  displayControl |= (LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
  Lcd_command(displayControl & LCD_DISPLAY_CONTROL_MASK);
  Lcd_delayUs(DELAY_0U40);
}

void Lcd_noDisplay(void) {
  displayControl &= ~LCD_DISPLAY_ON;
  Lcd_command(displayControl & LCD_DISPLAY_CONTROL_MASK);
  Lcd_delayUs(DELAY_0U40);
}

void Lcd_cursor(void) {
  displayControl |= (LCD_DISPLAY_CONTROL | LCD_CURSOR_ON);
  Lcd_command(displayControl & LCD_DISPLAY_CONTROL_MASK);
  Lcd_delayUs(DELAY_0U40);
}

void Lcd_noCursor(void) {
  displayControl &= ~LCD_CURSOR_ON;
  Lcd_command(displayControl & LCD_DISPLAY_CONTROL_MASK);
  Lcd_delayUs(DELAY_0U40);
}

void Lcd_blink(void) {
  displayControl |= (LCD_DISPLAY_CONTROL | LCD_BLINK_ON);
  Lcd_command(displayControl & LCD_DISPLAY_CONTROL_MASK);
  Lcd_delayUs(DELAY_0U40);
}

void Lcd_noBlink(void) {
  displayControl &= ~LCD_BLINK_ON;
  Lcd_command(displayControl & LCD_DISPLAY_CONTROL_MASK);
  Lcd_delayUs(DELAY_0U40);
}

void Lcd_setCursor(uint8_t line, uint8_t column) {
  uint8_t addr = column;
  switch(line) {
  default:
  case 0U:
    addr |= LCD_LINE0;
    break;
  case 1U:
    addr |= LCD_LINE1;
    break;
  case 2U:
    addr += displayColumns;
    addr |= LCD_LINE0;
    break;
  case 3U:
    addr += displayColumns;
    addr |= LCD_LINE1;
    break;
  }
  Lcd_command(addr & LCD_DDRAM_ADDR_MASK);
  Lcd_delayUs(DELAY_0U40);
}

void Lcd_init(uint8_t lines, uint8_t columns) {
  (void) lines;

  displayControl = 0;
  displayColumns = columns;

  /* Reset sequence */
  Lcd_pinsWrite(0x0F | LCD_RS | LCD_E);

  Lcd_delayUs(DELAY_INIT + 40000U);

  Lcd_command4bit((LCD_FUNCTION_SET | LCD_8BIT_DATA) >> 4);
  Lcd_delayUs(DELAY_INIT + 4100U);

  Lcd_command4bit((LCD_FUNCTION_SET | LCD_8BIT_DATA) >> 4);
  Lcd_delayUs(DELAY_INIT + 100U);

  Lcd_command4bit((LCD_FUNCTION_SET | LCD_8BIT_DATA) >> 4);
  Lcd_delayUs(DELAY_0U40);

  Lcd_command4bit((LCD_FUNCTION_SET) >> 4);
  Lcd_delayUs(DELAY_0U40);

  Lcd_command(LCD_FUNCTION_SET | LCD_2LINES);
  Lcd_delayUs(DELAY_0U40);

  Lcd_noDisplay();
  Lcd_clear();
  Lcd_command(LDC_ENTRY_MODE_SET | LCD_INCREMENT);
  Lcd_delayUs(DELAY_0U40);

  Lcd_display();
}

void Lcd_writeByte(uint8_t ch) {
  Lcd_data(ch);
  Lcd_delayUs(DELAY_0U40);
}

void Lcd_write(const void *str) {
  const uint8_t *s = (const uint8_t *) str;
  while(*s != 0U) {
    Lcd_writeByte(*s++);
  }
}

void Lcd_writeBuff(const void *buff, uint16_t length) {
  const uint8_t *b = (const uint8_t *) buff;
  while(length-- != 0) {
    Lcd_writeByte(*b++);
  }
}

int Lcd_print(const char *format, ...) {
  int used_length;
  char buf[LCXD_PRINT_BUFSZ];
  {
    va_list vl;
    va_start(vl, format);
    used_length = vsnprintf(buf, sizeof(buf), format, vl);
    va_end(vl);
  }
  Lcd_write(buf);
  return used_length;
}
