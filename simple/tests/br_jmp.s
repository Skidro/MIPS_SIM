# Description
# This program verifies the basic branch and jump instructions
# The expected output of this program is shown at the end of 
# this file
#
# Author
# Waqar Ali 	(wali@ku.edu)

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

/* Expected Output */
# [00] : 0x00000000 | [01] : 0x00000000 | [02] : 0x00000400 | [03] : 0x00000400
# [04] : 0x10000000 | [05] : 0x00000400 | [06] : 0x00000000 | [07] : 0x00000000
# [08] : 0x00000000 | [09] : 0x00000000 | [10] : 0x00000000 | [11] : 0x00000000
# [12] : 0x00000000 | [13] : 0x00000000 | [14] : 0x00000000 | [15] : 0x00000000
# [16] : 0x00000000 | [17] : 0x00000000
#
# MEM[50] : 0x00000400
