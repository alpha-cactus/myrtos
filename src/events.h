#ifndef _EVENTS_H_
#define _EVENTS_H_

#include "common.h"

// event type enum used for indexing
typedef enum TeEVT_EventType {
	EVENT_1MS = 0,
	EVENT_3MS = 1,
	EVENT_6MS = 2,
	EVENT_12MS = 3,
	NUM_EVENTS = 4
} TeEVT_EventType;

// event state structure
typedef struct TsEVT_EventState {
	pthread_t tid; 		// pthread unique id for the event
	void* event_fun; 	// event function called when thread starts
} TsEVT_EventState;

extern void* PrfrmEVT_p_1ms(void* vargp);
extern void* PrfrmEVT_p_3ms(void* vargp);
extern void* PrfrmEVT_p_6ms(void* vargp);
extern void* PrfrmEVT_p_12ms(void* vargp);

extern pthread_t GetEVT_p_ThreadId(TeEVT_EventType event_type);
extern void SetEVT_ThreadId(TeEVT_EventType event_type, pthread_t thread_id);

extern void* GetEVT_p_EventFunction(TeEVT_EventType event_type);

#endif /*  _EVENTS_H_ */
