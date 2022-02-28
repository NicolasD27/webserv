/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 09:38:11 by clorin            #+#    #+#             */
/*   Updated: 2022/02/27 17:07:55 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_UTILS_HPP
# define FILE_UTILS_HPP

#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>
#include <sys/param.h>

typedef struct		s_file
{
	std::string     name;
    long long       size;
    std::string     modif;
    int             type;
}					t_file;

bool		pathIsFile(const std::string &);
bool        pathIsDir(const std::string &);
std::string getWorkingPath(void);
std::string getLastModifiedDate(std::string & filename);
t_file      info(std::string const &);

#endif