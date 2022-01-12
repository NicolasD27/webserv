#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    std::string value = "123fr";
    int num;
    if (value.length() == 0)
        return 0;
    std::istringstream ss(value);
    ss >> num;
    std::cout << num;
    return 0;

}
// static int forward_port;

// // #undef std::max
// // #define std::max(x,y) ((x) > (y) ? (x) : (y))

// static int
// listen_socket(int listen_port)
// {
//     struct sockaddr_in a;
//     int s;
//     int yes;

//     if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         perror("socket");
//         return -1;
//     }
//     yes = 1;
//     if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
//                (char *) &yes, sizeof (yes)) < 0) {
//         perror("setsockopt");
//         close(s);
//         return -1;
//     }
//     memset(&a, 0, sizeof (a));
//     a.sin_port = htons(listen_port);
//     a.sin_family = AF_INET;
//     if (bind(s, (struct sockaddr *) &a, sizeof (a)) < 0) {
//         perror("bind");
//         close(s);
//         return -1;
//     }
//     printf("accepting connections on port %d\n", listen_port);
//     listen(s, 10);
//     return s;
// }

// static int
// connect_socket(int connect_port, char *address)
// {
//     struct sockaddr_in a;
//     int s;

//     if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         perror("socket");
//         close(s);
//         return -1;
//     }

//     memset(&a, 0, sizeof (a));
//     a.sin_port = htons(connect_port);
//     a.sin_family = AF_INET;

//     if ((a.sin_addr.s_addr = (in_addr_t)inet_addr(address))) {
//         perror("bad IP address format");
//         close(s);
//         return -1;
//     }

//     if (connect(s, (struct sockaddr *) &a, sizeof (a)) < 0) {
//         perror("connect()");
//         shutdown(s, SHUT_RDWR);
//         close(s);
//         return -1;
//     }
//     return s;
// }

// #define SHUT_FD1 {                      \
//         if (fd1 >= 0) {                 \
//             shutdown(fd1, SHUT_RDWR);   \
//             close(fd1);                 \
//             fd1 = -1;                   \
//         }                               \
//     }

// #define SHUT_FD2 {                      \
//         if (fd2 >= 0) {                 \
//             shutdown(fd2, SHUT_RDWR);   \
//             close(fd2);                 \
//             fd2 = -1;                   \
//         }                               \
//     }

// #include <sstream>

// #define SSTR( x ) static_cast< std::ostringstream & >( \
//         ( std::ostringstream() << std::dec << x ) ).str()
// #define BUF_SIZE 30000
// #define PORT 3000

// int
// main(int argc, char **argv)
// {
//     int h;
//     int fd1 = -1, fd2 = -1;
//     char buf1[BUF_SIZE], buf2[BUF_SIZE];
//     std::string header = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";
//     std::string response = "";
//     int buf1_avail, buf1_written;
//     int buf2_avail, buf2_written;
//     int content_length = 0;

//     signal(SIGPIPE, SIG_IGN);


//     h = listen_socket(PORT);
//     if (h < 0)
//         exit(EXIT_FAILURE);

//     for (;;) {
//         int r, nfds = 0;
//         fd_set rd, wr, er;
//         FD_ZERO(&rd);
//         FD_ZERO(&wr);
//         FD_ZERO(&er);
//         FD_SET(h, &rd);
//         nfds = std::max(nfds, h);
//         if (fd1 > 0 && buf1_avail < BUF_SIZE) {
//             FD_SET(fd1, &rd);
//             nfds = std::max(nfds, fd1);
//         }
//         if (response.length() - buf1_written > 0) {
//             FD_SET(fd2, &wr);
//             printf("here\n");
//             nfds = std::max(nfds, fd2);
//         }
//         r = select(nfds + 1, &rd, &wr, &er, NULL);

//         if (r == -1 && errno == EINTR)
//             continue;
//         if (r < 0) {
//             perror("select()");
//             exit(EXIT_FAILURE);
//         }
//         if (FD_ISSET(h, &rd)) {
//             printf("accepting");
//             unsigned int l;
//             struct sockaddr_in client_address;
//             memset(&client_address, 0, l = sizeof(client_address));
//             r = accept(h, (struct sockaddr *) &client_address, &l);
//             if (r < 0) {
//                 perror("accept()");
//             } else {
//                 SHUT_FD1;
//                 SHUT_FD2;
//                 buf1_avail = buf1_written = 0;
//                 buf2_avail = buf2_written = 0;
//                 fd1 = r;
//                 fd2 = r;
//                 if (fd2 < 0) {
//                     SHUT_FD1;
//                 } else
//                     printf("connexion de %s\n",
//                            inet_ntoa(client_address.sin_addr));
//             }
//         }

//         if (fd1 > 0)
//             if (FD_ISSET(fd1, &rd)) {
//                 printf("fd1 est la");
//                 r = read(fd1, buf1 + buf1_avail,
//                          BUF_SIZE - buf1_avail);
//                 // buf1[r] = 0;
//                 if (content_length < std::string(buf1).length())
//                 {
//                     content_length = std::string(buf1).length();
//                     response = header + SSTR(content_length) + "\n\r\n" + buf1;
                    
//                 }
//                 buf1_avail += r;
//             }
//         if (fd2 > 0 && buf1_written < response.length() && std::string(buf1).find("\r\n") != std::string::npos)
//             if (FD_ISSET(fd2, &wr)) {
                
                
                
//                  r = write(fd2, response.c_str() + buf1_written,
//                           response.length() - buf1_written);
                
//                 if (r < 1) {
//                     printf("shutdown2\n");
//                     SHUT_FD2;
//                 } else
//                     buf1_written += r;
//             }
//         // if (fd1 < 0 && response.length() - buf1_written == 0) {
//         //     SHUT_FD1;
//         // }
//         // if (response.length() == buf1_written && std::string(buf1).find("\r\n") != std::string::npos) {
//         //     SHUT_FD1;
//         //     buf1_written = 0;
//         // }
//         // printf("|%s|\n", buf1);
//         printf("fd1 %d\n%d %d %ld\n%s\n", fd1, buf1_avail, buf1_written, response.length(), "response.c_str()");
//     }
//     exit(EXIT_SUCCESS);
// }