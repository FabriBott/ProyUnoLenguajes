// Definiciones comunes

#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>

// Definiciones comunes
#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 7777
#define DEFAULT_CLIENT_PORT 8888
#define SERVER_IP "127.0.0.1"

// Colores para la consola
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"

using json = nlohmann::json;
using namespace std;

#endif // COMMON_HPP