		lw	0	1	ten	load reg[1] with 10
		lw	0	2	neg1	load reg[2] with -1
loop	add	1	3	3	reg[3] += reg[1]
		add	1	2	1	reg[1] -= 1
		beq	1	5	2	if reg[1] == 0, jump to noop
		beq	0	0	loop	else jump to loop
		noop
done	halt
ten	.fill	10
neg1	.fill	-1
