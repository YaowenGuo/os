/*************************************************************************
	> File Name: pcb.c
	> Author: 
	> Mail: 
	> Created Time: Fri 18 Sep 2015 08:06:40 AM
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"getTime.h"
#include"pcb.h"
long currentTime = 0;
Queue newQueue()
{
    Queue q = ( Queue )malloc(sizeof(PCB));
    q->next = NULL;
    return q;
}
Queue createPCB(int * pcb,int pcbNum)
{
    int i=0;
    PCB * newPcb = NULL;
    while(pcb[i] == 0 && i<pcbNum ) ++i;
    if( i<pcbNum )
    {
        newPcb = (PCB *)malloc(sizeof(PCB));
        newPcb->next = NULL;
        newPcb->ID = pcb[i];
        pcb[i] = 0;
        //newPcb->startTime = getCurrentTime();
        printf("Please input PCB name:");
        scanf(" %s",newPcb->name);
        printf("Please input start time:");
        scanf(" %ld",&(newPcb->startTime));
        printf("Please input serve time:");
        scanf(" %d",&(newPcb->serveTime));
        newPcb->useTime = newPcb->serveTime;

    }
    return newPcb;
}
Queue copyQueue(Queue head)
{
    Queue createQueue = newQueue(), newNode;
    while(head->next)
    {
        head = head->next;
        newNode = (PCB *)malloc(sizeof(PCB));
        *newNode = * head;
        addToQueue(createQueue,newNode);
    }
    return createQueue;
}

int freePCB(PCB * pcb,int *pcbId )
{
    pcbId[pcb->ID-PCBSTART] = pcb->ID;
    free(pcb);
    return 1;
}
void showQueue( Queue head ,char *name)
{
    printf("Current %s queue list is:\n",name);
    while( head->next )
    {
        head = head->next;
        printf("pcb: %s,start Time:%ld, serveTime: %d\n",head->name,head->startTime,
                head->serveTime);
    }
}
void clearQueue(Queue head,int id)
{
    PCB * p;
    p = head;
    while( p->next->ID != id)
        p = p->next;
    p->next = p->next->next;
}
int sjf(PCB * p1, PCB * p2)
{
    return p1->useTime < p2->useTime;
}
int priority(PCB * p1, PCB * p2)
{ 
    return (currentTime - p1->startTime + p1->useTime)/p1->useTime > 
        (currentTime - p2->useTime + p2->useTime)/p2->useTime;
}
/*
 *从队列中取得一个节点,队列为空就返回NULL
*/
PCB * getFromQueue(Queue head, int (*compare)(PCB* pcb1,PCB* pcb2))
{
    PCB * p = head,* get,*priorGet;
    if(head->next )
    {
        get = head->next;
        priorGet = head;
        p = get;
    }else
        return NULL;
    if( compare )
    {
        while(p->next)
        {
            if((*compare)(p->next, get))
            {
                get = p->next;
                priorGet = p;
            }
            p = p->next;
        }
    }
    priorGet->next = get->next;
    get->next = NULL;
    return get;
        
}
/*
 *将一个节点存入队列中
 */
int addToQueue(Queue head, PCB *pcb)
{
    if(pcb)
    {
        PCB *p = head;
        while(p->next)
            p = p->next;
        p->next = pcb;
        pcb->next = NULL;
        return 1;
    }
    return 0;
}

int moveTo(Queue sourceQ,Queue targetQ,int (*compare)(PCB *, PCB *))
{
    PCB * p;
    p = getFromQueue(sourceQ,compare);
    if(p != NULL)
    {
        addToQueue(targetQ,p);
        return 1;
    }else
        return 0;
}
