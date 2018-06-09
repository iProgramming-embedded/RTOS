#include"tinyOS.h"
#include "ARMCM3.H"

tTask * currentTask;
tTask * nextTask;
tTask * idleTask;

//优先级位图
tBitmap taskPrioBitmap;
tList taskTable[TINYOS_PRO_COUNT];
//调度锁计数器
uint8_t schedLockCount;

tList tTaskDelayedList;
//异常触发函数
void triggerPendSVC(void)
{

}


tTask * tTaskHighestReady(void)
{
	uint32_t highestPrio = tBitmapGetFirstSet(&taskPrioBitmap);
	tNode * node = tListFirst(&taskTable[highestPrio]);
	return tNodeParent(node,tTask,linkNode);
}
//锁定计数器初始化
void tTaskSchedInit(void)
{
	int i;
	schedLockCount = 0;
	tBitmapInit(&taskPrioBitmap);
	for(i=0;i<TINYOS_PRO_COUNT;i++)
	{
		tListInit(&taskTable[i]);
	}
}

//上锁，禁止调度函数
void tTaskSchedDisable(void)
{
	uint32_t status= tTaskEnterCritical();
	
	if(schedLockCount < 255){
		schedLockCount++;
	}
	
	tTaskExitCritical(status);
}
void tTaskSchedEnable()
{
	uint32_t status = tTaskEnterCritical();
	
	if(schedLockCount > 0){
		if(--schedLockCount == 0){
			tTaskSched();
		}
	}
	
	tTaskExitCritical(status);
}

void tTaskSchedRdy (tTask * task)
{
  tListAddFirst(&(taskTable[task->prio]), &(task->linkNode));
	tBitmapSet(&taskPrioBitmap, task->prio);
}

void tTaskSchedUnRdy (tTask * task)
{
	tListRemove(&taskTable[task->prio], &(task->linkNode));
	if (tListCount(&taskTable[task->prio]) == 0)
	{
		tBitmapClear(&taskPrioBitmap, task->prio);
	}
}

void tTaskSchedRemove (tTask * task)
{
	tListRemove(&taskTable[task->prio], &(task->linkNode));
	if (tListCount(&taskTable[task->prio]) == 0)
	{
		tBitmapClear(&taskPrioBitmap, task->prio);
	}
}

void tTaskSched()
{
	tTask * tempTask;
	uint32_t status = tTaskEnterCritical();
	
	if(schedLockCount > 0){
		tTaskExitCritical(status);
		return;
	}
	tempTask = tTaskHighestReady();
	if(tempTask != currentTask){
		nextTask = tempTask;
		tTaskSwitch();
	}
	
	tTaskExitCritical(status);
}	

void tTaskDelayedInit(void)
{
	tListInit(&tTaskDelayedList);
}

void tTimeTaskWait (tTask * task, uint32_t ticks)
{
	task->delayTicks = ticks;
	tListAddLast(&tTaskDelayedList, &(task->delayNode));
	task->state |= TINYOS_TASK_STATE_DELAYED;
}

void tTimeTaskWakeUp (tTask * task)
{
	tListRemove(&tTaskDelayedList, &(task->delayNode));
	task->state &= ~TINYOS_TASK_STATE_DELAYED;
}

void tTimeTaskRemove (tTask * task)
{
	tListRemove(&tTaskDelayedList, &(task->delayNode));
}
void tTaskSystemTickHandler ()
{
	tNode * node;
	
	uint32_t status = tTaskEnterCritical();
	
	for (node = tTaskDelayedList.headNode.nextNode; node != &(tTaskDelayedList.headNode); node = node->nextNode)
	{
		tTask * task = tNodeParent(node, tTask, delayNode);
		if (--task->delayTicks == 0)
		{
			if (task->waitEvent)
			{
				tEventRemoveTask(task, (void *)0, tErrorTimeout);
			}
			
			tTimeTaskWakeUp(task);
			
			tTaskSchedRdy(task);
		}
	}
	
	if (--currentTask->slice == 0)
	{
		if (tListCount(&taskTable[currentTask->prio]) > 0)
		{
			tListRemoveFirst(&taskTable[currentTask->prio]);
			tListAddLast(&taskTable[currentTask->prio], &(currentTask->linkNode));
			
			currentTask->slice = TINYOS_SLICE_MAX;
		}
	}
		
	tTaskExitCritical(status);
	
	tTaskSched();
}




//添加空闲任务
tTask tTaskIdle;
tTaskStack idleTaskEnv[1024];

void idleTaskEntry(void * param)
{
	for(;;)
	{
	}
}

//定义一个缓冲区

int main()
{
	tTaskSchedInit();
	
	tTaskDelayedInit();
	
	tInitApp();


	tTaskInit(&tTaskIdle, idleTaskEntry, (void *)0, TINYOS_PRO_COUNT-1,&idleTaskEnv[TINYOS_IDLETASK_STACK_SIZE]);
	idleTask = &tTaskIdle;
	
	nextTask = tTaskHighestReady();
	
	tTaskRunFirst();
	
	return 0;
}
