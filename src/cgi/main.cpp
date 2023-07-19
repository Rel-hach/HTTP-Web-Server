// #include "cgi.hpp"

// int main(int argc , char **argv , char **env)
// {

    
//     pid_t pid = fork();
//     if(pid== -1)
//     {
//         std::cerr<<"Fork failed"<<std::endl;
//         return 1;
//     }
//     if(pid == 0)
//     {
//         char* argv[]={(char*)"python3",(char*)"test1.py",nullptr};
//         execve("/usr/bin/python3",argv,nullptr);
//         std::cerr<<"execve failed"<<std::endl;
//         exit(0);
//     }
//     else
//         waitpid(pid,0,0);
    

// }