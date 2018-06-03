#ifndef TINYOS_H
#define TINYOS_H

#include <stdint.h>
#include "tLib.h"
#include "tConfig.h"

#define TINYOS_TASK_STATE_RDY 0
#define TINYOS_TASK_STATE_DELAYED (1 << 1)

typedef uint32_t tTaskStack;

typedef struct _tTask {
	tTaskStack * stack;
	uint32_t delayTicks;  //软延时计数器
	tNode delayNode;
	uint32_t prio;   //添加优先级字段
	uint32_t state;
}tTask;

extern tTask * currentTask;
extern tTask * nextTask;

uint32_t tTaskEnterCritical (void);
void tTaskExitCritical (uint32_t status);

void tTaskRunFirst(void);
void tTaskSwitch(void);

void tTaskSchedInit(void);
void tTaskSchedDisable(void);
void tTaskSchedEnable(void);
void tTaskSched(void);
#endif
