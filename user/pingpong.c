#include"user.h"
int main()
{
    char byte11[4];
    char byte12[4]="pong";
    char byte21[4];//要用的指针
    char byte22[4]="ping";
//创建管道
  int p[2];
  int num1,num2;
  pipe(p);
  //sprintf("before fork\n");
  if(fork()==0)//创建子进程
  {
    //printf("in child\n");
    //使用read和write对管道进行读写
    write(p[1],byte12,sizeof(byte12));
    num1=read(p[0],byte11,sizeof(byte11));
    if(num1<0)
    {
        printf("can't read\n,num1=%d",num1);//如果返回值小于0就是异常
    }
    printf("%d: received %s\n",getpid(),byte11);
   
    close(p[0]);//关闭通道
    close(p[1]);
    exit(0);

  }
  else
  {
    //printf("in parent,before wait\n");
    wait(0);
    write(p[1],byte22,sizeof(byte22));
    //printf("in parent,after wait\n");
    num2=read(p[0],byte21,sizeof(byte21));
    if(num2<0)
    {
        printf("num2 is:%d,read error\n",num2);
    }
    printf("%d: received %s\n",getpid(),byte21);
   
    close(p[1]);
    close(p[0]);
    exit(0);
  }
}