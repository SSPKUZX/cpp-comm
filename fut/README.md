this future implementation is based on the revised version of ananas/future
which is based on folly's future.

the following works:
	1) test whenx && timeout	
		1.1) timeout and future should be executed in different threads, or
		timeout will not work;
		1.2) whenx 
	2) refactor future.h --> DONE	
		


