# Description
# This program tests all the instructions (except branch and jump)
# which were short-listed for implementation in this project. The
# expected output of this program is shown at the end of this file
#
# Author
# Waqar Ali	(wali@ku.edu)

addi      $1, $zero, 0x0001
add       $2, $1, $zero
addu      $3, $1, $zero
sub       $4, $1, $zero
subu      $4, $1, $zero
and       $1, $1, $zero
or        $5, $2, $zero
sll       $6, $2, 2
srl       $7, $6, 1
addiu     $8, $zero, 0x0001
andi      $9, $6, 0x0004
ori       $10, $6, 0x0002
lui       $11, 0x1000
sw        $10, 50($b)
lw        $12, 50($b)
slt       $13, $2, $12
sltu      $14, $3, $10
slti      $15, $12, 0x00ff
sltiu     $16, $10, 0x00ff
nor       $17, $10, $10
syscall

/* Expected Output */
# [00] : 0x00000000 | [01] : 0x00000000 | [02] : 0x00000001 | [03] : 0x00000001
# [04] : 0x00000001 | [05] : 0x00000001 | [06] : 0x00000004 | [07] : 0x00000002
# [08] : 0x00000001 | [09] : 0x00000004 | [10] : 0x00000006 | [11] : 0x10000000
# [12] : 0x00000006 | [13] : 0x00000001 | [14] : 0x00000001 | [15] : 0x00000001
# [16] : 0x00000001 | [17] : 0xfffffff9
#
# MEM[50] : 0x00000006
