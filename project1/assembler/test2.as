		lw	0	1	five	load reg[1] with 5
		nor	1	1	2	reg[2] = reg[1] nor reg[1]
		add	1	2	3	reg[3] = reg[1] + reg[2]
		noop
done	halt
five	.fill	5
