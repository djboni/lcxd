# LCxD - ExampleSPI


Example using 74HC595 shift register.

The example shows an implementation using
[Arduinutil](https://github.com/djboni/arduinutil) as development platform.


```c
#include "Arduinutil.h"
#include "lcd.h"

void Lcd_595write(const uint8_t *buff, uint8_t length)
{
    const uint8_t ss_pin = 10;
    uint8_t num;

    pinMode(ss_pin, OUTPUT);
    digitalWrite(ss_pin, LOW);

    Spi_write(buff, length);
    while(Spi_getStatus() != 0) {}

    digitalWrite(ss_pin, HIGH);
}

/* Converts LCxD pins to the shift register pins and write the values to LCD
pins when E/LCD_VCC is changed.

  Param  LCD  Convert  74HC595
    pin  pin    to     pin
      0  D4     ->     6 QG
      1  D5     ->     5 QF
      2  D6     ->     4 QE
      3  D7     ->     3 QD
      4  E      ->     2 QC
      5  RS     ->     1 QB
      6  RW     ->     - --  (not used in LCxD)
      7  LED    ->     7 QH  (not used in LCxD)
      8  LCD_VCC ->    0 QA  (not used in LCxD)
*/
void Lcd_pinWrite(uint8_t pin, uint8_t value)
{
    static uint8_t pins = 0xFF; /* LED=1, D4-7=1, E=1, RS=1, LCD_VCC=1 */
    uint8_t buff[2];

    switch(pin) {
        case 0: /* D4 */
            pin = 6;
            break;
        case 1: /* D5 */
            pin = 5;
            break;
        case 2: /* D6 */
            pin = 4;
            break;
        case 3: /* D7 */
            pin = 3;
            break;
        case 4: /* E */
            pin = 2;
            break;
        case 5: /* RS */
            pin = 1;
            break;
        case 7: /* LED */
            pin = 7;
            break;
        case 8: /* LCD_VCC */
            pin = 0;
            break;
        default:
            ASSERT(0); /* Invalid pin. */
    }

    if(value == 0)
    {
        /* Possible E/LCD_VCC falling edge. Data is read by the LCD in this
         edge. First send data with high E/LCD_VCC and then send data with low
         E/LCD_VCC. */
        buff[0] = pins;
        pins &= ~(1 << pin);
        buff[1] = pins;

        if(pin == 2 || pin == 8)
            Lcd_595write(buff, 2);
    }
    else
    {
        /* Possible E/LCD_VCC rising edge. Data is not read by the LCD in this
        edge. Just send the data with high E/LCD_VCC. */
        pins |= (1 << pin);
        buff[0] = pins;

        if(pin == 2 || pin == 8)
            Lcd_595write(buff, 1);
    }
}

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

    Spi_begin(PRESCALER);

    Lcd_init(2, 16); /* LCD 1602 */

    Lcd_blink();
    Lcd_write("LCxD");

    for(;;) {}
}
```
