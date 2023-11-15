#ifndef __COPE_FILE_H__
#define __COPE_FILE_H__


#include "pthread_pool.h"

//保存普通文件的文件名
typedef struct file
{
    char file1[512];
    char file2[512];
}File;


//判断文件是否为普通文件
int Is_file(char *filename); //传入文件名


void *Cp_file(void *arg);   //将file1拷贝到file2


void Cp_dir(pthread_pool* pool,char *dir1,char *dir2);  //将dir1拷贝到dir2



#endif