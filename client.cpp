#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <fstream>

using namespace std;

// Función para obtener el puerto configurado en el archivo config.txt
int getPort()
{
    ifstream config("config.txt");
    int port = 8080; // Puerto por defecto en caso de que no se encuentre el archivo
    if (config)
    {
        config >> port; // Lee el puerto del archivo
    }
    return port;
}

int main()
{
    int clientSocket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Crear el socket del cliente
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        perror("Error al crear el socket");
        return -1;
    }

    // obtiene puerto del txt
    int port = getPort();
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // Convertir a formato de red

    // Convertir la dirección IP de texto a binario
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
    {
        perror("Dirección inválida");
        return -1;
    }

    // Intentar conectar con el servidor
    if (connect(clientSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Fallo en la conexión");
        return -1;
    }

    // pide el nombre del user
    string name;
    cout << "Ingrese su nombre de usuario:";
    getline(cin, name);
    send(clientSocket, name.c_str(), name.size(), 0); // Enviar el nombre al servidor

    if (fork() == 0)
    {
        while (true)
        {
            memset(buffer, 0, 1024);
            int bytes_received = recv(clientSocket, buffer, 1024, 0);
            if (bytes_received <= 0)
            {
                // Si el servidor cierra la conexión o hay un error, terminamos el proceso hijo
                cout << "Conexión cerrada por el servidor";
                close(clientSocket);
                exit(0);
            }
            cout << "Mensaje recibido: " << buffer << endl;
        }
        exit(0);
    }

    // bucle send msj
    while (true)
    {
        string To, mensaje;
        cout << "Para:";
        getline(cin, To);
        cout << "Mensaje:";
        getline(cin, mensaje);

        // formato del msj = "destinatario:mensaje"
        string full_msg = To + ":" + mensaje;
        send(clientSocket, full_msg.c_str(), full_msg.size(), 0);
    }

    // cierra socket
    close(clientSocket);
    return 0;
}