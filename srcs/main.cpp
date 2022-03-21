/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 14:44:21 by clorin            #+#    #+#             */
/*   Updated: 2022/03/21 16:17:50 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Webserv.hpp"
#include <signal.h>

Webserv		webserv;

void catchCtrl_C(int dummy) {
    (void) dummy;
    std::cerr << std::endl << C_RED << "Ctr-C detected "<< C_RESET << "... stopping server ...";
    webserv.stop();
    std::cerr << C_GREEN << "ok" << C_RESET << std::endl;
    exit(EXIT_SUCCESS);
}

int		main(int ac, char **av)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, catchCtrl_C);
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
