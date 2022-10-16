#include "user.h"
void chose(int *p);
int main()
{
    int p[2];
    pipe(p);//创建管道

    if(fork()!=0)//父进程
    {
        //关闭读端
        close(p[0]);
        //父进程写，子进程读
        for(int i=2;i<=35;i++)
        {
            write(p[1],&i,sizeof(int));//把数字写到管道里面去
        }
        close(p[1]);//关闭写端
        wait(0);


    }
    else//子进程
    {
        chose(p);//把管道传进去
    }
    exit(0);

}

void chose(int *p)
{
    //关闭写端
    close(p[1]);
    int i;
    //先读第一个
    int read_first=read(p[0],&i,sizeof(int));
    //对读第一个数的情况进行判断
    if(read_first==0)//也是递归终止的条件
    {
        //关闭读端
        close(p[0]);
        //结束进程
        exit(0);
    }
    else if(read_first>0)//正常读入的情况
    {
        printf("prime %d\n",i);
    }
    else
    {
        printf("error\n");
        exit(1);
    }

    //创建和下一个进程进行通信的管道
    int next_pipe[2];
    pipe(next_pipe);
    int k;
    if(fork()!=0)//父进程
    {
        //把不整除i的丢给下一个进程
        //先关闭读端口
        close(next_pipe[0]);
        while (read(p[0],&k,sizeof(int))>0)//还没有把输入读完的时候
        {
            if(k%i!=0)//如果k不整除i
            {
                write(next_pipe[1],&k,sizeof(int));//把k传给下一个进程去处理
            }
        }
        close(next_pipe[1]);//关闭写端
        wait(0);
        exit(0);
    }
    else//子进程
    {
        //递归调用
        chose(next_pipe);
    }

}