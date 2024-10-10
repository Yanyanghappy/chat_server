#ifndef SERVER_H
#define SERVER_H

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
#include <poll.h>

#include "public.h"
#include "map.h"

/**
 * @brief achieve receive the client msg through TCP/IP
 * @param size is the size of the client msg 
 * @param port is the port of listen
 * @return int 0 success -1 failed
 * @author rpy <rpyyanyang@163.com>
 */

int server_listen(int port);





#endif