# LCxD - ExampleIO


Example using direct IOs.

The example shows an implementation using
[Arduinutil](https://github.com/djboni/arduinutil) as development platform.


```c
#include "Arduinutil.h"
#include "lcd.h"

/* Converts LCxD pins to the Arduino pins and write value to LCD pins.

 Param  LCD   Convert  Arduino  ATmega328P  ATmega2560
 bit    pin     to     pin      
 0      DB4     ->     D4       PORTD4      PORTG5
 1      DB5     ->     D5       PORTD5      PORTE3
 2      DB6     ->     D6       PORTD6      PORTH3
 3      DB7     ->     D7       PORTD7      PORTH4
 4      RS      ->     D8       PORTB0      PORTH5
 5      E       ->     D9       PORTB1      PORTH6
 -      RW      ->     GND
 */
void Lcd_pinsWrite(uint8_t data_00ER7654) {
#if defined(__AVR_ATmega328P__)
  /* Faster way for ATmega328P (Arduino Uno, Nano, Pro Mini) */
  CRITICAL_VAL();
  CRITICAL_ENTER();
  PORTD = (PORTD & 0x0F) | (data_00ER7654 << 4);
  PORTB = (PORTB & 0xFC) | ((data_00ER7654 >> 4) & 0x03);
  CRITICAL_EXIT();
#elif defined(__AVR_ATmega2560__)
  /* Faster way for ATmega2560 (Arduino Mega 2560) */
  CRITICAL_VAL();
  CRITICAL_ENTER();
  PORTG = (PORTG & 0xDF) | ((data_00ER7654 << 5) & 0x20);
  PORTE = (PORTE & 0xF7) | ((data_00ER7654 << 2) & 0x08);
  PORTH = (PORTH & 0x87) | ((data_00ER7654 << 1) & 0x78);
  CRITICAL_EXIT();
#else
  digitalWrite(4, data_00ER7654 & 0x01);
  digitalWrite(5, data_00ER7654 & 0x02);
  digitalWrite(6, data_00ER7654 & 0x04);
  digitalWrite(7, data_00ER7654 & 0x08);
  digitalWrite(8, data_00ER7654 & 0x10);
  digitalWrite(9, data_00ER7654 & 0x20);
#endif
}

/* Delay if needed. */
void Lcd_delayUs(uint32_t delay_us)
{
    static uint32_t block_until = 0;

    uint32_t now = timerCounts();
    int32_t diff = block_until - now;
    uint32_t delay = TIMER_US_TO_COUNT(delay_us);

    if(diff > 0)
    {
        delayCounts(diff);
        now = timerCounts();
    }

    block_until = now + delay;
}

int main(void)
{
    init();
    timerBegin();

    Lcd_init(2, 16); /* LCD 1602 */

    Lcd_blink();
    Lcd_write("LCxD");

    for(;;) {}
}
```
