
SCB_VTOR        EQU     0xE000ED08               ; Vector Table Offset Register
NVIC_INT_CTRL   EQU     0xE000ED04               ; interrupt control state register
NVIC_SYSPRI2    EQU     0xE000ED20               ; system priority register (2)
NVIC_PENDSV_PRI EQU     0x00FF0000               ; PendSV priority value (lowest)
NVIC_PENDSVSET  EQU     0x10000000               ; value to trigger PendSV exception

	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8

enterCriticality PROC
		EXPORT enterCriticality
		MRS r0, PRIMASK
		CPSID I
		BX LR
		ENDP

exitCriticality PROC
		EXPORT exitCriticality
		MSR PRIMASK, r0
		BX LR
		ENDP
	
	ALIGN 4
		
	END