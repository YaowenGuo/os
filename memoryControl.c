#define WORD_SIZE 32 //计算机字长
#define W_BIT 12   //用于表示页内偏移地址的位数
#define P_BIT (WORD_SIZE-W_BIT)/2  //用于表示页号的位数
#define S_BIT WORD_SIZE-W_BIT - P_BIT //用于表示段号的位数
#define PAGE_LENGTH 512
typedef int * Segment;
typedef short * Packge;
typedef int LogicalAddress;
typedef short Offset,SegmentNum,PageNum;//页内地址，段号，页号
typedef struct Register
{
    short segmentLength;
    Segment segmentAddress;
}* SegTableRegister;//段表寄存器
typedef char* Memory;
//将逻辑地址转化成，物理地址
int  changeTo( LogicalAddress logical,SegTableRegister segRegister,Memory memory)
{
    Offset offset = logical && 0xfff;
    PageNum pageNum = logical>>W_BIT && 0x3ff;
    SegmentNum segmentNum = logical>>(W_BIT+P_BIT) && 0x3ff;
    
    if(segmentNum <= segRegister->segmentLength;)
    {
        Segment segmentItem = (SegRegister->segmentAddress)[segmentNum];// + segmentNum*sizeof(SegmentItem);
	if(pageNum <= SegmentItem->pageLength)
	{
	    short blockNum = memory[segmentItem->pageNum * PAGE_LENGTH + pageNum];
	    return blockNum * PAGE_LENGTH + offset;//返回物理地址
	}else exit(0);//页超界中断 
    }else exit(0);//段超界中断
}
//产生一张位示图，表示内存的使用情况        
int * getMemoryMap()
{
}
//根据作业产生一张段表，
SegmentTable getSegmentTable()
{
//输入该程序一共有几个段，产生相对应项的段表

}
//产生一张页表
PageTable getPageTable()
{
//,输入分配给本作业的块数，根据位示图查找空闲的块分配给该作业，在页表中标记逻辑页与页框的对应关系
}
//OPT，FIFO，LRU置换算法，利用堆栈完成页面置换
int OPT()
{
}

//将结果写到文件中
int writeToFile()
{
}

