/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 22:36:14 by clorin            #+#    #+#             */
/*   Updated: 2022/03/04 09:48:20 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <string>
#include <map>

class CGIHandler;
#include "Request.hpp"
#include "Response.hpp"
#include "utility.hpp"

class CGIHandler
{
    private:
        Response    *_response;
        std::string _body;
        std::string _script;
        std::string _file;
        char        **_envChar;
        std::map<std::string, std::string>  _env;
        void		setEnvChar();
        CGIHandler(CGIHandler const &cpy);

    public:

        CGIHandler(void);
        CGIHandler(Request const *, Response *, std::string script, std::string file);
        ~CGIHandler(void);
        CGIHandler & operator=(CGIHandler const &);

        size_t                  size() const;

        std::string		        executeCgi(unsigned int *);
};

#endif