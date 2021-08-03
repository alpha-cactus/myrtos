#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "common.h"

#define SEMAPHORE_LOCK_ADD 1u
#define TOTAL_NUM_SEMAPHORES 10u
#define SEMAPHORE_UNLOCKED 0u

extern uint8_t PrfrmSEM_y_LockSemaphore(uint8_t semaphore_id);
extern uint8_t PrfrmSEM_y_UnlockSemaphore(uint8_t semaphore_id);
extern void PrfrmSEM_SpinlockAcquire(uint8_t semaphore_id);
extern void PrfrmSEM_SpinlockRelease(uint8_t semaphore_id);

#endif /* _SEMAPHORE_H_ */
