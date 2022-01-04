std::string		Client::executeCgi(const char **scriptName, const std::string & body, char **env) {
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
        std::cout << "execution de "<<scriptName[0]<< std::endl;
		const char **arg = &scriptName[1];

		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
        
		execve(scriptName[0], const_cast<char* const *>(scriptName), env);
		std::cerr << "Execve crashed." << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char	buffer[CGI_BUFSIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, CGI_BUFSIZE);
			ret = read(fdOut, buffer, CGI_BUFSIZE - 1);
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