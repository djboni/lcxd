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

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void Lcd_pinWrite(uint8_t pin, uint8_t value);
void Lcd_delayUs(uint32_t delay_us);

void Lcd_init(uint8_t lines, uint8_t columns);

void Lcd_writeByte(uint8_t data);
void Lcd_write(const void *str);
void Lcd_writeBuff(const void *buff, uint16_t length);

void Lcd_clear(void);
void Lcd_home(void);
void Lcd_setCursor(uint8_t line, uint8_t column);

void Lcd_display(void);
void Lcd_noDisplay(void);
void Lcd_cursor(void);
void Lcd_noCursor(void);
void Lcd_blink(void);
void Lcd_noBlink(void);

void Lcd_command(uint8_t cmd);
void Lcd_data(uint8_t dat);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LCD_H_ */
