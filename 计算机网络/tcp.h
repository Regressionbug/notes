//
// Created by ppdog on 2020/11/20.
//

#ifndef TCPSOCKET_TCP_H
#define TCPSOCKET_TCP_H

#include<iostream>
#include<string>
#include<sys/socket.h>
#include<sys/unistd.h>
#include<netinet/in.h>
#include<cstdlib>

#define MAXSIZE 1024

namespace TCP {
    class Server {
    private:
        // variable
        int listen_fd; // listen socket
        int connect_fd; // connect socket
        in_port_t port; // port
        in_addr_t addr; // address
        sockaddr_in server_addr; // server address struct
        sockaddr_in client_addr; // client address struct
        socklen_t addr_len; // address length
        pid_t child_pid; // child_pid
    public:
        Server() = delete;
        /* *
         * initial server: 自动转换：点分十进制地址和端口
         * @param string address
         * @param uint16_t port
         * */
        Server(std::string address, uint16_t port) {
            in_addr addr;
            inet_pton(AF_INET, address.c_str(), &addr);
            this->addr = addr.s_addr;
            this->port = htons(port);
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = this->addr;
            server_addr.sin_port = this->port;
            addr_len = sizeof(client_addr);
        }
        /* *
         * initial server： 输入正确的网络地址和端口
         * @param in_addr_t addr
         * @param in_port_t port
         * */
        Server(in_addr_t addr, in_port_t port):addr(addr), port(port) {
            // server address
            server_addr.sin_family = AF_INET; // IPv4
            server_addr.sin_addr.s_addr = this->addr;
            server_addr.sin_port = this->port;
            addr_len = sizeof(client_addr);
        }
        // start server
        void start() {
            // father process pid
            std::cout << "Server process: " << getpid() << std::endl;
            // listen socket
            listen_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4 byte TCP
            if (listen_fd < 0) {
                std::cerr << "Server: can not create listen socket";
                return;
            }
            // bind()
            if (bind(listen_fd, (sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
                std::cerr << "Server: can not bind socket";
                return;
            }
            // listen()
            if (listen(listen_fd, 5) < 0) { // listen socket && backlog
                std::cerr << "Server: can not listen";
                return;
            }
            std::cout << "Server: start successfully" << std::endl;
            char addr[20];
            inet_ntop(AF_INET, &server_addr.sin_addr, addr, sizeof(addr));
            std::cout << "Server address: " << std::string(addr) << std::endl;
            std::cout << "Server port: " << ntohs(server_addr.sin_port) << std::endl;
            for ( ; ; ) {
                std::cout << "Server: waiting for client" << std::endl;
                // accept()
                connect_fd = accept(listen_fd, (sockaddr*) &client_addr, &addr_len);
                if (connect_fd > 0) {
                    std::cout << "Server: connect to a client" << std::endl;
                    char addr[20];
                    inet_ntop(AF_INET, &client_addr.sin_addr, addr, sizeof(addr));
                    std::cout << "Client address: " << std::string(addr) << std::endl;
                    std::cout << "Client port: " << ntohs(client_addr.sin_port) << std::endl;
                }
                if ( (child_pid = fork()) == 0 ) { // child process
                    close(listen_fd);
                    // handle request
                    str_echo(connect_fd);
                    exit(0);
                }
                // child process
                std::cout << "Child process: " << child_pid << std::endl;
                // close()
                close(connect_fd);
            }
        }

        void str_echo(int sockfd) {
            ssize_t n;
            char buf[MAXSIZE];

            while( (n = read(sockfd, buf, MAXSIZE)) > 0 ) {
                std::string message(buf);
                std::cout << "Server receive: " << message << std::endl;
                write(sockfd, buf, n);
            }
            if (n < 0)
                std::cerr << "str_echo: read error";
        }
    };

    class Client {
    private:
        int client_fd;
        in_addr_t addr; // server address
        in_port_t port; // server port
        sockaddr_in server_addr; // server address struct
    public:
        Client() = delete;
        /* *
         * initial client
         * @param in_addr_t addr
         * @param in_port_t port
         * */
        Client(std::string address, uint16_t port) {
            in_addr addr;
            inet_pton(AF_INET, address.c_str(), &addr);
            this->addr = addr.s_addr;
            this->port = htons(port);
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = this->addr;
            server_addr.sin_port = this->port;
        }
        /* *
         * initial client: 输入正确的服务器地址和端口
         * @param in_addr_t addr
         * @param in_port_t port
         * */
        Client(in_addr_t addr, in_port_t port): addr(addr), port(port){
            // server address
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = this->addr;
            server_addr.sin_port = this->port;
        }
        void start() {
            // Client process
            std::cout << "Client process: " << getpid() << std::endl;
            // connect socket
            client_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4 byte TCP
            if (client_fd < 0) {
                std::cerr << "Client: can not create connect socket";
                return;
            }
            // connect()
            if (connect(client_fd, (sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
                std::cerr << "Client: can not connect server" ;
                return;
            } else {
                std::cout << "Client: connect to server successfully" << std::endl;
            }
            // do things
            str_cli(stdin, client_fd);

            exit(0);
        }

        void str_cli(FILE* fp, int sockfd) {
            std::string sendline;
            char recvline[MAXSIZE];
            while (std::cin) {
                std::cin >> sendline;
                write(sockfd, sendline.c_str(), sendline.size());
                if (read(client_fd, recvline, MAXSIZE) == 0) {
                    std::cerr << "str_cli: server terminated prematurely";
                }
                std::string message(recvline);
                std::cout << "Client receive: " << recvline << std::endl;
            }
        }
    };
}

#endif //TCPSOCKET_TCP_H