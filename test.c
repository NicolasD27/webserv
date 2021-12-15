#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

static int forward_port;

#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))

static int
listen_socket(int listen_port)
{
    struct sockaddr_in a;
    int s;
    int yes;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
               (char *) &yes, sizeof (yes)) < 0) {
        perror("setsockopt");
        close(s);
        return -1;
    }
    memset(&a, 0, sizeof (a));
    a.sin_port = htons(listen_port);
    a.sin_family = AF_INET;
    if (bind(s, (struct sockaddr *) &a, sizeof (a)) < 0) {
        perror("bind");
        close(s);
        return -1;
    }
    printf("accepting connections on port %d\n", listen_port);
    listen(s, 10);
    return s;
}

static int
connect_socket(int connect_port, char *address)
{
    struct sockaddr_in a;
    int s;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        close(s);
        return -1;
    }

    memset(&a, 0, sizeof (a));
    a.sin_port = htons(connect_port);
    a.sin_family = AF_INET;

    if ((a.sin_addr.s_addr = (struct in_addr *)inet_addr(address) == -1)) {
        perror("bad IP address format");
        close(s);
        return -1;
    }

    if (connect(s, (struct sockaddr *) &a, sizeof (a)) < 0) {
        perror("connect()");
        shutdown(s, SHUT_RDWR);
        close(s);
        return -1;
    }
    return s;
}

#define SHUT_FD1 {                      \
        if (fd1 >= 0) {                 \
            shutdown(fd1, SHUT_RDWR);   \
            close(fd1);                 \
            fd1 = -1;                   \
        }                               \
    }

#define SHUT_FD2 {                      \
        if (fd2 >= 0) {                 \
            shutdown(fd2, SHUT_RDWR);   \
            close(fd2);                 \
            fd2 = -1;                   \
        }                               \
    }

#define BUF_SIZE 30000

int
main(int argc, char **argv)
{
    int h;
    int fd1 = -1, fd2 = -1;
    char buf1[BUF_SIZE], buf2[BUF_SIZE];
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    int buf1_avail, buf1_written;
    int buf2_avail, buf2_written;

    if (argc != 4) {
        fprintf(stderr,
                 "Utilisation\n\tfwd <listen-port> "
                 "<forward-to-port> <forward-to-ip-address>\n");
        exit(EXIT_FAILURE);
    }

    // signal(SIGPIPE, SIG_IGN);

    forward_port = atoi(argv[2]);

    h = listen_socket(atoi(argv[1]));
    if (h < 0)
        exit(EXIT_FAILURE);

    for (;;) {
        int r, nfds = 0;
        fd_set rd, wr, er;
        FD_ZERO(&rd);
        FD_ZERO(&wr);
        FD_ZERO(&er);
        FD_SET(h, &rd);
        nfds = max(nfds, h);
        if (fd1 > 0 && buf1_avail < BUF_SIZE) {
            FD_SET(fd1, &rd);
            nfds = max(nfds, fd1);
        }
        // if (fd2 > 0 && buf2_avail < BUF_SIZE) {
        //     FD_SET(fd2, &rd);
        //     nfds = max(nfds, fd2);
        // }
        // if (fd1 > 0
        //     && buf2_avail - buf2_written > 0) {
        //     FD_SET(fd1, &wr);
        //     nfds = max(nfds, fd1);
        // }
        // if (fd2 > 0
        //     && strlen(hello) - buf1_written > 0) {
        //     FD_SET(fd2, &wr);
        //     nfds = max(nfds, fd2);
        // }
        // if (fd1 > 0) {
        //     FD_SET(fd1, &er);
        //     nfds = max(nfds, fd1);
        // }
        // if (fd2 > 0) {
        //     FD_SET(fd2, &er);
        //     nfds = max(nfds, fd2);
        // }

        r = select(nfds + 1, &rd, &wr, &er, NULL);

        if (r == -1 && errno == EINTR)
            continue;
        if (r < 0) {
            perror("select()");
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(h, &rd)) {
            unsigned int l;
            struct sockaddr_in client_address;
            memset(&client_address, 0, l = sizeof(client_address));
            r = accept(h, (struct sockaddr *) &client_address, &l);
            if (r < 0) {
                perror("accept()");
            } else {
                SHUT_FD1;
                SHUT_FD2;
                buf1_avail = buf1_written = 0;
                buf2_avail = buf2_written = 0;
                fd1 = r;
                fd2 = r;
                // fd2 = connect_socket(forward_port, argv[3]);
                if (fd2 < 0) {
                    SHUT_FD1;
                } else
                    printf("connexion de %s\n",
                           inet_ntoa(client_address.sin_addr));
            }
        }
/* NB : lecture des données hors bande avant les lectures normales */
        // if (fd1 > 0)
        //     if (FD_ISSET(fd1, &er)) {
        //         char c;
        //         errno = 0;
        //         r = recv(fd1, &c, 1, MSG_OOB);
        //         if (r < 1) {
        //             SHUT_FD1;
        //         } else
        //             send(fd2, &c, 1, MSG_OOB);
        //     }
        // if (fd2 > 0)
        //     if (FD_ISSET(fd2, &er)) {
        //         char c;
        //         errno = 0;
        //         r = recv(fd2, &c, 1, MSG_OOB);
        //         if (r < 1) {
        //             SHUT_FD1;
        //         } else
        //             send(fd1, &c, 1, MSG_OOB);
        //     }
        if (fd1 > 0)
            if (FD_ISSET(fd1, &rd)) {
                r = read(fd1, buf1 + buf1_avail,
                         BUF_SIZE - buf1_avail);
                if (r < 1) {
                    SHUT_FD1;
                } else
                    buf1_avail += r;
            }
        // if (fd2 > 0)
        //     if (FD_ISSET(fd2, &rd)) {
        //         r = read(fd2, buf2 + buf2_avail,
        //                  BUF_SIZE - buf2_avail);
        //         if (r < 1) {
        //             SHUT_FD2;
        //         } else
        //             buf2_avail += r;
        //     }
        // if (fd1 > 0)
        //     if (FD_ISSET(fd1, &wr)) {
        //         r = write(fd1, buf2 + buf2_written,
        //                   buf2_avail - buf2_written);
        //         if (r < 1) {
        //             SHUT_FD1;
        //         } else
        //             buf2_written += r;
        //     }
        
        if (fd2 > 0)
            if (FD_ISSET(fd2, &wr)) {
                r = write(fd2, hello + buf1_written,
                          strlen(hello) - buf1_written);
                if (r < 1) {
                    SHUT_FD2;
                } else
                    buf1_written += r;
            }
/* Vérifie si l'écriture de données a provoqué la lecture de données */
        // if (buf1_written == buf1_avail)
        //     buf1_written = buf1_avail = 0;
        // if (buf2_written == buf2_avail)
        //     buf2_written = buf2_avail = 0;
/* une extrémité a fermé la connexion, continue
   d'écrire vers l'autre extrémité jusqu'à ce que ce soit vide */
        // if (fd1 < 0 && strlen(hello) - buf1_written == 0) {
        //     SHUT_FD2;
        // }
        // if (fd2 < 0 && buf2_avail - buf2_written == 0) {
        //     SHUT_FD1;
        // }
        printf("fd1 %d\n%s\n", fd1, buf1);
    }
    exit(EXIT_SUCCESS);
}

// // Server side C program to demonstrate HTTP Server programming
// #include <stdio.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/time.h>
// #include <sys/types.h>
// #include <unistd.h>

// // int
// // main(void)
// // {
// //     fd_set rfds;
// //     struct timeval tv;
// //     int retval;

// //     /* Surveiller stdin (fd 0) en attente d'entrées */
// //     FD_ZERO(&rfds);
// //     FD_SET(0, &rfds);

// //     /* Pendant 5 secondes maxi */
// //     tv.tv_sec = 5;
// //     tv.tv_usec = 0;

// //     retval = select(1, &rfds, NULL, NULL, &tv);
// //     /* Considérer tv comme indéfini maintenant ! */

// //     if (retval == -1)
// //         perror("select()");
// //     else if (retval)
// //         printf("Des données sont disponibles maintenant\n");
// //         /* FD_ISSET(0, &rfds) est vrai */
// //     else
// //         printf("Aucune données durant les 5 secondes\n");

// //     exit(EXIT_SUCCESS);
// // }
// #define PORT 8080
// #define BUFFER_SIZE 30000
// #undef max
// #define max(x,y) ((x) > (y) ? (x) : (y))
// int main(int argc, char const *argv[])
// {
//     int server_fd, new_socket; long valread;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
    
//     // Only this line has been changed. Everything is same.
//     char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    
//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
//     {
//         perror("In socket");
//         exit(EXIT_FAILURE);
//     }
    

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons( PORT );
    
//     memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
//     {
//         perror("In bind");
//         exit(EXIT_FAILURE);
//     }
//     if (listen(server_fd, 10) < 0)
//     {
//         perror("In listen");
//         exit(EXIT_FAILURE);
//     }
//     while(1)
//     {
//         printf("\n+++++++ Waiting for new connection ++++++++\n\n");
//         if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
//         {
//             perror("In accept");
//             exit(EXIT_FAILURE);
//         }
        
//         char buffer[30000] = {0};
//         fd_set rfds;
//         struct timeval tv;
//         int retval;
//         ssize_t nread;
//         struct sockaddr_storage peer_addr;
//         socklen_t peer_addr_len;
//         int r, nfds = 0;
//         fd_set rd, wr, er;
//         FD_ZERO(&rd);
//         FD_ZERO(&wr);
//         FD_ZERO(&er);
//         FD_SET(new_socket, &rd);
//         FD_SET(new_socket, &wr);
//         nfds = max(nfds, new_socket);

//         /* Surveiller stdin (fd 0) en attente d'entrées */
        

//         /* Pendant 5 secondes maxi */
//         tv.tv_sec = 5;
//         tv.tv_usec = 0;

//         retval = select(nfds + 1, &rd, NULL, NULL, NULL);
//         /* Considérer tv comme indéfini maintenant ! */

//         if (retval == -1)
//             perror("select()");
//         else if (retval)
//         {

//             nread = read(new_socket, buffer, BUFFER_SIZE);
//             buffer[nread] = 0;
//             /* FD_ISSET(0, &rfds) est vrai */
//         }
//         else
//             printf("Aucune données durant les 5 secondes\n");

        
//         printf("%s\n",buffer );
        
//         // retval = select(nfds + 1, NULL, &wr, NULL, &tv);
//         // if (retval == -1)
//         //     perror("select()");
//         // else if (retval)
//         // {

//         //     if (write(new_socket, hello, nread) != nread)
//         //             printf("Erreur d'envoi\n");
//         // }
//         // else
//         //     printf("Aucune données durant les 5 secondes\n");
       
//         // printf("------------------Hello message sent-------------------");
//         // close(new_socket);
//     }
//     return 0;
// }
