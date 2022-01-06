/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 14:44:21 by clorin            #+#    #+#             */
/*   Updated: 2022/01/06 11:31:35 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Webserv.hpp"
#include <signal.h>

int		main(int ac, char **av)
{
    Webserv		webserv;

    signal(SIGPIPE, SIG_IGN);

    try
    {
        if (ac == 2)
            webserv.config(av[1]);
        else if (ac == 1)
            webserv.config();
        webserv.printServers();
        if (!webserv.run())
            webserv.shutdownServers();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
	return (EXIT_SUCCESS);
}
