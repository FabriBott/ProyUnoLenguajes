
#include "../../include/server.hpp"


int main() {
    Server server;
    
    if (server.init()) {
        server.run();
    }
    
    return 0;
}
