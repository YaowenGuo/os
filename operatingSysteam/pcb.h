/*************************************************************************
	> File Name: pcb.h
	> Author: 
	> Mail: 
	> Created Time: Tue 22 Sep 2015 10:50:56 AM
 ************************************************************************/

#ifndef _PCB_H
#define _PCB_H
#define PCBSTART 500
extern long currentTime;
typedef struct PCBNode
{
    unsigned int ID;
    char name[20];
    unsigned long startTime;
    unsigned int serveTime;
    unsigned int useTime;
    unsigned int processCounter;
    unsigned int registers[16];
    struct PCBNode * next;
}PCB;
typedef PCB * Queue;
typedef char Data;
/*创建一个空的队列*/
Queue newQueue();
/*拷贝一个队列，返回新的队列*/
Queue copyQueue(Queue head);
/*创建一个新的PCB节点,并添加到队列里*/
Queue createPCB(int * pcbID,int pcbNum);
/*比较算法，短作业优先
 *p1作业小于p2返回真，否则返回假
 */
int sjf(PCB * p1,PCB * p2);
/*
 * 比较算法，优先级
 * p1优先级高返回真，否则返回假
 */
int priority(PCB *p1, PCB *p2);
/*打印输出队列的所有元素*/
void showQueue( Queue head ,char *);
/*清空队列*/
void clearQueue( Queue head, int id);
/*从队列中返回一个节点，对列为空就返回NULL*/
PCB * getFromQueue(Queue head ,int (*compare)(PCB *,PCB *));
/*将一个节点存入到队列中*/
int  addToQueue(Queue head, PCB * node);
/*从sourceQ中取一个PCB存入到targetQ中，成功返回true，失败返回false
 * 认为源队列为空也无法存取成功
 */
int moveTo(Queue sourceQ, Queue targetQ,int (*compare)(PCB *,PCB *));
#endif
