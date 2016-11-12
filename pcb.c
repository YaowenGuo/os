/*************************************************************************
	> File Name: pcb.c
	> Author: 
	> Mail: 
	> Created Time: Fri 18 Sep 2015 08:06:40 AM
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"pcb.h"
long currentTime = 0;//给全局变量赋初值
//创建一个新的队列，
//返回空队列
Queue newQueue()
{
    Queue q = ( Queue )malloc(sizeof(PCB));//申请一个空头
    q->next = NULL;
    return q;
}
//创建一个新的PCB，并初始化
//返回节点指针
Queue createPCB()
{
    PCB * newPcb = (PCB *)malloc(sizeof(PCB));//申请一个新PCB
    newPcb->next = NULL;
    printf("Please input PCB name:");
    scanf(" %s",newPcb->name);//命名
    printf("Please input start time:");
    scanf(" %ld",&(newPcb->startTime));//开始时间
    printf("Please input serve time:");
    scanf(" %d",&(newPcb->serveTime));//服务时间
    newPcb->useTime = newPcb->serveTime;//刚创建时，剩余时间等于服务时间

    return newPcb;
}
//拷贝一个队列
//head  指向一个原有队列
//返回一个新的队列
Queue copyQueue(Queue head)
{
    Queue createQueue = newQueue()/*创建一个空队列，用于添加拷贝的节点*/, newNode;
    while(head->next)
    {
        head = head->next;
        newNode = (PCB *)malloc(sizeof(PCB));
        *newNode = * head;//结构体整体赋值
        addToQueue(createQueue,newNode);//添加到新队列
    }
    return createQueue;
}
//释放PCB
int freePCB(PCB * pcb)
{
    free(pcb);
    return 1;
}
//输出成个队列
//head  要输出的队列
//name  用于输出时显示的该队列的名字
void showQueue( Queue head ,char *name)
{
    printf("Current %s queue list is:\n",name);
    while( head->next )
    {
        head = head->next;
	//分别输出  名字    开始时间    服务时间
        printf("pcb: %s,start Time:%ld, serveTime: %d\n",head->name,head->startTime,
                head->serveTime);
    }
}

//短作业有先比较，
//p1的服务时间小于p2返回真
int sjf(PCB * p1, PCB * p2)
{
    return p1->useTime < p2->useTime;
}
//优先级比较
//p1的优先级比p2的优先级高返回真
int priority(PCB * p1, PCB * p2)
{ 
//高响应比优先的计算公式计算优先级：    优先级=（等待时间+服务时间）/服务时间
    return (currentTime - p1->startTime + p1->useTime)/p1->useTime > 
        (currentTime - p2->useTime + p2->useTime)/p2->useTime;
}
/*
 *从队列中取得一个节点,如果compare 是段作业比较[int sjf(PCB * p1, PCB * p2)]，就挨个查找作业最短的
  如果compare是优先级比较【int priority(PCB * p1, PCB * p2)】，就查找优先级最高的
  compare传NULL 就取队列第一个，
队列为空就返回NULL
//head 指向队列
//compare为一个参数为两个PCB节点，返回值为 真假的函数指针，可以传 上面的两个比较算法
//返回获取到的PCB 节点
*/
PCB * getFromQueue(Queue head, int (*compare)(PCB* pcb1,PCB* pcb2))
{
    PCB * p = head,* get,*priorGet;
    if(head->next )//队列不为空时get指向第一个PCB，priorGet在指向get的前一个，用于将get节点从链表取出来
    {
        get = head->next;
        priorGet = head;
        p = get;
    }else
        return NULL;//空队列就直接返回
    if( compare )//如果有比较函数的话，就
    {
        while(p->next)
        {
            if((*compare)(p->next, get))//如果是优先级比较，p->next的优先级高返回真，则get指向p->next。
		//如果短作业，p->next的作业比get还要短，get指向p->next
            {
                get = p->next;
                priorGet = p;
            }
            p = p->next;
        }
    }
    //以下两行将获取的节点取出，
    priorGet->next = get->next;
    get->next = NULL;
    return get;
        
}
/*
 *将一个节点存入队列中，放在队列的尾部，成功返回真，失败返回假
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

//根据给出的比较算法（sjf  或 priority）从sourceQ队列中取一个PCB，放到targetQ队列中
//compare给NULL的话，直接去队列首节点
int moveTo(Queue sourceQ,Queue targetQ,int (*compare)(PCB *, PCB *))
{
    PCB * p;
    p = getFromQueue(sourceQ,compare);//调用函数取
    if(p != NULL)
    {
        addToQueue(targetQ,p);//添加到队尾
        return 1;
    }else
        return 0;
}
