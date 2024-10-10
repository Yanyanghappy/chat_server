#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>          
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>

#include "public.h"
#include "map.h"


/**
 * @brief achieve connect the server through TCP/IP and poll
 * @param ip is the ip(DEC) of server 
 * @param port is the port of connect
 * @param buf is the msg of transfer
 * @return int 0 success -1 failed
 * @author rpy <rpyyanyang@163.com>
 */

int client_connect(char* ip, int port, char * buf);





#endif