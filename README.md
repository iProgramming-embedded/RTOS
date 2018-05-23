# RTOS
    一款基于stm32的操作系统内核从零开始编写，后续会移植到stm32f103系列单片机上.
    v0.1,初始工程创建,flag1的切换。
    v0.2,内核编程，用pendSVC触发异常，在异常处理函数中保存R4~R11到缓冲区，在恢复，模拟任务切换时的寄存器保护与恢复。
    v0.3,任务的定义与切换，任务的含义，切换的原理，堆，栈，数据，代码区（各部分作用与分配），任务切换本质，保持前一任务当前运行状态，恢复后一任务之前的运行状态（要保存的值）。
    v0.4,任务切换的实现。
