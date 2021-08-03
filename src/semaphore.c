#include "semaphore.h"

// semaphore array
static uint8_t semaphores[TOTAL_NUM_SEMAPHORES];
// spinlock counter array for nested calls
static uint8_t spinlock_ctr[TOTAL_NUM_SEMAPHORES];

// semaphore mutex for atmoic access
pthread_mutex_t semaphore_mutex;

static inline uint8_t GetSEM_y_CPU_SemLockVal(void);
static inline void PrfrmSEM_AtmLock(uint8_t semaphore_id, uint8_t lock_val);
static inline void PrfrmSEM_AtmUnlock(uint8_t semaphore_id);

// returns unique CPU value for semaphore locking
static inline uint8_t GetSEM_y_CPU_SemLockVal(void)
{
	return ((uint8_t)GetOS_e_CPU() + 1);
}

// atmoic semaphore lock routine
static inline void PrfrmSEM_AtmLock(uint8_t semaphore_id, uint8_t lock_val)
{
	// lock mutex
	pthread_mutex_lock(&semaphore_mutex);
	// set semaphore to cpu lock value
	semaphores[semaphore_id] = lock_val;
	// unlock mutex
	pthread_mutex_unlock(&semaphore_mutex);
}

// atomic semaphore unlock
static inline void PrfrmSEM_AtmUnlock(uint8_t semaphore_id)
{
	// lock mutex
	pthread_mutex_lock(&semaphore_mutex);
	// unlock semaphore
	semaphores[semaphore_id] = SEMAPHORE_UNLOCKED;
	// unlock mutex
	pthread_mutex_unlock(&semaphore_mutex);
}

// get function for semaphore status
uint8_t GetSEM_y_SemaphoreStat(uint8_t semaphore_id)
{
	return semaphores[semaphore_id];
}

uint8_t PrfrmSEM_y_LockSemaphore(uint8_t semaphore_id)
{
	uint8_t status;

	// suspend interrupts/premption for semaphore lockn
	PrfrmOS_SuspendInterrupts();

	// read semaphore status
	status = GetSEM_y_SemaphoreStat(semaphore_id);

	// check if unlocked
	if (status == SEMAPHORE_UNLOCKED) {
		uint8_t lock_val;

		// if unlocked get cpu specific lock value 
		lock_val = GetSEM_y_CPU_SemLockVal();

		// perform semaphore lock
		PrfrmSEM_AtmLock(semaphore_id, lock_val);

		// read semaphore lock status for return
		status = GetSEM_y_SemaphoreStat(semaphore_id);
	}

	// resume interrupts
	PrfrmOS_ResumeInterrupts();

	return status;
}

uint8_t PrfrmSEM_y_UnlockSemaphore(uint8_t semaphore_id)
{
	uint8_t status;
	uint8_t lock_val;

	// suspend interrupts/preemption for semaphore unlock
	PrfrmOS_SuspendInterrupts();

	// read cpu specific lock value
	lock_val = GetSEM_y_CPU_SemLockVal();

	// read semaphore status
	status = GetSEM_y_SemaphoreStat(semaphore_id);

	// if status does NOT equal lock value then current
	// cpu does not own the semaphore lock
	if (status == lock_val)
	{
		// else attempt semaphore unlock
		PrfrmSEM_AtmUnlock(semaphore_id);

		// read semaphore status for return
		status = GetSEM_y_SemaphoreStat(semaphore_id);
	}

	// resume interrupts
	PrfrmOS_ResumeInterrupts();

	return status;
}

void PrfrmSEM_SpinlockAcquire(uint8_t semaphore_id)
{
	uint8_t status;
	uint8_t lock_val;

	//suspend interrupts during spinlock acquisition
	//interrupts resume by PrfrmOS_SpinlockRelease
	PrfrmOS_SuspendInterrupts();

	// read cpu specific lock value
	lock_val = GetSEM_y_CPU_SemLockVal();

	do {
		// read semaphore status
		status = GetSEM_y_SemaphoreStat(semaphore_id);

		// if unlocked attempt semaphore lock
		if (status == SEMAPHORE_UNLOCKED) 
		{
			PrfrmSEM_AtmLock(semaphore_id, lock_val);

			// read status for check before return
			status = GetSEM_y_SemaphoreStat(semaphore_id);
		}

		// repreat while not locked
	} while (status != GetSEM_y_CPU_SemLockVal());

	// increment spinlock counter
	// note we always increment as spinlock calls can be nested
	spinlock_ctr[semaphore_id]++;
}

void PrfrmSEM_SpinlockRelease(uint8_t semaphore_id)
{
	uint8_t status;
	uint8_t lock_val;

	// get status
	status = GetSEM_y_SemaphoreStat(semaphore_id);
	
	// get lock val
	lock_val = GetSEM_y_CPU_SemLockVal();

	if (status == lock_val) {
		// check spinlock count
		if (spinlock_ctr[semaphore_id] == 0) {
			// do nothing if zero count
		}
		else {
			// decrement counter
			spinlock_ctr[semaphore_id]--;
	
			// check if counter is 0 after decrement
			if (spinlock_ctr[semaphore_id] == 0) 
			{
				// if counter is 0 we can unlock the semaphore
				status = GetSEM_y_SemaphoreStat(semaphore_id);

				while (status == lock_val)
				{
					PrfrmSEM_AtmUnlock(semaphore_id);

					status = GetSEM_y_SemaphoreStat(semaphore_id);
				}

				// resume interrupts for nested calls
				PrfrmOS_ResumeInterrupts();
			}
		}
	}
}

