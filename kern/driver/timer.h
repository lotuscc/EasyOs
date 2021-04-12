#ifndef __TIMER_H__
#define __TIMER_H__

#include <defs.h>

extern volatile size_t ticks;

void timer_init(void);

size_t SYSTEM_READ_TIMER( void );


#endif /* !__TIMER_H__ */

