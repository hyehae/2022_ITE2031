	lw	0	1	data1
	noop
	noop
	noop
	beq	0 0 done	branch hazard
	add 1 1 1
done halt
data1	.fill	10