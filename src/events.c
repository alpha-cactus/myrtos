
#include "events.h"
#include "timer.h"
#include "semaphore.h"

// event states for tracking event unique values
static TsEVT_EventState event_states[NUM_EVENTS] = {
	[EVENT_1MS] = {.event_fun = PrfrmEVT_p_1ms},
	[EVENT_3MS] = {.event_fun = PrfrmEVT_p_3ms},
	[EVENT_6MS] = {.event_fun = PrfrmEVT_p_6ms},
	[EVENT_12MS] = {.event_fun = PrfrmEVT_p_12ms}
};

// store thread id into event state
void SetEVT_ThreadId(TeEVT_EventType event_type, pthread_t thread_id)
{
	event_states[event_type].tid = thread_id;
}

// get thread unique id from event state
pthread_t GetEVT_p_ThreadId(TeEVT_EventType event_type)
{
	return event_states[event_type].tid;
}

// get event function pointer
void* GetEVT_p_EventFunction(TeEVT_EventType event_type)
{
	return event_states[event_type].event_fun;
}

// event/task function
void* PrfrmEVT_p_1ms(void* vargp)
{
	printf("event 1ms\n");

	// call pthread exit at event end
	pthread_exit(NULL);
}

void* PrfrmEVT_p_3ms(void* vargp)
{
	printf("event 3ms\n");
	pthread_exit(NULL);
}

void* PrfrmEVT_p_6ms(void* vargp)
{
	printf("event 6ms\n");
	pthread_exit(NULL);
}

void* PrfrmEVT_p_12ms(void* vargp)
{
	// suspend interrupts during event execution
	PrfrmOS_SuspendInterrupts();
	sleep(3);
	printf("event 12ms\n");
	// resume interrupts before exit
	PrfrmOS_ResumeInterrupts();
	pthread_exit(NULL);
}

