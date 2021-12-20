/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:27:16 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:48:28 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <queue>
#include <netinet/in.h>
#include <string.h>

#define MAX_SIZE 30000
class Client
{
private:
    int     _socket;
    struct sockaddr_in _address;
    std::queue<char[MAX_SIZE]> _message_queue;
    char _sending_buff[MAX_SIZE];
    int _current_sending_byte;
    char _receiving_buff[MAX_SIZE];
    int _current_receiving_byte;
    
public:
    Client(int listen_socket);
    Client(Client const &);
    ~Client();
    Client &operator=(Client const &);

    int     setup(std::string &);
    
};

#endif