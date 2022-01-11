/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:45:49 by clorin            #+#    #+#             */
/*   Updated: 2022/01/11 15:33:10 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <string>
#include <iostream>


class Location
{
    private:
        std::string     _path;
        std::string     _index;
        bool            _autoIndex;

    public:
        Location(void){};
        ~Location(void){};
        Location(Location const &);
        Location &operator=(Location const &){return *this;};

        void            storeLine(std::string & , std::string &);

        std::string     getPath(void)const;
        std::string     getIndex(void)const;
        bool            isAutoindex(void)const;
    
        void            setPath(std::string &);
        void            setAutoIndex(bool);

        void            print(void) const;
};
#endif