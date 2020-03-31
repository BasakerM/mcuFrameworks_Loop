#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H 
#include "lan8720.h" 

#define LWIP_MAX_DHCP_TRIES		4   //DHCP������������Դ���
   
//lwip���ƽṹ��
typedef struct  
{
	unsigned char mac[6];      //MAC��ַ
	unsigned char remoteip[4];	//Զ������IP��ַ 
	unsigned char ip[4];       //����IP��ַ
	unsigned char netmask[4]; 	//��������
	unsigned char gateway[4]; 	//Ĭ�����ص�IP��ַ
	
	volatile unsigned char dhcpstatus;	//dhcp״̬ 
					//0,δ��ȡDHCP��ַ;
					//1,����DHCP��ȡ״̬
					//2,�ɹ���ȡDHCP��ַ
					//0XFF,��ȡʧ��.
}__lwip_dev;
extern __lwip_dev lwipdev;	//lwip���ƽṹ��

void lwip_pkt_handle(void);
void lwip_periodic_handle(void);
	
void lwip_comm_default_ip_set(__lwip_dev *lwipx);
unsigned char lwip_comm_mem_malloc(void);
void lwip_comm_mem_free(void);
unsigned char lwip_comm_init(void);
void lwip_dhcp_process_handle(void);

#endif













