#include "client.hpp"

int main() {
    Client client;
    
    if (client.init()) {
        client.run();
    }
    
    return 0;
}