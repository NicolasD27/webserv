/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 09:38:11 by clorin            #+#    #+#             */
/*   Updated: 2022/01/06 09:46:08 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_UTILS_HPP
# define FILE_UTILS_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

bool		pathIsFile(const std::string &);
bool        pathIsDir(const std::string &);

#endif