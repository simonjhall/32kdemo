.global exception_save
exception_save:
	/* a0 - a6 */
	.fill 7, 4, 0

	/* SS/US */
	.fill 2, 4, 0

	/* d0 - d7 */
	.fill 8, 4, 0
	

.global _addr_error
_addr_error:
	/* a0, a1 */
	move.l %a0, exception_save
	move.l %a1, exception_save+4
	/* destination vector */
	move.l #addr_error, %a1

	jmp generic

.global _illegal
_illegal:
	/* a0, a1 */
	move.l %a0, exception_save
	move.l %a1, exception_save+4
	/* destination vector */
	move.l #illegal, %a1

	jmp generic

.global _zero_divide
_zero_divide:
/* a0 */
	/* a0, a1 */
	move.l %a0, exception_save
	move.l %a1, exception_save+4
	/* destination vector */
	move.l #zero_divide, %a1

	jmp generic

generic:
	/* a2 - a6 */
	movea.l #(exception_save + 8), %a0
	movem.l %a2-%a6, (%a0)

	/* ss */
	add.l #(5 * 4), %a0
	move.l %a7, (%a0)

	/* us */
	move %usp, %a7
	move.l %a7, (4, %a0)
	/* get the system stack back */
	move.l (%a0), %a7

	/* d0 - d7 */
	addq.l #8, %a0
	movem.l %d0-%d7, (%a0)

	jmp (%a1)
