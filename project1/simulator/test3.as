		lw	0	1	ten	load reg[1] with 10 (symbolic)
		lw	0	2	5	load reg[2] with 5 (numberic)
		add	0	1	5	store reg[5] with reg[1] value
		add	0	2	1	store reg[1] with reg[2] value
		add	0	5	2	store reg[2] with reg[5] value (original at reg[1])
		add	0	0	5	store reg[5] with 0 value
		noop
done	halt
ten	.fill	10
