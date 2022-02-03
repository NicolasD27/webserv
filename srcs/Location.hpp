/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:45:49 by clorin            #+#    #+#             */
/*   Updated: 2022/01/20 18:12:51 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Colors.hpp"
#include "utility.hpp"

class Location
{
    private:
        std::string                 _path;
        std::vector<std::string>    _index;
        std::string                 _root;
        bool                        _autoIndex;
        std::vector<std::string>    _methods;
        std::string                 _cgi_path;
        std::vector<std::string>    _extensions;

    public:
        Location(void){};
        Location(std::string path, std::vector<std::string> index, std::string root, bool autoIndex, std::vector<std::string> methods, std::string);
        ~Location(void);
        Location(Location const &);
        Location &operator=(Location const &);

        void                        storeLine(std::string const & , std::string &);
        void                        addMethods(std::vector<std::string> );
        void                        addIndex(std::vector<std::string> );
        void                        addCgi(std::vector<std::string> );

        std::string                 getPath(void)const;
        std::vector<std::string>    getIndex(void)const;
        std::string                 getRoot(void)const;
        std::string                 getCgiPath(void)const;
        bool                        isAutoindex(void)const;
    
        void                        setPath(std::string &);
        void                        setAutoIndex(bool);

        bool                        isValid(void) const;
        bool                        hasMethod(std::string) const;
        bool                        hasExtension(std::string) const;

        void                        print(void) const;
};
#endif