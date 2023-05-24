		lw	0	2	ten	load reg[2] with 10 (symbolic)	
		lw	0	3	neg2	load reg[3] with -2 (symbolic)
start	add	2	3	2	reg[2] = reg[2] - reg[3]
		beq	2	1	2	if reg[2] == 0, jump to noop
		beq	0	0	start	else jump to start
		noop
done	halt
ten		.fill	10
neg2	.fill	-2
