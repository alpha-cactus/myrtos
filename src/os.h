#ifndef _OS_H_
#define _OS_H_

#include "common.h"

// typedef for unique CPU identifier
typedef enum CeOS_e_CPU {
	CPU0 = 0,
	CPU1 = 1,
	CPU2 = 2
} CeOS_e_CPU;

extern CeOS_e_CPU GetOS_e_CPU(void);
extern void PrfrmOS_SuspendInterrupts(void);
extern void PrfrmOS_ResumeInterrupts(void);
extern void CheckOS_Suspend(void);

#endif /* _OS_H_ */

