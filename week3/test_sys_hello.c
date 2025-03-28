#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
	long int i = syscall(436);
	printf("SYSCALL::SYS_HELO::RETVAL=%ld\n", i);
	return 0;
}
