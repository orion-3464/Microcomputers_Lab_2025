.include "m328PBdef.inc"
.def temp=r16
    
init_stack:
    ldi temp, low(RAMEND)	; initialize stack pointer
    out SPL, temp
    ldi temp, high(RAMEND)
    out SPH, temp

init_ports:
    

