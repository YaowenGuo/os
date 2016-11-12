typedef struct SegmentItem
{
    short pageSize;
    short pageNum;
}
//0~15为页号，16-30位为页表大小，31位为状态位
typedef short * Page；
//0~13位为页号，14,15位为状态位

//定义段表
Segment segmentItem;

//定义页表
Page pageItem;


