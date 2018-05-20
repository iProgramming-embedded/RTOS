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

//定义一个缓冲区
unsigned long stackBuffer[1024];
BlockType_t block;
int main()
{
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
