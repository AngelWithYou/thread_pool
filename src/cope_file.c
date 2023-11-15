#include "main.h"
#include "cope_file.h"
#include "pthread_pool.h"



//实现一个目录（只考虑目录 和 普通文件）的拷贝



//判断文件是否为普通文件
int Is_file(char *filename) //传入文件名
{
    struct stat st;
    int ret = stat(filename,&st);
    if(ret == -1)
    {
        perror("stat error");
        return -1;
    }
    if(S_ISREG(st.st_mode))     //如果是普通文件
    {
        return 0;
    }
    else if(S_ISDIR(st.st_mode))    //如果是目录
    {
        return 1;
    }
}


void *Cp_file(void *arg)   //将file1拷贝到file2
{
    File *file = (File*)arg;

    FILE *fd = fopen(file->file1,"r"); //打开文件
            //printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa:%s\n",file->file1);
            printf(" %s L_%d  %s\n",__FUNCTION__,__LINE__,file->file1);
    FILE *fd1 = fopen(file->file2,"w"); //打开文件2 没有则创建
            //printf("bbbbbbbbbbbbbbbbbbbbbbbbbbbbb:%s\n",file->file2);
            printf(" %s L_%d  %s\n",__FUNCTION__,__LINE__,file->file2);
    char buf[1000];
    while (!feof(fd))
    {
        size_t cnt = fread(buf,1,sizeof(fd),fd);    //将 file1 的文件内容存进数组
        fwrite(buf,1,cnt,fd1);  //将 buf 里的数组写入
    }
    //printf("%s\n",buf);
    fclose(fd);
    fclose(fd1);
}



void Cp_dir(pthread_pool* pool,char *dir1,char *dir2)   //将dir1拷贝到dir2
{
    mkdir(dir2,0777);

    DIR* dir = NULL;
    struct dirent* dirp = NULL;


    //1.打开一个目录
    dir = opendir(dir1);
    if(dir == NULL)
    {
        perror("opendir error");
        return;
    }



    //2遍历目录
    while (dirp = readdir(dir))
    {
        if((!strcmp(dirp->d_name,".")) || (!strcmp(dirp->d_name,"..")))
        {
            continue;
        }

        
        printf("\n");
        char buf[512];
        sprintf(buf,"%s/%s",dir1,dirp->d_name);
        
        char buf1[512];
        sprintf(buf1,"%s/%s",dir2,dirp->d_name);
        
        if(Is_file(buf) == 0)
        {
            File file;
            
            strcpy(file.file1,buf);
            //printf("ssssssssssssss:%s\n",file.file1);
            printf(" %s L_%d  %s\n",__FUNCTION__,__LINE__,file.file1);
            strcpy(file.file2,buf1);
            //printf("ffffffffffffff:%s\n",file.file2);
            printf(" %s L_%d  %s\n",__FUNCTION__,__LINE__,file.file2);

            add_task(pool,Cp_file,(void*)(&file));
            //usleep(1000);

            printf("mmmmmmmm\n");
            
        }
        else if (Is_file(buf) == 1)
        {
            Cp_dir(pool,buf,buf1);
        }
        
    }

    closedir(dir);
    
}

