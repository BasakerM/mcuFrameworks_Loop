#ifndef _fifo_h
#define _fifo_h

typedef struct
{
    unsigned char* buf; // 接收队列
    unsigned short size; // 缓冲区大小
    unsigned char head; // 队列头,是下一个将要读取的位置,现在还未读取
    unsigned char rear; // 队列尾,是下一个将要写入的位置,现在还未写入
}fifo_t;

// typedef struct
// {
//     /*
//         说明：
//             初始化队列
//         参数：
//             fifo_t* fifo：队列指针
//             unsigned char* buf：队列的缓冲区
//             unsigned short size：缓冲区大小
//     */
//     void (*init)(fifo_t* fifo,unsigned char* buf,unsigned short size);

//     /*
//         说明：
//             从队列读取数据
//         参数：
//             fifo_t* fifo：队列指针，将从该队列中读取
//             unsigned char* buf：存数据的缓冲区，从队列读出的数据将被存放这该缓冲区中
//             unsigned short len：希望读取的数据长度
//         返回：
//             unsigned short：实际读取的数据长度
//     */
//     unsigned short (*read)(fifo_t* fifo, unsigned char* buf, unsigned short len);

//     /*
//         说明：
//             往队列写入数据
//         参数：
//             fifo_t* fifo：队列指针，将数据写入该队列中
//             unsigned char* buf：存数据的缓冲区，将该缓冲区的数据写入队列
//             unsigned short len：希望写入的数据长度
//         返回：
//             unsigned short：实际写入的数据长度
//     */
//     unsigned short (*write)(fifo_t* fifo, unsigned char* buf, unsigned short len);

//     /*
//         说明：
//             获取可写长度
//         参数：
//             fifo_t* fifo：队列指针
//         返回：
//             unsigned short：队列剩余的可用长度
//     */
//     unsigned short (*getWriteableLen)(fifo_t* fifo);

//     /*
//         说明：
//             获取可读长度
//         参数：
//             fifo_t* fifo：队列指针
//         返回：
//             unsigned short：队列剩余的可用长度
//     */
//     unsigned short (*getReadableLen)(fifo_t* fifo);

//     /*
//         说明：
//             重置队列，也就是清空队列
//         参数：
//             fifo_t* fifo：队列指针
//         返回：
//             void
//     */
//     void (*reset)(fifo_t* fifo);
// }_fifoManage_t;

// extern _fifoManage_t fifo;


#define _fifo_computeHead(a) ((a->head + 1) % a->size)
#define _fifo_computeRear(a) ((a->rear + 1) % a->size)
#define _fifo_isNotFull(a) (_fifo_computeRear(a) != a->head)
#define _fifo_isNotEmpty(a) (a->head != a->rear)

/*
    说明：
        初始化队列
    参数：
        fifo_t* fifo：队列指针
        unsigned char* buf：队列的缓冲区
        unsigned short size：缓冲区大小
*/
static inline void fifo_init(fifo_t* fifo,unsigned char* buf,unsigned short size)
{
    fifo->buf = buf;
    fifo->size = size;
    fifo->head = 0;
    fifo->rear = 0;
}

/*
    说明：
        从队列读取数据
    参数：
        fifo_t* fifo：队列指针，将从该队列中读取
        unsigned char* buf：存数据的缓冲区，从队列读出的数据将被存放这该缓冲区中
        unsigned char len：希望读取的数据长度
    返回：
        unsigned short：实际读取的数据长度
*/
static inline unsigned short fifo_read(fifo_t* fifo, unsigned char* buf, unsigned short len)
{
    unsigned short readLenInFact = 0;

    while(_fifo_isNotEmpty(fifo) && readLenInFact != len) // 队列非空，且未读取到希望读取的数据长度，才能继续读
    {
        buf[readLenInFact] = fifo->buf[fifo->head]; // 读取数据
        fifo->head = _fifo_computeHead(fifo); // 计算队列头
        ++readLenInFact;
    }
    return readLenInFact;
}

/*
    说明：
        从队列读取数据
    参数：
        fifo_t* fifo：队列指针，将数据写入该队列中
        unsigned char* buf：存数据的缓冲区，将该缓冲区的数据写入队列
        unsigned short len：希望写入的数据长度
    返回：
        unsigned short：实际写入的数据长度
*/
static inline unsigned short fifo_write(fifo_t* fifo, unsigned char* buf, unsigned short len)
{
    unsigned short writeLenInFacr = 0;
    while(_fifo_isNotFull(fifo) && writeLenInFacr != len) // 队列未满，且未写入希望写入的数据长度，才能继续写
    {
        fifo->buf[fifo->rear] = buf[writeLenInFacr]; // 写入数据
        fifo->rear = _fifo_computeRear(fifo); // 计算队列尾
        ++writeLenInFacr;
    }
    return writeLenInFacr;
}

/*
    说明：
        获取队列剩余空间
    参数：
        fifo_t* fifo：队列指针
    返回：
        unsigned short：队列剩余的可用长度
*/
static inline unsigned short fifo_getWriteableLen(fifo_t* fifo)
{
    unsigned short head = fifo->head;
    unsigned short rear = fifo->rear;
    
    return (head <= rear)? (fifo->size - (rear - head) - 1):(head - rear - 1);
}

/*
    说明：
        获取可读长度
    参数：
        fifo_t* fifo：队列指针
    返回：
        unsigned short：队列剩余的可用长度
*/
static inline unsigned short fifo_getReadableLen(fifo_t* fifo)
{
    unsigned short head = fifo->head;
    unsigned short rear = fifo->rear;
    
    return (head <= rear)? (rear - head):(fifo->size - (head - rear));
}

/*
    说明：
        重置队列，也就是清空队列
    参数：
        fifo_t* fifo：队列指针
    返回：
        void
*/
static inline void fifo_reset(fifo_t* fifo)
{
    unsigned char* buf = fifo->buf;
    unsigned short size = fifo->size;
    while(size--)
    {
        buf[size] = 0;
    }
    fifo->head = 0;
    fifo->rear = 0;
}

#endif
