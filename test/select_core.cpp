#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>

int main(){
	fd_set rfds;
	struct timeval tv;
	int retval;
	
	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	/* Wait up to five seconds. */
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
	/* Don’t rely on the value of tv now! */
	printf("won't arrive here with FD_SETSIZE=%d\n", FD_SETSIZE);	
	if (retval == -1)
		perror("select()");
	else if (retval)
		printf("Data is available now.\n");
	/* FD_ISSET(0, &rfds) will be true. */
	else
		printf("No data within %ld  seconds.\n", tv.tv_sec);

	return 0;
}

