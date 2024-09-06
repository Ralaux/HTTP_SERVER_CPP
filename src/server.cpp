#include "response.h"
#include "http_methods.h"
#include "http_parser.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <limits>
#include <sys/stat.h>
#include <vector>
#include <thread>
#include <unistd.h>  // Ajouté pour les fonctions read, write, close

#define ROOT_DIR "www"

bool is_directory(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false;  // Le chemin n'existe pas
    }
    return (info.st_mode & S_IFDIR) != 0;  // Vérifie si c'est un répertoire
}

void handle_client(int client_socket) {
    char buffer[4096];
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        std::cerr << "Failed to read from client socket." << std::endl;
        close(client_socket);
        return;
    }
    
    buffer[bytes_read] = '\0';  // Terminer la chaîne de caractères

    HttpRequest request;
    parse_http_request(buffer, request);

    // Déterminer le chemin du fichier
    std::string file_path = ROOT_DIR + request.url;

    // if (request.url == "/" && request.method == "GET") {
    //     file_path = ROOT_DIR "/index.html";  // Fichier par défaut
    // }
    
    HttpResponse response;
    std::cout << file_path << std::endl;

    if (request.method == "GET") {
        response = http_get(file_path, response, request);
    } 
    else if (request.method == "DELETE") {
        response = http_delete(file_path, response, request);
    }
    else if (request.method == "POST") {
        response = http_post(file_path, response, request);
    }
    else if (request.method == "PUT") {
        response = http_post(file_path, response, request);
    }
    // else if (request.method == "NUMBER") {
    //     response = http_number(file_path, response, request);
    // }
    else {
        const char* error_message = "405 Method Not Allowed";
        create_http_response(&response, 405, "text/plain", error_message, std::strlen(error_message));
    }

        
    
    std::string response_str = format_http_response(response);
    write(client_socket, response_str.c_str(), response_str.size());
    close(client_socket);
}

void start_server(const char* host, int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind to address." << std::endl;
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) == -1) {
        std::cerr << "Failed to listen on socket." << std::endl;
        close(server_fd);
        return;
    }

    std::cout << "Server is listening on " << host << ":" << port << std::endl;

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_socket == -1) {
            std::cerr << "Failed to accept connection." << std::endl;
            continue;
        }

        // Créer un thread pour gérer le client
        std::thread(handle_client, client_socket).detach();
    }

    close(server_fd);
}