#include "mFw.h"

task_t task;

void test_loop(void)
{
    dbg("test");

    task_sleep(&task, 1000);
}

void testInit(void)
{
    task_registration(&task, test_loop);
}
