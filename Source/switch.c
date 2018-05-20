//内嵌汇编
__asm void PendSV_Handler(void)   //异常处理函数
{
	IMPORT blockPtr
	
	LDR R0,=blockPtr
	LDR R0,[R0]
	LDR R0,[R0]
	//保存寄存器值
	STMDB R0!,{R4-R11}          //批量往寄存器写入值
	
	LDR R1,=blockPtr
	LDR R1, [R1]
	STR R0, [R1]
	
	//添加测试代码
	ADD R4,R4, #1
	ADD R5,R5, #1
	
	LDMIA R0!,{R4-R11}
	BX LR   //退出异常
}
