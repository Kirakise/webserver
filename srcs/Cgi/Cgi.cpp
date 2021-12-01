#include "Cgi.hpp"
#include <string>
#include <stdio.h>
#include <unistd.h>


Cgi::Cgi(Response res) : res(res), body(res.body), conf(res.Conf), req(res.pars), headers(res.pars._headers){}
Cgi::~Cgi(){}


void Cgi::make_env(){
    env["CONTENT_LENGTH"] = std::to_string(body.length()); //c++ 11, replace?
    env["SERVER_SOFTWARE"] = "Webserv/1.0";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_PORT"] = std::to_string(conf.getPort());
    env["REQUEST_METHOD"] = req.getMethod();
    env["PATH_INFO"] = req.getPath();
    env["PATH_TRANSLATED"] = req.getPath();
    env["SCRIPT_NAME"] = conf.cgi_pass;
    // env["QUERY_STRING"] = ; //???????????????
    env["REMOTE_ADDR"] = conf.host;
    env["REMOTE_USER"] = headers["Authorization"];
    env["REMOTE_IDENT"] = headers["Authorization"];
    env["AUTH_TYPE"] = headers["Authorization"];
    env["SERVER_NAME"] = headers["Hostname"];
    env["CONTENT_TYPE"] = headers["Content-Type"];
}

void Cgi::env_to_array()
{
    make_env();
    char * env_arr[env.size()];
    size_t j = 0;
    std::string s;
    for (std::map<std::string, std::string>::iterator i = env.begin(); i != env.end(); i++){
        s = i->first + "=" + i->second;
        env_arr[j] = strdup(s.c_str());
        j++;
    }
    env_ready = env_arr;
}

// void Cgi::startCgi(){
//     int         save0;
//     int         save1;
//     std::string res;
//     pid_t       pid;
//     std::string cgi = this->conf.cgi_pass;
//     int ret = 1;

//     env_to_array();
//     std::string s;
//     s = this->conf.cgi_pass;

//     char **args;
//     args[0] = (char *)s.c_str();
//     args[1] = (char *)req.path.c_str();
//     std::cout << args[1] << std::endl;
//     args[2] = NULL;

//     FILE *fin = tmpfile();
//     FILE *fout = tmpfile();
//     long fdin = fileno(fin);
//     long fdout = fileno(fout);
//     save0 = dup(0);
//     save1 = dup(1);

//     pid = fork();

//     if (pid == -1){
//         body = "Status: 500\r\n\r\n";
//         return;
//     }
//     else if (pid == 0){
//         dup2(fdin, 0);
//         dup2(fdout, 1);
//         execve(cgi.c_str(), NULL, env_ready);
//         std::cerr << "Execve error" << std::endl;
//         write(1, "Status: 500\r\n\r\n", 16);
//     }
  
// }

    // for(size_t i = 0; env_ready[i]; i++)
    //     delete [] env_ready[i];
    // // delete[] env_ready;

    // if (!pid)
    //     exit(0);
    // body = res;

    // int fd;
    // FILE *file = tmpfile();
    // fd = fileno(file);
    // int tubes[2];
    // pipe(tubes);
    // pid_t pid;

    // char **args;
    // args[0] = (char *)res.Conf.cgi_pass.c_str();
    // args[1] = (char *)req.path.c_str();
    // args[2] = NULL;

    // env_to_array();

    // if ((pid = fork() == 0)){
    //     close(tubes[1]);
    //     dup2(tubes[0], 0);
    //     execve(args[0], args, env_ready);
    // }
    // else{

    // }

void Cgi::startCgi(){
    pid_t pid;
    int fdout = open("file", O_CREAT | O_TRUNC | O_WRONLY, 0777);
	// if (head->fdin != -1)
	// 	dup2(head->fdin, 0);
	// if (head->fdout != -1)
	// 	dup2(head->fdout, 1);
	// if (is_builtin(head->args[0]) != -1)
	// 	return (do_builtin(head));

    char **args = (char **)malloc(sizeof(char *) * 3);
    args[0] = (char *)res.Conf.cgi_pass.c_str();
    args[1] = (char *)req.path.c_str();
    args[2] = NULL;
    int fd = dup(1);

	pid = fork();
	dup2(fdout, 1);
	if (pid == 0){
        close(1);
        system((char *)(this->conf.cgi_pass + " " + req.path + " > file").c_str());
        // execve(args[0], args, env_ready);
        exit(-1);
    }
	else
		waitpid(pid, 0, 0);
    dup2(fd, 1);
    close(fdout);
    std::string q = "file";
    body = readFile(q);
}
 

// void Cgi::startCgi(){
//     pid_t		pid;
// 	int			saveStdin;
// 	int			saveStdout;
// 	char		**env;
// 	std::string	newBody;

// 	this->env_to_array();

// 	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
// 	saveStdin = dup(STDIN_FILENO);
// 	saveStdout = dup(STDOUT_FILENO);

// 	FILE	*fIn = tmpfile();
// 	FILE	*fOut = tmpfile();
// 	long	fdIn = fileno(fIn);
// 	long	fdOut = fileno(fOut);
// 	int		ret = 1;

// 	write(fdIn, req.body.c_str(), req.body.size());
// 	lseek(fdIn, 0, SEEK_SET);

//         char **args = (char **)malloc(sizeof(char *) * 3);
//     args[0] = (char *)res.Conf.cgi_pass.c_str();
//     // args[1] = (char *)req.path.c_str();
//     args[1] = NULL;
//     args[2] = NULL;

// 	pid = fork();

// 	if (pid == -1)
// 	{
// 		std::cerr <<  "Fork crashed."  << std::endl;
// 		newBody = "Status: 500\r\n\r\n";
// 	}
// 	else if (!pid)
// 	{
// 		char * const * nll = NULL;

// 		dup2(fdIn, STDIN_FILENO);
// 		dup2(fdOut, STDOUT_FILENO);

//         system((char *)(this->conf.cgi_pass + " " + req.path).c_str());
// 		// execve(this->conf.cgi_pass.c_str(), NULL, env);
// 		// std::cerr <<  "Execve crashed."  << std::endl;
// 		// write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
// 	}
// 	else
// 	{
// 		char	buffer[1000] = {0};

// 		waitpid(-1, NULL, 0);
// 		lseek(fdOut, 0, SEEK_SET);

// 		ret = 1;
// 		while (ret > 0)
// 		{
// 			memset(buffer, 0, 1000);
// 			ret = read(fdOut, buffer, 1000 - 1);
// 			newBody += buffer;
// 		}
// 	}

// 	dup2(saveStdin, STDIN_FILENO);
// 	dup2(saveStdout, STDOUT_FILENO);
// 	fclose(fIn);
// 	fclose(fOut);
// 	close(fdIn);
// 	close(fdOut);
// 	close(saveStdin);
// 	close(saveStdout);

// 	// for (size_t i = 0; env[i]; i++)
// 	// 	delete[] env[i];
// 	// // delete[] env;

// 	if (!pid)
// 		exit(0);

// 	std::cout << newBody << std::endl;
//     this->body = newBody;
// }