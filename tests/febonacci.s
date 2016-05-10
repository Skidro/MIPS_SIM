1,1,2,3,5,8,13,21,34,55,..
1,2,3,4,5,6,7 ,8 ,9 ,10,..

n1 = 0;
n2 = 1;

nx = 1;
nn = k;

while (nn > 2)
    nx = n2 + n1;
    n1 = n2;
    n2 = nx;
    nn = nn - 1;

return nx;


addi	$1, $zero, 0x000k	nn
addi    $2, $zero, 0x0001       1
addi    $3, $zero, 0x0002       2
addi    $4, $zero, 0x0001	n1
addi    $5, $zero, 0x0001       n2
addi    $6, $zero, 0x0001	nx
loop:
beq     $1, $3, end		while (nn > 2)
add     $6, $5, $4		nx = n1 + n2
add	$4, $5, $zero		n1 = n2
add     $5, $6, $zero		n2 = nx
sub     $1, $1, $2		nn = nn - 1
j	loop			goto loop
end:
syscall				return

