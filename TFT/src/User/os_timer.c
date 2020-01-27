#include "os_timer.h"
#include "includes.h"

u32 os_counter = 0;

//���ڲ���ϵͳʱ�ӣ�����������10ms�ж�һ��
void OS_TimerInit(u16 psc, u16 arr) {
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  RCC->APB1ENR |= 1 << 2;           //TIM4ʱ��ʹ��
  TIM4->ARR = arr;                  //�趨�Զ���װֵ
  TIM4->PSC = psc;                  //Ԥ��Ƶ��
  TIM4->SR = (uint16_t) ~(1 << 0);  //��������ж�
  TIM4->DIER |= 1 << 0;             //���������ж�
  TIM4->CR1 |= 0x01;                //ʹ�ܶ�ʱ��3
}

void TIM4_IRQHandler(void)  //TIM4�ж�
{
  if ((TIM4->SR & 0x01) != 0)  //���ָ����TIM�жϷ������:TIM �ж�Դ
  {
    os_counter++;
    //��ӡ������ͳ�ƴ�ӡ��ʱ
    setPrintingTime(os_counter);

    loopTouchScreen();

    //��ʱ���
    if (os_counter == (1 << 30)) {
      os_counter = 0;
    }

    TIM4->SR = (uint16_t) ~(1 << 0);  //���TIMx���жϴ�����λ:TIM �ж�Դ
  }
}

/* ϵͳʱ�� ��λ�� 10ms */
u32 OS_GetTime(void) {
  return os_counter;
}

/*
����:����ṹ���ʼ��
����:
task_t:����������ṹ��
time:������ִ��һ��
task:��ִ�е�����
para:��ִ������Ĳ���
*/
void OS_TaskInit(OS_TASK *task, u32 time, FP_TASK function, void *para) {
  task->time = time;
  task->task = function;
  task->para = para;
}
/*
����:����ѭ���е���,����Ƿ���Ҫִ������
����:��ִ�������Ӧ������ṹ��
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
����:ʹ�� task_t ��Ӧ������
����:
task_t:��ִ�������Ӧ������ṹ��
is_exec:�������Ƿ�����ִ��
is_repeat���������Ƿ��ظ�ִ��
*/
void OS_TaskEnable(OS_TASK *task_t, u8 is_exec, u8 is_repeat) {
  task_t->is_exist = 1;
  task_t->is_repeat = is_repeat;
  task_t->start_time = OS_GetTime();
  if (is_exec)
    (*task_t->task)(task_t->para);
}

/*
����:ʧ�� task_t ��Ӧ������
����:��ִ�������Ӧ������ṹ��
*/
void OS_TaskDisable(OS_TASK *task_t) {
  task_t->is_exist = 0;
}
