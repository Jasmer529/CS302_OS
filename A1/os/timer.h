#ifndef TIMER_H
#define TIMER_H

#include "types.h"

#define TICKS_PER_SEC (4)
// QEMU
#define CPU_FREQ (10000000)

uint64 get_cycle();
void timer_init();
void set_next_timer();
void sleepms(uint64 ms);

typedef struct {
    uint64 sec;   // 自 Unix 纪元起的秒数
    uint64 usec;  // 微秒数
} TimeVal;

#endif  // TIMER_H