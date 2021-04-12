#include "timer.h"
#include "x86.h"
/* *
 * Support for time-related hardware gadgets - the 8253 timer,
 * which generates interruptes on IRQ-0.
 * */

#define TIMER_IO_PORT   0x40                    // 8253计数器0端口 0x40
#define TIMER_MODE_PORT 0x43                    // 8253控制字寄存器端口 0x43

/* *
 * Frequency of all three count-down timers; (TIMER_FREQ/freq)
 * is the appropriate count to generate a frequency of freq Hz.
 * */

#define TIMER_FREQ      1193182
#define TIMER_DIV(x)    ((TIMER_FREQ + (x) / 2) / (x))

#define TIMER_SEL0      0x00                    // select counter 0
#define TIMER_RATEGEN   0x04                    // mode 2, rate generator
#define TIMER_16BIT     0x30                    // r/w counter 16 bits, LSB first

volatile size_t ticks;

size_t SYSTEM_READ_TIMER( void ){
    return ticks;
}

/* *
 * clock_init - initialize 8253 clock to interrupt 100 times per second,
 * and then enable IRQ_TIMER.
 * */
void timer_init(void) {
    // set 8253 timer-chip
    // 选择定时器0,读写方式为先低后高,工作方式为方式2 比率发生器, 数制格式为二进制
    outb(TIMER_MODE_PORT, TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);
    
    // 设定每秒中断100次
    // 先写低8位
    outb(TIMER_IO_PORT, TIMER_DIV(100) % 256);
    // 再写高8位
    outb(TIMER_IO_PORT, TIMER_DIV(100) / 256);

    // initialize time counter 'ticks' to zero
    ticks = 0;

}

