#include "mFw.h"

task_t task;

void test_loop(void)
{
    // console.printf("\ntest");
    dbg("test");

    task_sleep(&task, 1000);
}

void testInit(void)
{
    task_registration(&task, test_loop);
}
