
// Server side C program to demonstrate HTTP Server programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// int
// main(void)
// {
//     fd_set rfds;
//     struct timeval tv;
//     int retval;

//     /* Surveiller stdin (fd 0) en attente d'entrées */
//     FD_ZERO(&rfds);
//     FD_SET(0, &rfds);

//     /* Pendant 5 secondes maxi */
//     tv.tv_sec = 5;
//     tv.tv_usec = 0;

//     retval = select(1, &rfds, NULL, NULL, &tv);
//     /* Considérer tv comme indéfini maintenant ! */

//     if (retval == -1)
//         perror("select()");
//     else if (retval)
//         printf("Des données sont disponibles maintenant\n");
//         /* FD_ISSET(0, &rfds) est vrai */
//     else
//         printf("Aucune données durant les 5 secondes\n");

//     exit(EXIT_SUCCESS);
// }
#define PORT 8080
#define BUFFER_SIZE 30000
#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))
int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Only this line has been changed. Everything is same.
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        fd_set rfds;
        struct timeval tv;
        int retval;
        ssize_t nread;
        struct sockaddr_storage peer_addr;
        socklen_t peer_addr_len;
        int r, nfds = 0;
        fd_set rd, wr, er;
        FD_ZERO(&rd);
        FD_ZERO(&wr);
        FD_ZERO(&er);
        FD_SET(new_socket, &rd);
        FD_SET(new_socket, &wr);
        nfds = max(nfds, new_socket);

        /* Surveiller stdin (fd 0) en attente d'entrées */
        

        /* Pendant 5 secondes maxi */
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        retval = select(nfds + 1, &rd, NULL, NULL, &tv);
        /* Considérer tv comme indéfini maintenant ! */

        if (retval == -1)
            perror("select()");
        else if (retval)
            nread = recvfrom(new_socket, buffer, BUFFER_SIZE, 0,
                (struct sockaddr *) &peer_addr, &peer_addr_len);
            /* FD_ISSET(0, &rfds) est vrai */
        else
            printf("Aucune données durant les 5 secondes\n");

        
        printf("%s\n",buffer );
        
        retval = select(nfds + 1, NULL, &wr, NULL, &tv);
        if (retval == -1)
            perror("select()");
        else if (retval)
        {

            if (sendto(new_socket, hello, nread, 0,
                    (struct sockaddr *) &peer_addr,
                    peer_addr_len) != nread)
                    printf("Erreur d'envoi\n");
        }
        else
            printf("Aucune données durant les 5 secondes\n");
       
        printf("------------------Hello message sent-------------------");
        close(new_socket);
    }
    return 0;
}
