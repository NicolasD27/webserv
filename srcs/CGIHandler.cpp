/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 22:40:48 by clorin            #+#    #+#             */
/*   Updated: 2022/02/06 16:35:10 by clorin           ###   ########.fr       */
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

	CGIHandler::CGIHandler(Request const *request, Response const *response)
{
	std::map<std::string, std::string>	headers = response->getHeaders();
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["REQUEST_METHOD"] = request->getHttpMethod();
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["AUTH_TYPE"] = "";
	_env["CONTENT_TYPE"] = headers["Content-type"];
	_env["PATH_INFO"] = response->getRessourcePath();
	_env["QUERY_STRING"] = request->getQueryString();
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

	std::cout << "Vars dans CGI : \n";
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); ++it)
    {
        std::cout << "\t"<< C_YELLOW << it->first << C_RESET<<": "<< C_CYAN << it->second << C_RESET<< std::endl;
    }
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