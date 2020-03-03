#ifndef mTcpDriver_h
#define mTcpDriver_h

typedef struct
{
    void (*serverInit)(void);
    void (*serverLoop)(void);
    void (*serverSendData)(unsigned char*);
    unsigned char (*serverCheckRecv)(unsigned char*);
    unsigned char (*serverCheckConnect)(void);
    
}mTcpDriverManage_t;
extern mTcpDriverManage_t mTcp;
#endif
