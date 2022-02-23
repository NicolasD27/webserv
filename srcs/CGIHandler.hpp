/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 22:36:14 by clorin            #+#    #+#             */
/*   Updated: 2022/02/18 19:43:32 by clorin           ###   ########.fr       */
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
        std::string _body;
        std::string _script;
        std::string _file;
        Response    *_response;

    public:
        std::map<std::string, std::string>  _env;

        //CGIHandler(Request const *, std::map<std::string, std::string>&);
        CGIHandler(void);
        CGIHandler(Request const *, Response *, std::string script, std::string file);
        ~CGIHandler(void);
        CGIHandler & operator=(CGIHandler const &);

        //void                    initEnv();
        char					**getEnv() const;
        size_t                  size() const;

        std::string		        executeCgi();
};

#endif