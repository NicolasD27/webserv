/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 10:58:14 by clorin            #+#    #+#             */
/*   Updated: 2022/01/10 11:07:31 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUSCODE_HPP
# define STATUSCODE_HPP

#include <string>
#include <map>

class StatusCode
{
    private:
        StatusCode(StatusCode const &){};
        StatusCode &operator=(StatusCode const &){return *this;};
        
        std::map<unsigned int, std::string>      _err_code;

    public:
         StatusCode(void);
         ~StatusCode(void){};
         
        std::string     &operator[](unsigned int);
};

#endif