#include <stdio.h>
#include "pthread_pool.h"
#include "main.h"
#include "cope_file.h"

int main(int argc,char* argv[])
{
    pthread_pool* pool = NULL;

    if(argc < 3)
    {
        printf("少了参数!!!\n");
        return -1;
    }

    //1.创建一个线程池并初始化
    pool = malloc(sizeof(*pool));
    Pool_Init(pool,10);

    //2.拷贝文件(线程池)
    Cp_dir(pool,argv[1],argv[2]);

    //3.销毁线程池
    Pool_Destroy(pool);

    printf("copy ok!!!\n");

    return 0;
}