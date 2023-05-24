		lw	0	1	ten		
		lw	0	2	neg1
loop	add	1	3	3	
		add	1	2	1	
		beq	1	5	2	
		beq	0	0	start	error: label undefined
		noop
done	halt
ten	.fill	10
neg1	.fill	-1
