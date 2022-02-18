/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LanguageCodes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 11:55:18 by clorin            #+#    #+#             */
/*   Updated: 2022/01/10 12:44:37 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LANGUAGE_CODES_HPP
# define LANGUAGE_CODES_HPP

#include <string>
#include <vector>

class LanguageCodes
{
    private:
        LanguageCodes(LanguageCodes const &){};
        LanguageCodes &operator=(LanguageCodes const &){return *this;};
        
        std::vector<std::string>      _codes;

    public:
        LanguageCodes(void);
        ~LanguageCodes(void);

        std::vector<std::string>     getCodes();
        bool langExist(std::string code);

};
#endif