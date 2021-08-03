#ifndef _TIMER_H_
#define _TIMER_H_

#include "common.h"

#define TIMER_COUNT_12 	12u
#define TIMER_COUNT_6 	6u
#define TIMER_COUNT_3 	3u
#define TIMER_COUNT_1 	1u
#define TIMER_COUNT_0 	0u

typedef struct TsTIM_PrdTimerState {
	uint32_t msec;
	uint32_t count;
	pthread_t tid;
} TsTIM_PrdTimerState;

extern void RunTIM_StartTimer(void);
extern uint32_t PrfrmTIM_IncrTimer(void);
extern uint32_t GetTIM_TimerCount(void);
extern void PrfrmTIM_StopTimer(void);

#endif /* _TIMER_H_ */
