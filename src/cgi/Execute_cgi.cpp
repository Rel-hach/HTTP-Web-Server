#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
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
int Execute_cgi(response &res) {
    std::vector<std::string> env_cgi;
    // res._method
     std::cout<<"Child process exited abnormally"<<std::endl;
    env_cgi.push_back("name=mohamed");//set env
    // res._extension
    char **_env = vector_to_char(env_cgi);
    int pipefd[2];
    int child_status;
    pid_t pid;

    if (pipe(pipefd) == -1) {
        std::cout<<"pipe failed"<<std::endl;
        return 1;
    }
    pid = fork();
    if (pid < 0) {
        std::cout<<"fork failed"<<std::endl;
        return 1;
    } else if (pid == 0) {
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO);
        if(res._extension== ".php")
        {
            char* argv[]={(char*)"php",(char*)"file.php",nullptr};
            execve("/usr/bin/php",argv,_env);
        }
        if(res._extension== ".py")
        {
            char* argv[]={(char*)"python3",(char*)"test1.py",nullptr};
            execve("/usr/bin/python3",argv,_env);
        }
        std::cerr<<"execve failed"<<std::endl;
        exit(0);
    } else {       
        close(pipefd[1]);
        char buffer[1024];
        int num_bytes = read(pipefd[0], buffer, sizeof(buffer));
        if (num_bytes == -1) {
            std::cerr<<"execve failed"<<std::endl;
            return 1;
        }
        buffer[num_bytes] = '\0';
        wait(&child_status);
// 504 Gateway Timeout
        if (WIFEXITED(child_status)) {
            std::cout<<buffer<<std::endl;
            if(std::string(buffer) == "execve failed")
            {
                return 500;
            }
            else
            {
                res._fileContent = buffer;
                return 200;
            }
        } else {
            std::cout<<"Child process exited abnormally"<<std::endl;
        }
    }

    return 0;
}



