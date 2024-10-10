#include "server.h"
#include "public.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>


int server_listen(int port)
{
    printf("server_listen......\n");

    int listen_fd;
    ssize_t ret = 0;
    struct Client_data{
        struct sockaddr_in addr;
        char* write_buf;
        char buffer[BUFFER_SIZE];
    };
    struct Client_data *client_data = malloc(sizeof(struct Client_data) *CLIENT_NUM);
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("listen socket creat failed");
        return -1;
    }
    struct sockaddr_in listen_fd_addr;
    memset(&listen_fd_addr, '\0', sizeof(listen_fd_addr));
    listen_fd_addr.sin_family = AF_INET;
    listen_fd_addr.sin_port = htons(port);
    listen_fd_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listen_fd, (struct sockaddr*) &listen_fd_addr, 
                                        sizeof(listen_fd_addr))) {
        printf("%d : ", port);
        perror("server bind failed");
        return -1;
    }
    
    if (listen(listen_fd, CLIENT_NUM)) {
        printf("%d : ", port);
        perror("server listen failed");
        return -1;
    }
    printf("Server is listening on port %d...\n", port);    

    struct pollfd fds[CLIENT_NUM + 1];
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    for (int i = 1; i <= CLIENT_NUM + 1; i++) {
        fds[i].fd = -1;
        fds[i].events = 0;
        fds[i].revents = 0;
    }
    int user_counter=0;
    
    while (1) {
        ret = poll(fds, CLIENT_NUM + 1, -1);
        if (ret < 0) {
            perror("server poll failed");
            break;
        }

        for(volatile int i = 0; i < CLIENT_NUM + 1; i++){
            /* new connection */
            if ((fds[i].fd == fds[0].fd) && (fds[i].revents & POLLIN)) {
                printf("server accept new connection\n");
                struct sockaddr_in client_addr;
                socklen_t client_addr_length = sizeof(client_addr);
                int accept_fd = accept(listen_fd, 
                                        (struct sockaddr *) &client_addr, 
                                                &client_addr_length);
                if(accept_fd < 0){
                    perror("server accept failed");
                    continue;
                }
                if(user_counter > CLIENT_NUM){
                    printf("Too many users\n");
                    char* info = "Too many users\n";
                    send(accept_fd,info,strlen(info),0);
                    close(accept_fd);
                    continue;
                }
                user_counter++;
                client_data[accept_fd].addr = client_addr;
                fds[user_counter].fd = accept_fd;
                fds[user_counter].events = POLLIN;
                fds[user_counter].revents = 0;
                //setnonblocking(accept_fd);
                char buf[INET_ADDRSTRLEN];
                printf("comes a new user: %s,%d,now have %d users\n",
                        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, buf, 
                                                            INET_ADDRSTRLEN),  
                        client_addr.sin_port, user_counter);
            }
            /* recv data from client and transfer to other clients */
            else if (fds[i].revents & POLLIN) {
                memset(client_data[fds[i].fd].buffer, '\0', BUFFER_SIZE);
                ret = recv(fds[i].fd, client_data[fds[i].fd].buffer, 
                                                            BUFFER_SIZE, 0);
                if (ret < 0) {
                    perror("server recv failed");
                    printf("ret <0 is %d\n", fds[i].fd);

                    /* 
                     * Recycling client data occupying space 
                     * and change the client_data, user_counter 
                     * to achieve no gap in fds 
                     */
                    close(fds[i].fd);
                    client_data[fds[i].fd] = client_data[fds[user_counter].fd];
                    fds[i] = fds[user_counter];
                    fds[user_counter].fd = -1;
                    fds[user_counter].events = 0;
                    fds[user_counter].revents = 0;
                    user_counter--;
                    continue;
                }
                else if (ret == 0) {
                    printf("server closed connection client: %d\n", fds[i].fd);
                    
                    close(fds[i].fd);
                    fds[i] = fds[user_counter];
                    client_data[fds[i].fd] = client_data[fds[user_counter].fd];
                    fds[user_counter].fd = -1;
                    fds[user_counter].events = 0;
                    fds[user_counter].revents = 0;
                    user_counter--;
                    printf("comes a new user,now have %d users\n",user_counter); 
                    continue;
                }
                /* transfer data to other clients */
                else {
                    printf("server recv data from client: %d : %s\n", 
                                    fds[i].fd, client_data[fds[i].fd].buffer);
                    for (volatile int j = 1; j <= user_counter; j++) {
                        if(fds[j].fd == fds[i].fd){
                            continue;
                        }
                        ret = send(fds[j].fd, client_data[fds[i].fd].buffer,
                                strlen(client_data[fds[i].fd].buffer), 0);
                        if (ret < 0) {
                            perror("server send failed");

                            continue;
                        }
                    }
                }
            }
        }
    }
    close(listen_fd);
    free(client_data);

    return 0;
}


 
 
 
 