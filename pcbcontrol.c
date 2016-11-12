/*************************************************************************
	> File Name: pcbcontrol.c
	> Author: 
	> Mail: 
	> Created Time: Wed 23 Sep 2015 07:50:00 AM
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"pcb.h"
#define TIME_SLICE 2//时间片为 2
#define true 1
#define false 0
void schedule(Queue create,Queue ready,Queue execute,int (*compare)(PCB *,PCB *),int pirod);

enum Action{create=1,end,block,wake,scheduling};//定义各个分支的唯一枚举，方便理解
int main(int argc, char *argv[])
{
    int pcbNum,i,action;
    Queue createQueue = newQueue(),readyQueue = newQueue(), executeQueue = newQueue(),  blockQueue = newQueue();
    Queue copy1,copy2,copy3;

    enum Action goWhich ;
    PCB *newPcb, *p;
    while(1)
    {
        printf("1--------Create process\n");
        printf("2--------End process\n");
        printf("3--------Block process\n");
        printf("4--------Wake process\n");
        printf("5--------Schedule process\n");
        printf("Please select one with input a number\n");
        
        //srand(time(NULL));
        //goWhich = ( enum Action)(rand()%5+1);
        scanf(" %u",&goWhich);
        switch(goWhich)
        {
            case create:
                newPcb = createPCB();
                addToQueue(createQueue,newPcb);
                showQueue(createQueue,"CREATE");
                break;
            case end:
                showQueue(executeQueue,"END");
                if(p = getFromQueue(executeQueue,NULL))
                {
                    freePCB(p);
                    showQueue(executeQueue,"END");
                }//else
                //printf("The execute queue is empoty!Can't execute END action.\n");
                break;
            case block:
                if(moveTo(executeQueue,blockQueue,NULL))
                {
                    showQueue(executeQueue,"EXECUTE");
                    showQueue(blockQueue,"BLOCK");
                }
                //else{
                //    printf("Fail to block current PCB!May be there is no pcocess is on execute\n");
                //}
                break;
            case wake:
                if(moveTo(blockQueue,readyQueue,NULL))
                {
                    showQueue(readyQueue,"READY");
                    showQueue(blockQueue,"BLOCK");
                }
                break;
            case scheduling:
		/*
		由于使用了四种调度算法，处理同一组数据，而每一种调度之后都改变了原始数据，是指在执行结束后释放了
		由于不想从新输入数据，就想处理之前拷贝成几个相同的队列
		*/
                copy1 = copyQueue(createQueue),
                copy2 = copyQueue(createQueue),
                copy3 = copyQueue(createQueue);
                showQueue(readyQueue,"createQueue");//打印输出
                /*记录等待时间，用于分析总的等待时间，和平均等待时间*/
                currentTime = 0;
		//先来先服务
                printf("------First come first serve------------\n");
                printf("%10s,%10s,%10s,%10s\n","jobName","endTime","useTime","weightlyWaitTime");
		//由于先来先服务直接取队列首就行了，所以第四个参数不用比较算法：传NULL。也不用时间片，四五个参数传false
                schedule(createQueue,readyQueue,executeQueue,NULL,false);
                printf("------Short job first schedule----------\n");//短作业的提示
                printf("%10s,%10s,%10s,%10s\n","jobName","endTime","useTime","weightlyWaitTime");
                currentTime = 0;//相对时间置0
		//由于短作业，所以第四个参数要用段作业比较函数：传sjf函数。也不用时间片，四五个参数传false
                schedule(copy1,readyQueue,executeQueue,sjf,false);
                printf("------Priority schedule-----------------\n");//优先级的提示
                printf("%10s,%10s,%10s,%10s\n","jobName","endTime","useTime","weightlyWaitTime");
                currentTime = 0;
		//第四个参数传优先级比较函数，时间片false
                schedule(copy2,readyQueue,executeQueue,priority,false);
                printf("------Time slice schedule---------------\n");//时间片
                printf("%10s,%10s,%10s,%10s\n","jobName","endTime","useTime","weightlyWaitTime");
                currentTime = 0;
		//启用时间片
                schedule(copy3,readyQueue,executeQueue,NULL,true);
        }
    }
    return 1;
}
 

/*
由于四种调度的代码重用极高，就想将其写成一个函数，将不同的地方以参数的方式传入，
前三个分别是:创建队列，就绪队列，执行队列，
第四个是从队列中去节点时用哪种方法，
第五个是是否使用时间片
其中PCB是同时创建的，但是到达时间不同，就增加了创建态，认为，只要到达时间小于当前时间就创建完了，将其移到就绪态   
*/     
void schedule(Queue create,Queue ready,Queue execute,int (*compare)(PCB *,PCB *),int pirod)
{
    int jobNum = 0;
    long waitTime = 0;
    float weightlyWaitTime = 0;
    PCB * p, * get;
    while(create->next || ready->next || execute->next)//三个队列其一不空就一直调用
    {
        p  = create;
        while(p->next)//每次循环都要将到达时间<= 当前时间的移到就绪态
        {
            if(p->next->startTime <= currentTime)
            {
                get = p->next;
                p->next = get->next;
                get->next = NULL;
                addToQueue(ready,get);
            }else
            {
                p = p->next;
            }
        }

        if(execute->next)//执行态有PCB在执行
        {
            if(!pirod || (execute->next->useTime <= TIME_SLICE))//没有使用时间片或者该pcb的剩余时间小于时间片，就让其执行完，并释放她
            {
                jobNum++;
                p = getFromQueue(execute,NULL);
                currentTime = currentTime + p->useTime;
                waitTime = waitTime + currentTime - p->startTime;
                float weightly = (currentTime - p->startTime )/(float)p->serveTime;
                weightlyWaitTime = weightlyWaitTime + weightly;
                printf("%10s%10ld%10d%10.3f\n",p->name,currentTime,(int)(currentTime - p->startTime),weightly);
                freePCB(p);
            }else
            {//否则当前时间增加一个时间片
                p = getFromQueue(execute,NULL);
                p->useTime = p->useTime -TIME_SLICE;
                currentTime = currentTime + TIME_SLICE;
                addToQueue(ready,p);//并将其移到就绪队列尾，等待下一次轮转
            }
            if(ready->next)//此时就绪态有作业需要执行的话就挑一个
            {
                moveTo(ready,execute,compare);//按照传入的调度算法进行选择挑作业最短的还是优先级最高的，或是队列首的
            }
        }else if(ready->next)//否则直接从就绪态挑一个
        {
            moveTo(ready,execute,compare);//按照传入的调度算法进行选择挑作业最短的还是优先级最高的，或是队列首的
        }
    }
    printf("%d jobs average wait time:%ld,  average weightly wait time:%.3f\n",jobNum,waitTime/jobNum,weightlyWaitTime/jobNum);
}
