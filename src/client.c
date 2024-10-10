#include "client.h"
#include <stdio.h>

int client_connect(char* ip, int port, char* buf)
{
    printf("client_connect......\n");
    ssize_t ret = 0;
    char read_buf[BUFFER_SIZE];
    char recv_buf[BUFFER_SIZE];

    /* create socket and connect the server */
    struct sockaddr_in client_addr;
    memset(&client_addr.sin_zero, 0, sizeof(client_addr.sin_zero));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &client_addr.sin_addr.s_addr);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(sockfd, (struct sockaddr *)&client_addr, 
                                                sizeof(client_addr))) {
        perror("client_connect");
        return -1;
    }
    else {
        setnonblocking(sockfd);
        printf("connect success!\n");
        ret = send(sockfd, buf, sizeof(buf), 0);
        if( ret < 0 ) {
            perror("send");
            return -1;
        }
        else {
            //printf("send success buf : %ld\n", ret);
        }
    }

    /* set the pipe */
    int pipefd[2];
    ret = pipe(pipefd);
    if (ret < 0) {
        perror("pipe");
        return -1;
    }

    struct pollfd fds[2];
    /* set data from standard input */
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    /* set data from socket */
    fds[1].fd = sockfd;
    fds[1].events = POLLIN;
    fds[1].revents = 0;

    while (1) {
        if (poll(fds, 2, -1) < 0) {
            perror("poll");
            break;
        }
        //printf("poll success!\n");
        if (fds[0].revents & POLLIN) {
            //printf("read from stdin and send for server\n");
            memset(read_buf, 0, BUFFER_SIZE);
            ret = splice(fds[0].fd,NULL,pipefd[1],NULL,32768,0); 
            ret = splice(pipefd[0],NULL,fds[1].fd,NULL,32768,0);
            // ret = read(fds[0].fd, read_buf, BUFFER_SIZE);
            // if(ret < 0)
            // {
            //     perror("stdin read");
            //     break;
            // }
            // ret = send(fds[1].fd, read_buf, BUFFER_SIZE, 0);
            // if(ret < 0)
            // {
            //     perror("stdin send to server");
            //     break;
            // }
        }
        if (fds[1].revents & POLLIN) {
            printf("%d :", fds[1].fd);
            fflush (stdout) ;
            memset(recv_buf, 0, BUFFER_SIZE);
            ret = recv(fds[1].fd, recv_buf, BUFFER_SIZE, 0);
            if (ret < 0){
                perror("recv from server");
                break;
            }
            else if (ret == 0) { 
                printf("server close the connection\n"); 
                break; 
            }
            //printf("read from socket and write in stdout:");
            if (write(1, recv_buf, BUFFER_SIZE) < 0) {
                perror("write in stdout");
                break;
            }
            printf("\n");
        }
    }

    close(sockfd);
	close(pipefd[0]);
	close(pipefd[1]);

    return 0;
}
