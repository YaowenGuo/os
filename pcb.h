/*************************************************************************
	> File Name: pcb.h
	> Author: 
	> Mail: 
	> Created Time: Tue 22 Sep 2015 10:50:56 AM
 ************************************************************************/

#ifndef _PCB_H
#define _PCB_H
extern long currentTime;//定义一个相对时间，从0开始，逐渐增加
typedef struct PCBNode
{
    char name[20];
    unsigned long startTime;//到达时间，
    unsigned int serveTime;//服务时间即运行时间，完成该pcb需要的时间
    unsigned int useTime;//剩余时间，用于时间片轮转时，轮转几次后还有多少时间完成该pcb
    struct PCBNode * next;
}PCB;
typedef PCB * Queue;//定义一个队列别名，
//typedef char Data;
/*创建一个空的队列*/
Queue newQueue();
/*拷贝一个队列，返回新的队列*/
Queue copyQueue(Queue head);
/*创建一个新的PCB节点,并添加到队列里*/
Queue createPCB();
/*比较算法，短作业优先
 *p1 p2 均为一个PCB
 *p1作业小于p2返回真，否则返回假
 */
int sjf(PCB * p1,PCB * p2);
/*
 * 比较算法，优先级
 * p1优先级高返回真，否则返回假
 */
int priority(PCB *p1, PCB *p2);
/*打印输出队列的所有元素
*/
void showQueue( Queue head ,char *);
/*清空队列*/
void clearQueue( Queue head);
/*从队列中返回一个节点，对列为空就返回NULL*/
PCB * getFromQueue(Queue head ,int (*compare)(PCB *,PCB *));
/*将一个节点存入到队列中*/
int  addToQueue(Queue head, PCB * node);
/*从sourceQ中取一个PCB存入到targetQ中，成功返回true，失败返回false
 * 认为源队列为空也无法存取成功
 */
int moveTo(Queue sourceQ, Queue targetQ,int (*compare)(PCB *,PCB *));
#endif
