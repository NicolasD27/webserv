/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 22:40:48 by clorin            #+#    #+#             */
/*   Updated: 2022/03/03 12:20:08 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"

CGIHandler::~CGIHandler()
{
	size_t i = 0;
	while(_envChar[i])
		free(_envChar[i++]);
	free (_envChar);
}

CGIHandler::CGIHandler()
{
	std::map<std::string, std::string> _env;
}

CGIHandler & CGIHandler::operator=(CGIHandler const & src)
{
	_body = src._body;
	_env = src._env;
	_script = src._script;
	_file = src._file;
	this->setEnvChar();
	return *this;
}
/*
 - Server related variables
    	SERVER_SOFTWARE
    	SERVER_NAME
    	GATEWAY_INTERFACE	
 - Query-specific variables:
		SERVER_PROTOCOL
			Le nom et la révision du protocole dans lequel la requête a été faite (Format : protocole/révision)
		SERVER_PORT
			Le numéro de port sur lequel la requête a été envoyée.
		REQUEST_METHOD
			La méthode utilisée pour faire la requête. Pour HTTP, elle contient généralement « GET » ou « POST ».
		PATH_INFO
			Le chemin supplémentaire du script tel que donné par le client.
			Par exemple, si le serveur héberge le script « /cgi-bin/monscript.cgi » et que le client demande l'url 
			« http://serveur.org/cgi-bin/monscript.cgi/marecherche », alors PATH_INFO contiendra « marecherche ».
		PATH_TRANSLATED
			Contient le chemin demandé par le client après que les conversions virtuel → physique ont été faites par le serveur.
		SCRIPT_NAME
			Le chemin virtuel vers le script étant exécuté. Exemple : « /cgi-bin/script.cgi »
		QUERY_STRING
			Contient tout ce qui suit le « ? » dans l'URL envoyée par le client.
			Toutes les variables provenant d'un formulaire envoyé avec la méthode « GET » seront contenues dans le QUERY_STRING sous la forme
			« var1=val1&var2=val2&... ».
		REMOTE_ADDR
			L'adresse IP du client.
		AUTH_TYPE
			Le type d'identification utilisé pour protéger le script (s’il est protégé et si le serveur supporte l'identification).
		REMOTE_USER
			Le nom d'utilisateur du client, si le script est protégé et si le serveur supporte l'identification.
		REMOTE_IDENT
			Nom d'utilisateur (distant) du client faisant la requête. Le serveur doit supporter l'identification RFC 931.
			Cette variable devrait être utilisée à des fins de journaux seulement.
		CONTENT_TYPE
			Le type de contenu attaché à la requête, si des données sont attachées (comme lorsqu'un formulaire est envoyé avec la méthode « POST »).
		CONTENT_LENGTH
			La longueur du contenu envoyé par le client.
		

		REQUEST_URI
			dans l'exemple audessus il semblerait que cela soit : /cgi-bin/monscript.cgi/marecherche avec les ?var=DD si GET

*/

	CGIHandler::CGIHandler(Request const *request, Response *response, std::string script, std::string file):
	_response(response),
	_body(request->getBody()),
	_script(script),
	_file(file),
	_envChar(NULL)
{
	std::map<std::string, std::string>	headers_response = response->getHeaders();
	std::map<std::string, std::string>	headers_request = request->getHeaders();
	Location							*location = response->getLocationBlock();

	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "webserv";
	_env["SERVER_NAME"] = response->getHost();
	_env["SERVER_PORT"] = to_string(response->getPort());
	_env["REQUEST_METHOD"] = request->getHttpMethod();
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["AUTH_TYPE"] = "";
	_env["CONTENT_TYPE"] = headers_request["Content-Type"];
	_env["PATH_INFO"] = response->getRessourcePath();
	_env["PATH_TRANSLATED"] = response->getRessourcePath(); 
	_env["QUERY_STRING"] = request->getQueryString();
	_env["CONTENT_LENGTH"] = headers_request["Content-Length"];
	_env["REMOTE_ADDR"] = request->getAddressClient();
	_env["REMOTE_PORT"] = to_string(request->getPortClient());
	_env["REMOTE_IDENT"] = headers_request["Authorization"];
	_env["REMOTE_USER"] = headers_request["Authorization"];
	_env["SCRIPT_NAME"] = response->getRessourcePath();
	_env["REQUEST_URI"] = response->getRessourcePath();
	_env["REDIRECT_STATUS"] = "200";
	_env["UPLOAD_DIR"] = location->getUploadDir();
	this->setEnvChar();
 }

void					CGIHandler::setEnvChar()
{
	_envChar = new char*[this->_env.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = this->_env.begin(); i != this->_env.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		_envChar[j] = new char[element.size() + 1];
		_envChar[j] = strcpy(_envChar[j], (const char*)element.c_str());
		j++;
	}
	_envChar[j] = NULL;
}

size_t                  CGIHandler::size() const
{
    return this->_env.size();
}

std::string		CGIHandler::executeCgi(unsigned int *status)
{
	pid_t		pid;
	int			saveStdin;
	int			saveStdout;
	std::string newBody = "";
	const char *scriptName[3] = {_script.c_str(), _file.c_str() ,NULL};
	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	saveStdin = dup(STDIN_FILENO);
	saveStdout = dup(STDOUT_FILENO);

	FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
	int		ret = 1;
	int		exit_status = 0;
	*status = 200;

	// std::cout << "Vars dans CGI : \n";
	// for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); ++it)
    // {
    //     std::cout << "\t"<< C_YELLOW << it->first << C_RESET<<": "<< C_CYAN << it->second << C_RESET<< std::endl;
    // }
	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);
	std::cout << "execution de "<< scriptName[0] << " with " << scriptName[1] << std::endl;
    
	pid = fork();

	if (pid == -1)
	{
		std::cerr << "Fork crashed." << std::endl;
		*status = 500;
		return ("Status: 500\r\n\r\n");
	}
	else if (pid == 0)
	{
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(scriptName[0], const_cast<char* const *>(scriptName), _envChar);
		std::cerr << "Execve crashed." << std::endl;
		exit_status = -1;
    }
	else
	{
		int			status_exited;
		waitpid(-1, &status_exited, 0);
		if (WIFEXITED(status_exited))
		{
				if(WEXITSTATUS(status_exited))
					*status = 500;
		}
		_response->CGIReady(fdOut, fOut);
	}

	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	fclose(fIn);
	close(fdIn);
	
	close(saveStdin);
	close(saveStdout);
	if (!pid)
		exit(exit_status);
	return (newBody);
}