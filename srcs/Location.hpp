/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:45:49 by clorin            #+#    #+#             */
/*   Updated: 2022/01/17 10:26:40 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Colors.hpp"

class Location
{
    private:
        std::string                 _path;
        std::vector<std::string>    _index;
        std::string                 _root;
        bool                        _autoIndex;
        std::vector<std::string>    _methods;

    public:
        Location(void){};
        Location(std::string path, std::vector<std::string> index, std::string root, bool autoIndex, std::vector<std::string> methods);
        ~Location(void);
        Location(Location const &);
        Location &operator=(Location const &);

        void                        storeLine(std::string const & , std::string &);
        void                        addMethods(std::vector<std::string> );
        void                        addIndex(std::vector<std::string> );

        std::string                 getPath(void)const;
        std::vector<std::string>    getIndex(void)const;
        std::string                 getRoot(void)const;
        bool                        isAutoindex(void)const;
    
        void                        setPath(std::string &);
        void                        setAutoIndex(bool);

        bool                        isValid(void) const;

        void                        print(void) const;
};
#endif