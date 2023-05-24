		lw	0	2	ten
		lw	0	3	neg2
start	add	2	3	2
		beq	2	1	2
		beq	0	0	start
		noop
start	halt				error: duplicated label
ten		.fill	10
neg2	.fill	-2
