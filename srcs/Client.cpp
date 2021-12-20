/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:33:58 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:37:53 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int listen_socket)
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    socklen_t client_len = sizeof(address);
    int new_client_sock = accept(listen_socket, (struct sockaddr *)&address, &client_len);
    if (new_client_sock < 0) {
        perror("accept()");
        return ;
    }
    _socket = new_client_sock;
    address = address;
    _current_sending_byte   = -1;
    _current_receiving_byte = 0;

}

Client::Client(Client const &cpy)
{
    if (this != &cpy) {
		*this = cpy;
	}
}

Client::~Client() {}

Client      &Client::operator=(Client const &cpy)
{
    if(this != &cpy)
    {
        _socket = cpy._socket;
        _address = cpy._address;
        _current_sending_byte   = cpy._current_sending_byte;
        _current_receiving_byte = cpy._current_receiving_byte;

    }
    return *this;
}
