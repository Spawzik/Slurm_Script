#include <iostream>
#include <boost/process.hpp>
#include <string>
using namespace std;

namespace bp = boost::process;

int main(int argc, char** argv) 
{
    char buffer[20];
    for (int i = 0; i < atoi(argv[1]); i++)
    {
        snprintf(buffer, sizeof(buffer), "./parser %d", i);
        // printf("%s", buffer);
        bp::ipstream pipe_stream;
        std::string sBuffer = buffer;
        // std::cout << sBuffer << std::endl;
        bp::child c(sBuffer, bp::std_out > pipe_stream);

        std::string line;
        while (pipe_stream && std::getline(pipe_stream, line) && !line.empty()) 
        {
            std::cout << line << std::endl;
        }

        c.wait();
        std::cout << "Exit code: " << c.exit_code() << std::endl;
    }
    
    return 0;
}