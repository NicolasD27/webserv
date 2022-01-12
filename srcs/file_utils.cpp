/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 09:35:45 by clorin            #+#    #+#             */
/*   Updated: 2022/01/10 11:53:22 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.hpp"

bool		pathIsFile(const std::string & path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0 )
	{
		if (s.st_mode & S_IFDIR)
			return false;
		else if (s.st_mode & S_IFREG)
			return true;
		else
			return false;
	}
	else
		return false;
}

bool        pathIsDir(const std::string & path)
{
	struct stat s;

	if (stat(path.c_str(), &s) == 0 )
        return (! ((s.st_mode & S_IFDIR) == 0));
    else
		return false;
}

std::string getWorkingPath(void)
{
	char temp[MAXPATHLEN];
	return ((getcwd(temp, sizeof(temp))? std::string(temp) : std::string("")));
}