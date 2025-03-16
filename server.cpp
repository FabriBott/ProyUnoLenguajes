#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <fstream>

using namespace std;

// mapa que guarda el nombre del user y el socket al que esta conectado
map<string, int> users;

// sacar el puerto del txt de config
int getPort()
{
    ifstream config("config.txt"); // abre txt
    int port = 8080;               //
    if (config)
    {
        config >> port; // pone en port el puerto
    }
    return port;
}

// Maneja la comunicación con un cliente
void ComClient(int cSocket)
{
    char buffer[1024];
    while (true)
    {                     // bucle infinito (para msjs)
        buffer[0] = '\0'; // Limpia el buffer

        int bytes_received = recv(cSocket, buffer, 1024, 0);
        if (bytes_received <= 0)
        { // Si no recibe datos, cierra la conexión
            close(cSocket);
            return;
        }

        string msg(buffer);         // Convierte el mensaje a string
        size_t pos = msg.find(":"); // Busca el separador entre usuario y mensaje
        if (pos != string::npos)
        {
            string recipient = msg.substr(0, pos); // Extrae el destinatario
            string message = msg.substr(pos + 1);  // Extrae el mensaje

            if (users.find(recipient) != users.end())
            { // ve que el otro user exiat y manda msj
                send(users[recipient], message.c_str(), message.size(), 0);
            }
        }
    }
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int port = getPort(); // Obtiene el puerto de config.txt

    // Crea el socket del servidor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Error al crear el socket");
        return -1;
    }

    // Configura la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Asigna el socket a la dirección y puerto
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error al enlazar el socket");
        return -1;
    }

    // server pasa a modo listen
    listen(server_socket, 5);
    cout << "Servidor en ejecución en el puerto" << port << endl;

    while (true)
    {
        // Acepta conexiones entrantes de clientes
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0)
        {
            perror("Error al aceptar la conexión");
            continue;
        }

        // se recibe el nombre y lo guarda en el map
        char name[1024];
        recv(client_socket, name, 1024, 0);
        users[string(name)] = client_socket;
        cout << "Usuario registrado: " << name << endl;

        // Crea un proceso hijo para manejar al cliente
        if (fork() == 0)
        {
            ComClient(client_socket); // Maneja la comunicación con el cliente
            exit(0);
        }
    }

    close(server_socket); // Cierra el socket del servidor cuando termina
    return 0;
}