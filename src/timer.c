#include "timer.h"
#include "events.h"

static void* PrfrmTIM_PeriodicFunction(void* vargp);
static void PrfrmTIM_ScheduleTasks(void);
static void PrfrmTIM_StartEvent(TeEVT_EventType event_type);

// periodic timer state
static TsTIM_PrdTimerState ptimer = {
	.msec = USLEEP_MSEC(1000),
	.count = 0
};

// periodic timer thread function
static void* PrfrmTIM_PeriodicFunction(void* vargp)
{
	// infinite loop
	while (1)
	{
		// check if interrupts are suspended 
		CheckOS_Suspend();

		// increment timer count
		PrfrmTIM_IncrTimer();
		printf("periodic timer count: %d\n", ptimer.count);
		// schedule tasks/events
		PrfrmTIM_ScheduleTasks();

		// sleep for period time
		usleep(ptimer.msec);
	}

	return NULL;
}

// timer count increment
// used to by task scheduler
uint32_t PrfrmTIM_IncrTimer(void)
{
	ptimer.count = (ptimer.count + 1) % 12;

	return ptimer.count;
}

// get function for timer count
uint32_t GetTIM_TimerCount(void)
{
	return ptimer.count;
}

// starts the periodic timer
void RunTIM_StartTimer(void)
{
	pthread_t prd_thread_id;

	// increment timer to ensure timer count is 0 when started
	PrfrmTIM_IncrTimer();
	while(PrfrmTIM_IncrTimer() != TIMER_COUNT_0);

	// create timer thread
	pthread_create(&prd_thread_id, NULL, PrfrmTIM_PeriodicFunction, NULL);
	// store thread id for stop timer function
	ptimer.tid = prd_thread_id;
	pthread_join(prd_thread_id, NULL);
}

// stops the periodic timer
void PrfrmTIM_StopTimer(void)
{
	pthread_cancel(ptimer.tid);
}

// schedules tasks/events using threads
static void PrfrmTIM_ScheduleTasks(void)
{
	uint32_t count;

	// check timer count
	count = GetTIM_TimerCount();

	// start events based on count
	if ((count % TIMER_COUNT_12) == 0x0) {
		PrfrmTIM_StartEvent(EVENT_12MS);
		PrfrmTIM_StartEvent(EVENT_6MS);
		PrfrmTIM_StartEvent(EVENT_3MS);
		PrfrmTIM_StartEvent(EVENT_1MS);
	}
	else if ((count % TIMER_COUNT_6) == 0x0) {
		PrfrmTIM_StartEvent(EVENT_6MS);
		PrfrmTIM_StartEvent(EVENT_3MS);
		PrfrmTIM_StartEvent(EVENT_1MS);
	}
	else if ((count % TIMER_COUNT_3) == 0x0) {
		PrfrmTIM_StartEvent(EVENT_3MS);
		PrfrmTIM_StartEvent(EVENT_1MS);
	}
	else if ((count % TIMER_COUNT_1) == 0x0) {
		PrfrmTIM_StartEvent(EVENT_1MS);
	}
}

// create a new thread for a event/task
static void PrfrmTIM_StartEvent(TeEVT_EventType event_type)
{
	pthread_t thread_id;
	void* event_fun;

	// get thread id for current event
	thread_id = GetEVT_p_ThreadId(event_type);
	// cancel the thread if it exists
	pthread_cancel(thread_id);

	// get event function for thread
	event_fun = GetEVT_p_EventFunction(event_type);

	// create thread
	pthread_create(&thread_id, NULL, event_fun, NULL);
	// set thread id in state structure
	SetEVT_ThreadId(event_type, thread_id);
}

