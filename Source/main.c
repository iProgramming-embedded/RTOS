#include"tinyOS.h"

#define NVIC_INT_CRL     0xE000Ed04  //中断控制与状态寄存器
#define NVIC_PENDSVSET   0x10000000  //向寄存器写入值
#define NVIC_SYSPRI2     0xE000ED22  //系统优先寄存器
#define NVIC_PENDSV_PRI  0x000000FF  //配置优先级

#define MEM32(addr)  *(volatile unsigned long *)(addr)
#define MEME8(addr)  *(volatile unsigned char *)(addr)

//异常触发函数
void triggerPendSVC(void)
{
	MEME8(NVIC_SYSPRI2)=NVIC_PENDSV_PRI;   //最低优先级
	MEM32(NVIC_INT_CRL)=NVIC_PENDSVSET;    //用于PendSV
}

void tTaskInit (tTask * task, void (*entry)(void *), void * param, tTaskStack * stack)
{
	task->stack = stack;
}
typedef struct _BlockType_t
{
		unsigned long *stackPtr;
}BlockType_t;

BlockType_t *blockPtr;
void delay(int count)
{
	while(--count>0);
}

int flag;

unsigned long stackBuffer[1024];
BlockType_t block;

tTask tTask1;
tTask tTask2;

tTaskStack task1Env[1024];
tTaskStack task2Env[1024];

void task1 (void * param)
{
	for (;;)
	{
	}
}

void task2 (void * param)
{
	for (;;)
	{
	}
}

//定义一个缓冲区
unsigned long stackBuffer[1024];
BlockType_t block;
int main()
{
	tTaskInit(&tTask1, task1, (void *)0x11111111, &task1Env[1024]);
	tTaskInit(&tTask2, task2, (void *)0x22222222, &task2Env[1024]);
	block.stackPtr = &stackBuffer[1024];//指向最后一个单元
	blockPtr = &block;
	for(;;){
		flag = 0;
		delay(100);
		flag = 1;
		delay(100);
		
		triggerPendSVC();
	}
}
