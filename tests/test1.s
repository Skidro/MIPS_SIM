addiu $2, $zero, 1024		/* 0x24020400 */
repeat:				
addiu $3, $3, 256		/* 0x24630100 */
beq   $3, $2, continue		/* 0x10620001 */
j     repeat			/* 0x08000001 */
continue:
lui   $4, 0x1000		/* 0x3c041000 */
sw    $3, 50($4)		/* 0xac830032 */
lw    $5, 50($4)		/* 0x8c850032 */
syscall				/* 0xc0000000 */
