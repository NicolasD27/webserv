/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 22:36:14 by clorin            #+#    #+#             */
/*   Updated: 2022/01/20 16:11:15 by clorin           ###   ########.fr       */
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
        std::map<std::string, std::string> _env;

    public:
        CGIHandler(void);
        ~CGIHandler(void);

        void                    initEnv();
        char					**getEnv() const;
        size_t                  size() const;

        std::string		        executeCgi(const char **scriptName, const std::string & body);
};

#endif