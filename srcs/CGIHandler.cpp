/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 22:40:48 by clorin            #+#    #+#             */
/*   Updated: 2022/02/06 10:25:03 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::~CGIHandler(){}

// void    CGIHandler::initEnv()
// {
//     _env["REQUEST_METHOD"] = "GET";//ok
//     _env["SERVER_PROTOCOL"] = "HTTP/1.1";
// 	_env["SERVER_SOFTWARE"] = "Webserv/1.0";
//     _env["COMSPEC"] = "";
//     _env["DOCUMENT_ROOT"] = "";
//     _env["GATEWAY_INTERFACE"] = "CGI/1.1";//ok
//     _env["HTTP_ACCEPT"] = "";
//     _env["HTTP_ACCEPT_ENCODING"] = "";
//     _env["HTTP_ACCEPT_LANGUAGE"] = "";
//     _env["HTTP_CONNECTION"] = "";
//     _env["HTTP_HOST"] = "";
//     _env["HTTP_USER_AGENT"] = "";
//     _env["PATH"] = "";
// 	_env["PATH_INFO"] = "";	//ok
//     _env["QUERY_STRING"] = "say=Salut&to=Maman"; //ok
//     _env["REMOTE_ADDR"] = "";//ok
//     _env["REMOTE_PORT"] = "";
//     _env["REQUEST_URI"] = "";//ok
//     _env["SCRIPT_FILENAME"] = "";
//     _env["SCRIPT_NAME"] = "";//ok
//     _env["SERVER_ADDR"] = "";
//     _env["SERVER_SIGNATURE"] = "";
//     _env["SERVER_ADMIN"] = "clorin@student.42.fr";
//     _env["SERVER_NAME"] = "webserv";
//     _env["SERVER_PORT"] = "3000";
// }

/*
 Your program should set the follo
AUTH_TYPE
CONTENT_LENGTH
CONTENT_TYPE
GATEWAY_INTERFACE
PATH_INFO
PATH_TRANSLATED
QUERY_STRING
REMOTE_ADDR
REMOTE_IDENT
REMOTE_USER
REQUEST_METHOD ok
REQUEST_URI
SCRIPT_NAME

*/

	CGIHandler::CGIHandler(Request const *request, std::map<std::string, std::string>&headers)
{
	_env["REQUEST_METHOD"] = request->getHttpMethod();
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["AUTH_TYPE"] = "";
	std::cout << "HEADERS dans CGI : \n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
    {
        std::cout << "\t"<< C_YELLOW << it->first << C_RESET<<": "<< C_CYAN << it->second << C_RESET<< std::endl;
    }
}

char					**CGIHandler::getEnv() const
{
	char	**env = new char*[this->_env.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = this->_env.begin(); i != this->_env.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}

size_t                  CGIHandler::size() const
{
    return this->_env.size();
}

std::string		CGIHandler::executeCgi(const char **scriptName, const std::string & body)
{
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	std::string	newBody = body;

	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;

	pid = fork();

	if (pid == -1)
	{
		std::cerr << "Fork crashed." << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (pid == 0)
	{
        std::cout << "execution de "<<scriptName[0] << " with ";
        
		char **env;
		//pour cgi tester
		_env["PATH_INFO"] = "/www/html/rep/hello.php";
        env = this->getEnv();		//todo free()
		size_t i = 1;
        while(scriptName[i])
            std::cout << scriptName[i++] << " ";
        std::cout << std::endl;
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(scriptName[0], const_cast<char* const *>(scriptName), env);
		std::cerr << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
    }
	else
	{
		char	buffer[1024] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 1024);
			ret = read(fdOut, buffer, 1024 - 1);
			newBody += buffer;
		}
	}

	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	fclose(fOut);
	close(fdIn);
	close(fdOut);
	close(saveStdin);
	close(saveStdout);
	if (!pid)
		exit(0);

	return (newBody);
}