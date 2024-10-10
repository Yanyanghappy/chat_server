# 本程序是服务端和客户端写在一起，通过传入不同的参数实现启动服务端或客户端
    可以通过运行 ./mes 即会出现提示
# 实现功能
    以poll为例实现一个简单的聊天室 程序，以阐述如何使用I/O复用技术来同时处理网络连接和用户输入。 
    该聊天室程序能让所有用户同时在线群聊，它分为客户端和服务器两个部分。
    其中客户端程序有两个功能：
        一是从标准输入终端读入用户数据，并将用户数据发送至服务器；
        二是往标准输出终端打印服务器发送给它的数据。
    服务器的功能是接收客户数据，并把客户数据发送 给每一个登录到该服务器上的客户端（数据发送者除外）。

# 运行方法
    1.编译程序：bear make
        如果需要交叉编译即更改Makefile即可
    2.运行程序：./mes即会出现提示符，根据传入的参数有两种模式可以选择
        server Usage: ./mes <port>  //监听port端口
        client Usage: ./mes <ip> <port> <buffer>  //发送buffer大小的数据到ip:port并一直建立连接

# 学习内容

## 1 理解poll的原理和使用细则
在一个`poll`函数调用中，对于每个文件描述符，`revents`字段只会被设置一次，以反映调用时刻的状态。这意味着，即使多个事件同时发生，`poll`函数也只会为每个文件描述符报告一次事件。除非后续重新调用了poll（与之前的poll无关了），而epoll是注册的事件表（只注册一次），会同时触发一个文件描述符的多个事件，所以会对于这个文件描述符出现竟态条件需要避免

- POLLRDHUP和EPOLLRDHUP这两个事件其实是一个东西，分别对应poll和epoll，通常用来判断对端是否关闭，但是当你对某个socket注册POLLIN和POLLRDHUP(EPOLLIN和EPOLLRDHUP)时，在对端关闭时，对于poll来说会一直触发POLLIN + POLLRDHUP事件，epoll也会触发EPOLLIN + EPOLLRDHUP事件，是否一直触发要看epoll是工作在LT模式下还是ET模式下。

- 每次重新调用 `poll` 后，`revents` 字段会被内核重置为 0，除非有相应的事件发生。
- 检测对端是否关闭连接也会触发一次POLLIN事件
	- 所以，当我们使用POLLRDHUP（EPOLLRDHUP）事件来判断对端是否关闭时，POLLRDHUP（EPOLLRDHUP）事件的处理应放在POLLIN（EPOLLIN）事件的前面，避免将对端关闭当做一次读事件，而无法处理POLLRDHUP（EPOLLRDHUP）事件。

- poll的触发逻辑
	- （即所有事件存在内核中，一次poll只能处理一次事件， POLLIN | POLLOUT这种在一起的算一次）
	- 正常触发，来一次事件触发poll->重新调用poll后->再来一次事件...
	- 连续触发，调用poll后，对于一个fd同时连续接收到多个事件（比如连接和可读），这些事件会存在内核中然后依次触发poll，比如事件A，B，C一起到达，poll先调用一次然后传入事件A，然后处理完后，revents置0，下一次调用poll(fd)后还会被事件B触发。
	- 数据触发，只要缓冲区中有数据可读，并且没有应用程序读取这些数据，`poll` 在后续调用中会一直（每一次）报告文件描述符为读就绪。
	- 如果poll注册的文件描述符集中有相同的fd，比如fds[1]=fds[2]=5；则如果有一个fd = 5 的事件POLLIN触发，在轮询fds的过程中fds[1]和fds[2]的revents等于POLLIN事件

        
## 2 确定编程风格规范
    while (1) {
    }

    if(2 > 1){

    }
    /* 只有函数才花括号在下一行且关键字紧挨括号 */
    void func()
    {
    }

    /* 命名用小写 */
    client_data

    srtuct Client{

    };

    /* 
     * 只有结构体的首字母大写，其它所有全小写
     * dididi
    */
    
    strcut Client client;

    char *p = NULL;

    (a, b, c, |80列
        d ,f)
## 3 printf的格式化输出的小问题
	在linux系统下,printf函数是行缓冲式的输出,当printf遇到n时,或者缓冲区满时,才会将缓冲区里的内容刷新到标准输出 (stdout).
    因此, printf ("p"); 等语句的显示不能立刻显示在屏幕上,但是printf ("pn"); 可以.
    为了解决这个问题可以通过: fflush (stdout);解决,即变成:printf ("p");fflush (stdout_printf只显示一瞬间 
