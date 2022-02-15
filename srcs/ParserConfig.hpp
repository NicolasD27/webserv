/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 10:36:45 by clorin            #+#    #+#             */
/*   Updated: 2022/01/11 15:39:02 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSERCONFIG_HPP
# define PARSERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "utility.hpp"
#include "Server.hpp"
#include "Location.hpp"


class ParserConfig
{
    private:
        ParserConfig(void);
        ParserConfig(ParserConfig const &);
        ParserConfig &operator=(ParserConfig const &);
        
        static bool                        removeSemicolon(std::vector<std::string> &tokens);
        

    public:
    
        ~ParserConfig();
        static std::vector<std::string> form_inst_line(std::ifstream &);
        static bool check_block(std::ifstream &, std::vector<Server*> &);
        static bool check_location_block(std::ifstream &, std::vector<std::string> const &, Server *, Location *);
        
        
};

#endif