/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 22:36:14 by clorin            #+#    #+#             */
/*   Updated: 2022/02/06 10:20:41 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <string>
#include <map>

#include "Request.hpp"

class CGIHandler
{
    private:
        CGIHandler(void){};
        std::map<std::string, std::string> _env;

    public:

        CGIHandler(Request const *, std::map<std::string, std::string>&);
        ~CGIHandler(void);

        //void                    initEnv();
        char					**getEnv() const;
        size_t                  size() const;

        std::string		        executeCgi(const char **scriptName, const std::string & body);
};

#endif