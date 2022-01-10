/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 12:46:39 by clorin            #+#    #+#             */
/*   Updated: 2022/01/10 13:48:59 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGES_HPP
# define ERRORPAGES_HPP

#include <string>
#include <map>

# define PAGE403    "<h1>Forbidden</h1><p>You don't have permission to access this resource.</p>"
# define PAGE404    "<h1 style='color:red'>Error 404: Not found :-(</h1><p>I have no idea where that file is, sorry.  Are you sure you typed in the correct URL?</p>"
# define PAGE500    "<h1>Oops! Something went wrong...</h1><p>We seem to be having some technical difficulties. Hang tight.</p>"


class ErrorPages
{
    private:
        ErrorPages(ErrorPages const &){};
        ErrorPages &operator=(ErrorPages const &){return *this;};

        std::map<unsigned int, std::string>      _errorPage;

    public:
        ErrorPages(void);
        ~ErrorPages(void);
        
        std::string     &operator[](unsigned int);
};

#endif