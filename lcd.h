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
     (*pinWrite)(uint8_t pin, uint8_t value);
     (*delayUs)(uint32_t delay_us);
} Lcd;

 Lcd_init(Lcd *lcd,
         (*funcPinWrite)(uint8_t pin, uint8_t value),
         (*funcDelayUs)(uint32_t delay_us));

 Lcd_writeByte(Lcd *lcd, uint8_t data);
 Lcd_write(Lcd *lcd, const  *str);
 Lcd_writeBuff(Lcd *lcd, const  *buff, uint16_t length);

 Lcd_clear(Lcd *lcd);
 Lcd_home(Lcd *lcd);
 Lcd_setCursor(Lcd *lcd, uint8_t line, uint8_t column);

 Lcd_display(Lcd *lcd);
 Lcd_noDisplay(Lcd *lcd);
 Lcd_cursor(Lcd *lcd);
 Lcd_noCursor(Lcd *lcd);
 Lcd_blink(Lcd *lcd);
 Lcd_noBlink(Lcd *lcd);

 Lcd_command(Lcd *lcd, uint8_t cmd);
 Lcd_data(Lcd *lcd, uint8_t dat);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LCD_H_ */
