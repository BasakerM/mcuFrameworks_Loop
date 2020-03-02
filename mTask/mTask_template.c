#include "mCore.h"

void templateTast_loop(void);
void templateTast_loop1(void);
void templateTast_loop2(void);

m_task_t templataTask_task; // 任务块
// m_task_t templataTask_task1; // 任务块
// m_task_t templataTask_task2; // 任务块

// 需要将该初始化函数,添加到 mConfig.c 文件中
void templateTask_init(void)
{
    mCore.task.registration(&templataTask_task,templateTast_loop); // 注册任务
    // mCore.task.registration(&templataTask_task1,templateTast_loop1); // 注册任务
    // mCore.task.registration(&templataTask_task2,templateTast_loop2); // 注册任务
}

m_bool_t templataTask_flag = m_false;

void templateTast_loop(void)
{
    templataTask_flag = (templataTask_flag == m_true)? (m_false):(m_true);
    mCore.task.setCall(&templataTask_task,templateTast_loop1);
    // mCore.task.sleep(&templataTask_task,10); // 休眠10ms
}

m_bool_t templataTask_flag1 = m_false;

void templateTast_loop1(void)
{
    templataTask_flag1 = (templataTask_flag1 == m_true)? (m_false):(m_true);
    mCore.task.setCall(&templataTask_task,templateTast_loop2);
    // mCore.task.sleep(&templataTask_task1,10); // 休眠10ms
}

m_bool_t templataTask_flag2 = m_false;

void templateTast_loop2(void)
{
    templataTask_flag2 = (templataTask_flag2 == m_true)? (m_false):(m_true);
    mCore.task.setCall(&templataTask_task,templateTast_loop);
    mCore.task.sleep(&templataTask_task,10); // 休眠10ms
    // mCore.task.sleep(&templataTask_task2,10); // 休眠10ms
}
