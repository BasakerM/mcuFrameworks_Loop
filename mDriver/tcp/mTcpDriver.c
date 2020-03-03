#include "mTcpDriver.h"
// #include "m_apiConfig.h"

#include "stdio.h"
#include "string.h"

#include "tcp_server_demo.h"

void _m_tcp_server_init(void)
{
	err_t err;  
	struct tcp_pcb *tcppcbnew;
	struct tcp_pcb *tcppcbconn;

    // #ifdef M_USING_CONSOLE
    //     mConsole.printf("\ninit tcp");
    // #endif
    while(lwip_comm_init() != 0);
    // #ifdef M_USING_CONSOLE
    //     mConsole.printf("\ninit success");
	//     mConsole.printf("\ncreat tcp");
    // #endif
	tcppcbnew=tcp_new(); //创建
    if(tcppcbnew)
	{
        // #ifdef M_USING_CONSOLE
		//     mConsole.printf(" : creat success\nbind tcp");
        // #endif
		err=tcp_bind(tcppcbnew,IP_ADDR_ANY,TCP_SERVER_PORT); //绑定
		if(err==ERR_OK)
		{
            // #ifdef M_USING_CONSOLE
            //     mConsole.printf(" : bind success\nlisten tcp");
            // #endif
			tcppcbconn=tcp_listen(tcppcbnew); //监听
            // #ifdef M_USING_CONSOLE
			//     mConsole.printf("\naccept tcp");
            // #endif
			tcp_accept(tcppcbconn,tcp_server_accept); //accept
		}
        else
        {
            // #ifdef M_USING_CONSOLE
            //     mConsole.printf("\nbind tcp fail");
            // #endif
        }
	}
    else
    {
        // #ifdef M_USING_CONSOLE
        //     mConsole.printf("\ncreat tcp fail");
        // #endif
    }
}

void _m_tcp_server_loop(void)
{
    lwip_periodic_handle();
}

void _m_tcp_server_sendData(unsigned char* buff)
{
    
    // #ifdef M_USING_CONSOLE
    //     mConsole.printf("\nsend Data: ");
    //     mConsole.sendStr((char*)buff);
    // #endif
    // if((tcp_server_flag & 1<<7)) { return; }
    sprintf((char*)tcp_server_sendbuf,(char*)buff); // 给出数据
    tcp_server_flag|=1<<7; //标记要发送数据
//    while (tcp_server_flag & 1<<7)
//    {
//        lwip_periodic_handle();
//    }
    
}

unsigned char _m_tcp_server_checkRecv(unsigned char* buff)
{
    if(tcp_server_flag&1<<6) //是否收到数据
    {
        // #ifdef M_USING_CONSOLE
        //     mConsole.printf("\nReceive Data: ");
        //     mConsole.sendStr((char*)tcp_server_recvbuf);
        // #endif
        sprintf((char*)buff,(char*)tcp_server_recvbuf); // 给出数据
        tcp_server_flag&=~(1<<6); //标记数据已经被处理
        return strlen((char*)buff); // 返回数据长度
    }
    return 0;
}

unsigned char _m_tcp_server_connflag = 0; // 连接标记
/*
*   
*/
unsigned char _m_tcp_server_checkConnect(void)
{
    if(tcp_server_flag&1<<5) //是否连接
    {
        if(_m_tcp_server_connflag==0)
        {
            // #ifdef M_USING_CONSOLE
            //     mConsole.printf("\nClient IP:%d.%d.%d.%d",lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);
            // #endif
            _m_tcp_server_connflag=1; //标记连接了
        }
    }
    else if(_m_tcp_server_connflag)
    {
        // #ifdef M_USING_CONSOLE
        //     mConsole.printf("\nClient disconnect");
        // #endif
        _m_tcp_server_connflag=0; //标记连接断开了
    }
    return _m_tcp_server_connflag;
}

mTcpDriverManage_t mTcp = {_m_tcp_server_init, _m_tcp_server_loop, _m_tcp_server_sendData, _m_tcp_server_checkRecv, _m_tcp_server_checkConnect};
