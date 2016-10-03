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

typedef struct displayLcd_t {
    void (*pinWrite)(uint8_t pin, uint8_t value);
    void (*delayUs)(uint32_t delay_us);
    uint8_t displayControl;
} Lcd;

void Lcd_init(Lcd *lcd,
        void (*funcPinWrite)(uint8_t pin, uint8_t value),
        void (*funcDelayUs)(uint32_t delay_us));

void Lcd_writeByte(Lcd *lcd, uint8_t data);
void Lcd_write(Lcd *lcd, const void *str);
void Lcd_writeBuff(Lcd *lcd, const void *buff, uint16_t length);

void Lcd_clear(Lcd *lcd);
void Lcd_home(Lcd *lcd);
void Lcd_setCursor(Lcd *lcd, uint8_t line, uint8_t column);

void Lcd_display(Lcd *lcd);
void Lcd_noDisplay(Lcd *lcd);
void Lcd_cursor(Lcd *lcd);
void Lcd_noCursor(Lcd *lcd);
void Lcd_blink(Lcd *lcd);
void Lcd_noBlink(Lcd *lcd);

void Lcd_command(Lcd *lcd, uint8_t cmd);
void Lcd_data(Lcd *lcd, uint8_t dat);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LCD_H_ */
