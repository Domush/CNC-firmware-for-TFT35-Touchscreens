#include "os_timer.h"
#include "includes.h"

u32 os_counter = 0;

//Used to generate the system clock, this project sets a 10ms interrupt once
void OS_TimerInit(u16 psc, u16 arr) {
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  RCC->APB1ENR |= 1 << 2;           //TIM4 clock enable
  TIM4->ARR = arr;                  //Set auto reload value
  TIM4->PSC = psc;                  //Prescaler
  TIM4->SR = (uint16_t) ~(1 << 0);  //Clear update interrupt
  TIM4->DIER |= 1 << 0;             //Allow update interrupts
  TIM4->CR1 |= 0x01;                //Enable timer 3
}

void TIM4_IRQHandler(void)  //TIM4 interrupt
{
  if ((TIM4->SR & 0x01) != 0)  //Check if the specified TIM interrupt occurs: TIM interrupt source
  {
    os_counter++;
    //Statistics printing takes time during printing
    setPrintingTime(os_counter);

    loopTouchScreen();

    //Timing overflow
    if (os_counter == (1 << 30)) {
      os_counter = 0;
    }

    TIM4->SR = (uint16_t) ~(1 << 0);  //Clear TIMx interrupt pending bit: TIM interrupt source
  }
}

/* System time unit is 10ms */
u32 OS_GetTime(void) {
  return os_counter;
}

/*
Function: Task structure initialization
parameter:
task_t: task structure to be filled
time: how often the task is executed
task: the task to be executed
para: parameters of the task to be executed
*/
void OS_TaskInit(OS_TASK *task, u32 time, FP_TASK function, void *para) {
  task->time = time;
  task->task = function;
  task->para = para;
}
/*
Function: Called in the main loop to detect whether the task needs to be executed
Parameters: the task structure corresponding to the task to be executed
*/
void OS_TaskCheck(OS_TASK *task_t) {
  if (task_t->is_exist == 0) return;
  if (OS_GetTime() < task_t->start_time + task_t->time) return;
  if (task_t->is_repeat == 0) {
    task_t->is_exist = 0;
  } else {
    task_t->start_time = OS_GetTime();
  }
  (*task_t->task)(task_t->para);
}

/*
Function: Enable the task corresponding to task_t
parameter:
task_t: the task structure corresponding to the task to be executed
is_exec: Whether this task is executed immediately
is_repeat: Whether this task is repeated
*/
void OS_TaskEnable(OS_TASK *task_t, u8 is_exec, u8 is_repeat) {
  task_t->is_exist = 1;
  task_t->is_repeat = is_repeat;
  task_t->start_time = OS_GetTime();
  if (is_exec)
    (*task_t->task)(task_t->para);
}

/*
Function: Task corresponding to disabled task_t
Parameters: the task structure corresponding to the task to be executed
*/
void OS_TaskDisable(OS_TASK *task_t) {
  task_t->is_exist = 0;
}
