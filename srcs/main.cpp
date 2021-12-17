/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 14:44:21 by clorin            #+#    #+#             */
/*   Updated: 2021/12/17 16:45:45 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Webserv.hpp"

int		main(int ac, char **av)
{
    Webserv		webserv;

    try
    {
        if (ac == 2)
            webserv.config(av[1]);
        else if (ac == 1)
             webserv.config();
        webserv.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
	return (EXIT_SUCCESS);
}
