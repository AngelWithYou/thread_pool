#ifndef __PTHREAD_POOL_H__
#define __PTHREAD_POOL_H__

#define MAX_ACTIVE_PTHREADS  50
#define MAX_TASKS            100

#include<stdbool.h>
#include<pthread.h>
//任务结点的类型
//任务链表上的任务结点,只需要描述好一个任务就可以了
//线程会不断的从任务队列上获取任务
struct task
{
   void* (*do_task)(void*arg);//线程的函数指针,指向要完成的那个任务
   void* arg;
   struct task* next;//指向下一个任务结点
};

typedef struct pthread_pool
{
   //有多个线程,就会有多个tid
   pthread_t* tids;
   
   //线程池中正在服役的线程(正在执行任务的线程数)
   unsigned int active_threads;
   
   //任务队列(链表),所有的线程都需要从任务队列上获取任务执行
   struct task* task_list;//指向第一个需要被执行的任务
   
   //互斥锁,用来保护这个任务  "任务队列"要互斥访问
   pthread_mutex_t lock;//互斥锁
   
   //线程的条件变量,表示"任务队列"是否有任务
   pthread_cond_t cond;//条件变量
   
   //记录程序的状态  退出  不退出
   bool shutdown;//FAKSE 不退出 /TRUE 退出
   
   //线程池最多能够容纳多少个线程
   unsigned int max_threads;
   
   //线程池中任务链表当前的任务数量
   unsigned int cur_tasks;
   
   //线程池中任务队列最大的任务数量
   unsigned int max_waiting_task;
   
   //...
      
}pthread_pool;

/*
	Pool_Init : 初始化一个线程池
			初始化你指定的那一个线程池,线程池中能够有多少个初始线程
	@pool : 指向你要初始化的那一个线程池
	@thread_num : 你要初始化的线程池 中开始的线程数目
	返回值 :
		成功 返回 0
		失败 返回 -1
*/
int Pool_Init(pthread_pool* pool,unsigned int thread_num);

/*
	Routine : 任务调配函数,所有的线程开始都执行此函数
			此函数会不断地从线程池的任务队列中取下任务结点
			交给线程去完成/执行,
			任务结点中 包含了 任务函数指针 和 任务函数所需要的参数
*/
void* Routine(void* arg);

/*
	Pool_Destroy : 销毁一个线程池
			销毁线程池之前要保证所有的任务都已经完成.
	@pool : 要销毁的线程池
	返回值 :
		无
*/
void Pool_Destroy(pthread_pool* pool);

/*
	add_task : 给任务链表添加任务结点
	@pool : 线程池指针
	@fun_task : 你添加的任务是谁 ==> 线程函数指针
	@fun_arg :  你要执行的那个任务的参数
	返回值 :
		无
*/
void add_task(pthread_pool* pool,void*(*fun_task)(void*arg),void* fun_arg );

/*****add_thread函数实现*****/
//利用pthread_create创建线程,并且把线程id保存到线程池 tids数组中去
//让每一个新创建的线程去执行任务调配函数
/*
	add_thread : 往线程池中新增几个线程
	@pool : 线程池指针
	@add_thread_num : 表示你要添加多少个线程
	返回值 :
		返回 添加之后,正在服役的线程数
		返回 -1,失败
*/
int add_thread(pthread_pool* pool,unsigned int add_thread_num);

/*****remove_thread*****/
//利用pthread_cancel 去取消部分线程
//利用 pthread_join 回收被取消线程的资源
/*
	remove_thread : 销毁部分线程
	@pool : 线程池指针
	@remove_thread_num : 销毁线程数目
	返回值 :
		成功 返回销毁之后,线程池中 活跃的线程数(包括工作的和休眠的)
		失败 返回-1
*/
int remove_thread(pthread_pool* pool,unsigned int remove_thread_num);
#endif