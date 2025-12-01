/* Drew Schlesener
 * Denton Jarvis
 * Kevin Cunningham
*/
#include <iostream>
#include <boost/process.hpp> // -lboost_system -lpthread
#include <string>
using namespace std;

namespace bp = boost::process;

int main(int argc, char** argv) 
{
    char buffer[20];
    for (int i = 0; i < atoi(argv[1]); i++)
    {
        snprintf(buffer, sizeof(buffer), "./parser %d", i);
        bp::ipstream pipe_stream;
        std::string sBuffer = buffer;
        bp::child Process(sBuffer, bp::std_out > pipe_stream);

        std::string line;
        while (pipe_stream && std::getline(pipe_stream, line) && !line.empty()) 
        {
            std::cout << line << std::endl;
        }

        Process.wait();
        std::cout << "Exit code: " << Process.exit_code() << std::endl;
    }
    
    return 0;
}