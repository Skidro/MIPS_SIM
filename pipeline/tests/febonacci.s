# Description
# This program calculates the nth febonacci number
# The c-algorithm and instruction-by-instruction
# translation of the code can be seen below
#
# Usage:
# In order to calculate the nth febonacci, the value
# 'n' has to be programmed into R-1 (shown as k in 
# the illustration below. Once this is done, the
# resulting hex-code can be fed to the mips processor
# and the output i.e. the nth febonnaci number can
# be seen in the register R-6
#
# Example:
# Here are the starting numbers of febonacci sequence:
# 1,1,2,3,5,8,13,21,34,55,..
# 1,2,3,4,5,6,7 ,8 ,9 ,10,..
# In order to calculate the 10th febonacci number, the 
# first instruction below should be changed in the 
# following way:
# Assembly		  || C	     || Machine
# addi	$1, $zero, 0x000a || nn = 10 || 0x2001000a
# Once the resulting hex file has been executed, the
# register file should contain the following:
# R6: 0x00000037 (	55)
#
# Author
# Waqar Ali	(wali@ku.edu)


# Conceptual C Algorithm
n1 = 1;
n2 = 1;

nx = 1;
nn = k;

while (nn > 2)
    nx = n2 + n1;
    n1 = n2;
    n2 = nx;
    nn = nn - 1;

return nx;


# Assembly Code			||   Equivalent C Code	|| Machine Code
addi	$1, $zero, 0x000k	||	nn = k		|| 0x2001kkkk
addi    $2, $zero, 0x0001       ||	c1 = 1 (Const)	|| 0x20020001
addi    $3, $zero, 0x0002       ||	c2 = 2 (Const)	|| 0x20030002
addi    $4, $zero, 0x0001	||	n1 = 1		|| 0x20040001
addi    $5, $zero, 0x0001       ||	n2 = 1		|| 0x20050001
addi    $6, $zero, 0x0001	||	nx = 1		|| 0x20060001
loop:				||			||
beq     $1, $3, end		||	while (nn > c2)	|| 0x10230005
add     $6, $5, $4		||	nx = n2 + n1	|| 0x00a02020
add	$4, $5, $zero		||	n1 = n2		|| 0x00c02820
add     $5, $6, $zero		||	n2 = nx		|| 0x00220822
sub     $1, $1, $2		||	nn = nn - c1	|| 0x08000006
j	loop			||			||
end:				||			||
syscall				||	return		|| 0x0000000c

