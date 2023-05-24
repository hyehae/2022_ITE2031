		lw	0	2	ten			
		lw	0	3	neg2
start	ad	2	3	2	error: unrecognized opcode
		beq	2	1	2	
		beq	0	0	start
		noop
done	halt
ten	.fill	10
neg2	.fill	-2
