#include "mCore.h"

/****************************************************************************************************/

// #include "templateTask.h"

extern void templateTask_init(void);

extern void mMessagePubTestTask_init(void);
extern void mMessageSubTestTask_init(void);

extern void mConsoleTask_init(void);


/**
 * 说明 : 添加任务后,将任务的初始化函数写在该函数中
*/
void TaskInit_config(void)
{
	templateTask_init();
	
	mMessagePubTestTask_init();
	mMessageSubTestTask_init();

	mConsoleTask_init();
}

/****************************************************************************************************/
