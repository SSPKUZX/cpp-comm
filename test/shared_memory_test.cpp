#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

#define KEY          1234
#define SIZE         1024

int main()
{
	int shmid;
	char *shmaddr;
	struct shmid_ds buf;
	shmid = shmget(KEY, SIZE, IPC_CREAT | 0600);    /*建立共享内存*/

	if (fork() == 0)
	{
		shmaddr = (char *) shmat(shmid, 0, 0);
		strcpy(shmaddr, "Hi! I am Child process!\n");
		printf("Child:   write to shared memery: Hi! I am Child process!\n");
		shmdt(shmaddr);

		return 0;
	}
	else
	{
		sleep(3);      /*等待子进程执行完毕*/
		shmctl(shmid, IPC_STAT, &buf);          /*取得共享内存的状态*/
		printf("shm_segsz = %lu bytes\t", buf.shm_segsz);
		printf("shm_cpid = %d\t", buf.shm_cpid);
		printf("shm_lpid = %d\t", buf.shm_lpid);

		shmaddr = (char*) shmat(shmid, 0, SHM_RDONLY);
		printf("Father:   %s\n", shmaddr);        /*显示共享内存内容*/
		shmdt(shmaddr);
		shmctl(shmid, IPC_RMID, NULL);         /*删除共享内存*/

		return 0;
	}
	
}
