/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimesType.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 11:55:18 by clorin            #+#    #+#             */
/*   Updated: 2022/01/10 12:44:37 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMESTYPE_HPP
# define MIMESTYPE_HPP

#include <string>
#include <map>

class MimesType
{
    private:
        MimesType(MimesType const &){};
        MimesType &operator=(MimesType const &){return *this;};
        
        std::map<std::string, std::string>      _mimeType;

    public:
        MimesType(void);
        ~MimesType(void);

        const std::string     getType(std::string type);
        bool                  typeExist(std::string type);
};
#endif