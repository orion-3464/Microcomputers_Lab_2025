wait_msec:
    push r24 ; 2 cycles
    push r25 ; 2 cycles
    ldi r24 , low(999) ; 1 cycle
    ldi r25 , high(999) ; 1 cycle
    rcall wait_usec ; 998.375 usec
    pop r25 ; 2 cycles
    pop r24 ; 2 cycles
    nop ; 1 cycle
    nop ; 1 cycle
    sbiw r24 , 1 ; 2 cycles
    brne wait_msec ; 1 or 2 cycles
    ret ; 4 cycles


wait_usec:
    sbiw r24 ,1 ; 2 cycles (2/16 usec)
    call delay_8cycles ; 4+8=12 cycles
    brne wait_usec ; 1 or 2 cycles
    ret
delay_8cycles:
    nop
    nop
    nop
    nop

    ret
