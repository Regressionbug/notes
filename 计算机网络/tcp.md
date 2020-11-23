## socket函数

```c
#include<sys/socket.h>
int socket(int family, int type, int protocol);
```

- 作用：创建套接字

- 返回：若成功则为非负描述符，若出错则为-1

- 参数

  `familt`：协议族(`IP`)

  - `AF_INET`：IPv4协议
  - `AF_INET6`：IPv6协议
  - `AF_LOCAL`：Unix域协议
  - `AF_ROUTE`：路由套接字
  - `AF_KEY`：密钥套接字

  `type`：套接字类型

  - `SOCK_STREAM`：字节流套接字
  - `SOCK_DGRAM`：数据报套接字
  - `SOCK_SEQPACKET`：有序分组套接字
  - `SOCK_RAW`：原始套接字

  `protocol`：协议常值(`TCP/UDP`)

  - `IPPROTO_TCP`：TCP传输协议
  - `IPPROTO_UDP`：UDP传输协议
  - `IPPROTO_SCTP`：SCTP传输协议

## connect函数

```c
#include<sys/socket.h>
int connect(int sockfd, const struct sockaddr *server_addr, socklen_t addr_len);
```

- 作用：用connect函数来建立与TCP服务器的连接

- 参数

  `sockfd`：socket函数返回的套接字描述符

  `server_addr`：指向协议地址结构的指针

  `addr_len`：该结构的大小

## bind函数

```c
#include<sys/socket.h>
int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
```

- 作用：把本地协议地址赋予一个套接字

- 参数

  `sockfd`：socket函数返回的套接字描述符

  `my_addr`：指向协议地址结构的指针

  `addr_len`：该结构的大小

## listen函数

```c
#include<sys/socket.h>
int listen(int sockfd, int backlog);
```

- 作用：把未连接的套接字转换成一个被动套接字

- 参数

  `sockfd`：套接字描述符

  `backlog`：套接字排队的最大连接个数

## accept函数

```c
#include<sys/socket.h>
int accept(int sockfd, struct sockaddr *client_addr, socklen_t *addrlen);
```

- 作用：从已完成连接队列头返回下一个已完成连接套接字(区别于监听套接字)

- 返回：若成功则为非负描述符，若出错则为-1

- 参数

  `sockfd`：套接字描述符

  `client_addr`：已连接的对端进程的协议地址

  `addrlen`：套接字地址结构长度

## close函数

```c
#include<unistd.h>
int close(int sockfd);
```

- 作用：关闭套接字，终止TCP连接
- 参数：`sockfd`：套接字描述符

## getsockname getpeername函数

```c
#include<sys/socket.h>
int getsockname(int sockfd, struct sockaddr *local_addr, socklen_t *addrlen);
int getpeername(int sockfd, struct sockaddr *peer_addr, socklen_t * addrlen);
```

- 作用：

  返回与某个套接字关联的本地协议地址(`getsockname`)

  返回与某个套接字关联的外地协议地址(`getpeername`)