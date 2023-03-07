#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <string.h>
int main()
{
	int fd;
	int len;
	char buf[64]={0};
	char buf2[64+1]="kamilu";

	fd = open("/dev/hellodev",O_RDWR);
	if(fd<0)
	{
		perror("open fail \n");
		return 1;
	}

	len = read(fd,buf,64);

	buf[len]='\0';

	printf("read:%s  len = %d\n",buf,len);
	len = write(fd,buf2,strlen(buf2));
	printf("len = %d\n",len);
	close(fd);
	return 0;
}