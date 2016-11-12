/*************************************************************************
	> File Name: pcbcontrol.c
	> Author: 
	> Mail: 
	> Created Time: Wed 23 Sep 2015 07:50:00 AM
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"pcb.h"
#include"getTime.h"
#define TIME_SLICE 2
#define true 1
#define false 0
void schedule(Queue create,Queue ready,Queue execute,int (*compare)(PCB *,PCB *),int pirod,int *pcbID);

enum Action{create=1,end,block,wake,scheduling};
int main(int argc, char *argv[])
{
    int pcbNum,i,action;
    Queue createQueue = newQueue(),readyQueue = newQueue(), executeQueue = newQueue(),  blockQueue = newQueue();
    Queue copy1,copy2,copy3;

    printf("Please input a unsigned integer as the maximum Pprocess of you PC!\n");
    scanf(" %d",&pcbNum);
    int * pcbID = (int *)malloc(sizeof(int)*pcbNum);
    for( i=0; i < pcbNum; i++) pcbID[i] = PCBSTART+i;
//    for(i = 0; i<pcbNum;i++) printf("%d\n",pcbID[i]);
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
                newPcb = createPCB(pcbID,pcbNum);
                addToQueue(createQueue,newPcb);
                showQueue(createQueue,"CREATE");
                break;
            case end:
                showQueue(executeQueue,"END");
                if(p = getFromQueue(executeQueue,NULL))
                {
                    freePCB(p,pcbID);
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
                copy1 = copyQueue(createQueue),
                copy2 = copyQueue(createQueue),
                copy3 = copyQueue(createQueue);
                showQueue(readyQueue,"createQueue");
                /*记录等待时间，用于分析总的等待时间，和平均等待时间*/
                currentTime = 0;
                printf("------First come first serve------------\n");
                printf("%10s,%10s,%10s,%10s\n","jobName","endTime","useTime","weightlyWaitTime");
                schedule(createQueue,readyQueue,executeQueue,NULL,false,pcbID);
                printf("------Short job first schedule----------\n");
                printf("%10s,%10s,%10s,%10s\n","jobName","endTime","useTime","weightlyWaitTime");
                currentTime = 0;
                schedule(copy1,readyQueue,executeQueue,sjf,false,pcbID);
                printf("Priority schedule :\n");
                printf("------Priority schedule-----------------\n");
                printf("%10s,%10s,%10s,%10s\n","jobName","endTime","useTime","weightlyWaitTime");
                currentTime = 0;
                schedule(copy2,readyQueue,executeQueue,priority,false,pcbID);
                printf("Time slice schedule :\n");
                printf("------Time slice schedule---------------\n");
                printf("%10s,%10s,%10s,%10s\n","jobName","endTime","useTime","weightlyWaitTime");
                currentTime = 0;
                schedule(copy3,readyQueue,executeQueue,NULL,true,pcbID);
        }
    }
    return 1;
}
         
void schedule(Queue create,Queue ready,Queue execute,int (*compare)(PCB *,PCB *),int pirod,int *pcbID)
{
    int jobNum = 0;
    long waitTime = 0;
    float weightlyWaitTime = 0;
    PCB * p, * get;
    while(create->next || ready->next || execute->next)
    {
        p  = create;
        while(p->next)
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

        if(execute->next)
        {
            if(!pirod || (execute->next->useTime <= TIME_SLICE))
            {
                jobNum++;
                p = getFromQueue(execute,NULL);
                currentTime = currentTime + p->useTime;
                waitTime = waitTime + currentTime - p->startTime;
                float weightly = (currentTime - p->startTime )/(float)p->serveTime;
                weightlyWaitTime = weightlyWaitTime + weightly;
                printf("%10s%10ld%10d%10.3f\n",p->name,currentTime,(int)(currentTime - p->startTime),weightly);
                freePCB(p,pcbID);
            }else
            {
                p = getFromQueue(execute,NULL);
                p->useTime = p->useTime -TIME_SLICE;
                currentTime = currentTime + TIME_SLICE;
                addToQueue(ready,p);
            }
            if(ready->next)
            {
                moveTo(ready,execute,compare);//按照传入的调度算法进行选择
            }
        }else if(ready->next)
        {
            moveTo(ready,execute,compare);
        }
    }
    printf("%d jobs average wait time:%ld,  average weightly wait time:%.3f\n",jobNum,waitTime/jobNum,weightlyWaitTime/jobNum);
}
