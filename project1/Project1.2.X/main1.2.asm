.include "m328PBdef.inc"
.def F0 = r16
.def F1 = r17
.def A = r18
.def B = r19
.def C = r20
.def D = r21
.def counter = r22
.def temp = r23

main:
    ldi counter, 0x06
    ldi A,0x51
    ldi B,0x41
    ldi C,0x21
    ldi D,0x01
loop:
    rcall calculate_F0
    rcall calculate_F0
    
    out PORTD,F0
 
    out PORTD,F1

    inc A	;increasing A by 1
    
    ldi temp,0x02	
    add B,temp	;increasing B by 2

    ldi temp,0x03
    add C,temp	;increasing C by 3

    ldi temp,0x04
    add D,temp	;increasing D by 4

    dec counter
    brne loop	;if Z=0 --> counter>0
stop:
    rjmp stop	;end of the program

calculate_F0:
    mov F0,A
    or F0,D	;F0 = A+D
    mov temp,B
    com temp	;temp = B'
    and F0,temp	;F0 = (A+D)*B' = A*B' + D*B'
    com F0	;F0 = (A*B' + B'*D)'
    ret
    
calculate_F1:
    mov F1,A	;F1 = A
    mov temp,C	
    com temp	;temp = C'
    or F1,temp	;F1 = A+C'
    mov temp,D	
    com temp	;temp = D'
    or temp,B	;temp = B+D'
    and F1,temp	;F1 = (A+C')*(B+D')
    ret

end