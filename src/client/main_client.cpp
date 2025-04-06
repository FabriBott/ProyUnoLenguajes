#include "../../include/client.hpp"

// int main() {
//     Client client;
    
//     if (client.init()) {
//         client.run();
//     }
    
//     return 0;
// }

int main() {
    {
        Client client;
        if (client.init()) {
            client.run();
        }
    } // Client destructor is called here due to scope ending
    
    return 0;
}
