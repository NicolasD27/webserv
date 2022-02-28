/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 09:35:45 by clorin            #+#    #+#             */
/*   Updated: 2022/02/28 09:07:49 by clorin           ###   ########.fr       */
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


std::string getLastModifiedDate(std::string & filename)
{
	struct stat result;
	struct tm * mod_time;
	char buffer [80];
	
	if(stat(filename.c_str(), &result)==0)
	{
		time_t lmtime = result.st_mtime;
		mod_time = localtime(&lmtime);
		strftime (buffer,80,"%a, %d %b %Y %X %Z",mod_time);
		return std::string(buffer);
	}
	return "";
}

bool  compareByName(t_file &a, t_file &b)
{
    return (a.name < b.name);
}

t_file		info(std::string const & path)
{
	struct stat sb;
	t_file 		file;

	if (stat(path.c_str(), &sb) == -1)
	{
       perror("stat");
	   return file;
    }
	else
	{
		file.type = sb.st_mode & S_IFMT;
		file.size = sb.st_size;
		file.modif = ctime(&sb.st_mtime);
	}
	return file;
}