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
//         char* argv[]={(char*)"php",(char*)"file.php",nullptr};
//         execve("/usr/bin/php",argv,nullptr);
//         std::cerr<<"execve failed"<<std::endl;
//         exit(0);
//     }
//     else
//         waitpid(pid,0,0);
    

// }