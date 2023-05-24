		lw	0	1	ten
		lw	0	2	neg1
loop	add	1	3	3
		add	1	2	1
		beq	0	1	2
		beq	0	0	loop
		noop
done	halt
ten	.fill	10
neg1	.fill	-1