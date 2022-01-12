/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:45:49 by clorin            #+#    #+#             */
/*   Updated: 2022/01/12 09:43:27 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <string>
#include <iostream>
#include <vector>

class Location
{
    private:
        std::string                 _path;
        std::string                 _index;
        std::string                 _root;
        bool                        _autoIndex;
        std::vector<std::string>    _methods;

    public:
        Location(void){};
        ~Location(void);
        Location(Location const &);
        Location &operator=(Location const &){return *this;};

        void            storeLine(std::string & , std::string &);
        void            addMethods(std::vector<std::string> &);

        std::string     getPath(void)const;
        std::string     getIndex(void)const;
        std::string     getRoot(void)const;
        bool            isAutoindex(void)const;
    
        void            setPath(std::string &);
        void            setAutoIndex(bool);

        void            print(void) const;
};
#endif