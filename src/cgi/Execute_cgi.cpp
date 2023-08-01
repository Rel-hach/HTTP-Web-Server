#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "../../inc/response.hpp"

char** vector_to_char(std::vector<std::string> env_cgi) {
    size_t i;
    char** env = (char**)malloc((env_cgi.size() + 1) * sizeof(char*));
    if (env == NULL) {
        std::cerr<<"Memory allocation failed for env"<<std::endl;
        return NULL;
    }
    for (i = 0; i < env_cgi.size(); i++) {
        env[i] = (char*)malloc((env_cgi[i].size() + 1) * sizeof(char));
        if (env[i] == NULL) {
            std::cerr<<"Memory allocation failed for env[i]"<<std::endl;
            for (size_t j = 0; j < i; j++) {
                free(env[j]);
            }
            free(env);
            return NULL;
        }
        strcpy(env[i], env_cgi[i].c_str());
    }
    env[i] = NULL;
    return env;
} 
// location/file.py
//delete file
//session
//coockise
//php
//script
int Execute_cgi(response &res) {
    std::vector<std::string> env_cgi;
    std::cout<<"cgi"<<std::endl;
    env_cgi.push_back("QUERY_STRING="+ res._querry);
    char ** _env = vector_to_char(env_cgi);
    std::string path;
    std::string namecgi;
    if(res._extension == ".php")
    {
        path = "/usr/bin/php";
        namecgi = "php";
    }else if(res._extension == ".py")
    {
        path = "/usr/bin/python3";
        namecgi = "python3";
    }
    int pipefd[2];
    pid_t pid;
    if (pipe(pipefd) == -1) {
        std::cout<<"pipe failed"<<std::endl;
        return 1;
    }
    pid = fork();
    if (pid < 0) {
        std::cout<<"fork failed"<<std::endl;
        return 1;
    } else if (pid == 0) 
    {
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO);
        char* argv[]={(char*)namecgi.c_str(),(char*)(res._realPath).c_str(),nullptr};
        execve(path.c_str(),argv,_env);
        exit(127);
    } 
    else 
    {   
        close(pipefd[1]);
        int status;
        pid_t child_pid = waitpid(pid, &status, 0);
        if (child_pid == -1) {
            std::cerr << "Error: waitpid failed." << std::endl;
            return 1;
        } 
        if (status == 0) {
            char buffer[1024000];
            memset(buffer,'\0',1024000);
            int num_bytes = read(pipefd[0], buffer, sizeof(buffer));
            if (num_bytes == -1) {
                std::cerr<<"read failed"<<std::endl;
                return 1;
            }
            res._fileContent = buffer;
            res._contentLength = res._fileContent.length();
            res._contentType = "text/html";
            return 200;
        }   
        std::cout<<"time"<<std::endl;  
    }
    return 504;
}



