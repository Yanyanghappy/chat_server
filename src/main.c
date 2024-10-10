#include "server.h"
#include "client.h"
#include "public.h"



/**
 * @brief Multi threaded listening and processing of data from serial port
 * @return int 0 is success -1 is failure
 */

int main(int argc, char **argv)
{
    printf("server Usage: ./mes <port>\n");
    printf("client Usage: ./mes <ip> <port> <buffer>\n");
    if(argc == 4) {
        char *ip = argv[1];
        int port = atoi(argv[2]);
        char *buf = argv[3];
        printf("ip is %s\nbuf is %s\n", ip, buf);

        if(client_connect(ip, port, buf)) {
            printf("client failed to start\n");
            return -1;
        }
    }
    else if(argc == 2) {
        int port = atoi(argv[1]);
        if(server_listen(port)) {
            printf("Server failed to start\n");
            return -1;
        }
    }
    return 0;
}