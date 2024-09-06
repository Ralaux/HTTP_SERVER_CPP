#ifndef SERVER_H
#define SERVER_H

#include <string>

// Fonction pour définir si c'est un dossier
bool is_directory(const std::string& path);

// Fonction pour démarrer le serveur
void start_server(const char* host, int port);

// Fonction pour gérer les erreurs
void handle_error(const char* msg);

// Fonction pour lire un fichier depuis le disque
std::string read_file(const std::string& path);

// Fonction pour gérer les connexions client
void handle_client(int client_socket);

#endif // SERVER_H
