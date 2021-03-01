SCB_VTOR	    EQU	    0xE000ED08
NVIC_INT_CTRL   EQU     0xE000ED04
NVIC_SYSPRI2    EQU     0xE000ED20
NVIC_PENDSV_PRI EQU     0xFFFF0000
NVIC_PENDSVSET  EQU     0x10000000

    AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB
    REQUIRE8
    PRESERVE8

	IMPORT xd_task_switch_interrupt_flag
	IMPORT xd_interrupt_from_task
	IMPORT xd_interrupt_to_task

xd_interrupt_disable	PROC
	EXPORT	xd_interrupt_disable
	MRS		r0,	PRIMASK
	CPSID	I
	BX		LR
	ENDP

xd_interrupt_enable	PROC
	EXPORT	xd_interrupt_enable
	MSR		PRIMASK,	r0
	BX		LR
	ENDP

xd_context_switch	PROC
	EXPORT	xd_context_switch
	LDR	r2,	=xd_task_switch_interrupt_flag
	LDR	r3,	[r2]
	CMP r3,	#1
	BEQ	_reswitch
	MOV	r3,	#1
	STR	r3,	[r2]
	LDR	r2,	=xd_interrupt_from_task
	STR	r0,	[r2]

_reswitch
	LDR	r2,	=xd_interrupt_to_task
	STR	r1,	[r2]

	LDR	r0,	=NVIC_INT_CTRL
	LDR	r1,	=NVIC_PENDSVSET
	STR	r1,	[r0]
	BX	LR
	ENDP

PendSV_Handler  PROC
    EXPORT PendSV_Handler
    MRS r2, PRIMASK
    CPSID   I

    LDR r0, =xd_task_switch_interrupt_flag
    LDR r1, [r0]
    CBZ r1, pendsv_exit

    MOV r1, #0x00
    STR r1, [r0]

    LDR r0, =xd_interrupt_from_task
    LDR r1, [r0]
    CBZ r1, switch_to_task

    MRS r1, psp

    IF      {FPU} != "SoftVFP"
    TST     lr, #0x10               ; if(!EXC_RETURN[4])
    VSTMFDEQ  r1!, {d8 - d15}       ; push FPU register s16~s31
    ENDIF

    STMFD r1!,  {r4 - r11}

    IF      {FPU} != "SoftVFP"
    MOV     r4, #0x00               ; flag = 0

    TST     lr, #0x10               ; if(!EXC_RETURN[4])
    MOVEQ   r4, #0x01               ; flag = 1

    STMFD   r1!, {r4}               ; push flag
    ENDIF

    LDR r0, [r0]
    STR r1, [r0]

switch_to_task
    LDR r1, =xd_interrupt_to_task
    LDR r1, [r1]
    LDR r1, [r1]

    IF      {FPU} != "SoftVFP"
    LDMFD   r1!, {r3}               ; pop flag
    ENDIF

    LDMFD r1!,  {r4 - r11}

    IF      {FPU} != "SoftVFP"
    CMP     r3,  #0                 ; if(flag_r3 != 0)
    VLDMFDNE  r1!, {d8 - d15}       ; pop FPU register s16~s31
    ENDIF

    MSR psp, r1

    IF      {FPU} != "SoftVFP"
    ORR     lr, lr, #0x10           ; lr |=  (1 << 4), clean FPCA.
    CMP     r3,  #0                 ; if(flag_r3 != 0)
    BICNE   lr, lr, #0x10           ; lr &= ~(1 << 4), set FPCA.
    ENDIF

pendsv_exit
    MSR PRIMASK, r2
    ORR lr, lr, #0x04
    BX	lr
    ENDP

xd_context_switch_to	PROC
	EXPORT  xd_context_switch_to
    LDR     r1, =xd_interrupt_to_task
    STR     r0, [r1]

    IF      {FPU} != "SoftVFP"
    ; CLEAR CONTROL.FPCA
    MRS     r2, CONTROL             ; read
    BIC     r2, #0x04               ; modify
    MSR     CONTROL, r2             ; write-back
    ENDIF

    LDR     r1, =xd_interrupt_from_task
    MOV     r0, #0x0
    STR     r0, [r1]

    LDR     r1, =xd_task_switch_interrupt_flag
    MOV     r0, #1
    STR     r0, [r1]

    LDR     r0, =NVIC_SYSPRI2
    LDR     r1, =NVIC_PENDSV_PRI
    LDR.W   r2, [r0, #0x00]
    ORR     r1,r1,r2
    STR     r1, [r0]

    LDR     r0, =NVIC_INT_CTRL
    LDR     r1, =NVIC_PENDSVSET
    STR     r1, [r0]

	LDR		r0,	=SCB_VTOR
	LDR		r0,	[r0]
	LDR		r0,	[r0]
	MSR		msp, r0

    CPSIE   F
    CPSIE   I

    ENDP

xd_interrupt_task_switch PROC
    EXPORT xd_interrupt_task_switch
    BX      lr
    ENDP


    ALIGN   4

    END
