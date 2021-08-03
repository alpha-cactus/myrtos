
#include "os.h"
#include "events.h"

// mutex for suspend interrupt functionality
pthread_mutex_t suspend_mutex;
pthread_cond_t resume_cond;
static uint8_t suspend_flag = 0;

// get unique CPU id for current thread
CeOS_e_CPU GetOS_e_CPU(void)
{
	CeOS_e_CPU retval;

	retval = CPU0;
	
	return retval;
}

// suspend interrupts 
void PrfrmOS_SuspendInterrupts(void)
{
	// perform mutex lock on suspend flag
	pthread_mutex_lock(&suspend_mutex);
	suspend_flag = 1;
	// lock/unlock assures atomic operation
	pthread_mutex_unlock(&suspend_mutex);
}

void PrfrmOS_ResumeInterrupts(void)
{
	// perform mutex lock on suspend flag
	pthread_mutex_lock(&suspend_mutex);
	suspend_flag = 0;
	// lock/unlock assures atomic operation
	pthread_cond_broadcast(&resume_cond);
	pthread_mutex_unlock(&suspend_mutex);
}

void CheckOS_Suspend(void)
{
	// wait check on suspend flag
	// any thread can all this during a point at which it needs
	// to check if interrupts are suspended
	pthread_mutex_lock(&suspend_mutex);
	while (suspend_flag !=0)
	{
		pthread_cond_wait(&resume_cond, &suspend_mutex);
	}
	pthread_mutex_unlock(&suspend_mutex);
}
