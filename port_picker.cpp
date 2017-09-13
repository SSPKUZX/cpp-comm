#include "port_picker.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include <sys/types.h>
#include <sys/socket.h>

namespace utl 
{
	
	uint32_t try_port( uint32_t const port ) 
	{
	    struct sockaddr_in addr;
		addr.sin_port = htons( port );  //port : [30150, 30199] 
		addr.sin_family = AF_INET;  // IPV4
		addr.sin_addr.s_addr = htonl(INADDR_ANY);  // set our addr to any interface
		
		int sock = socket(AF_INET, SOCK_STREAM, 0);
		if (0 != bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in))) {
			close(sock);
		   	return 0;
		}
		
		socklen_t addr_len = sizeof(struct sockaddr_in);
		if (0 != getsockname(sock, (struct sockaddr*)&addr, &addr_len)) {
			close(sock);
			return 0;
		}
		
		int ret_port = ntohs(addr.sin_port);
		close(sock);
	
		return ret_port;	
	}
	
} // end of utl 
