#include <fstream>
#include <vector>
#include "http_parser.h"
#include <response.h>
#include <cstring>
#include <iostream>
#include "server.h"

// #include <experimental/filesystem>

// namespace fs = std::experimental::filesystem;

std::string get_content_type(std::string file_path) {
    std::string content_type = "application/octet-stream";
    if (file_path.size() >= 5 && file_path.compare(file_path.size() - 5, 5, ".html") == 0) content_type = "text/html";
    else if (file_path.size() >= 4 && file_path.compare(file_path.size() - 4, 4, ".txt") == 0) content_type = "text/plain";
    else if (file_path.size() >= 5 && file_path.compare(file_path.size() - 5, 5, ".json") == 0) content_type = "application/json";
    else if (file_path.size() >= 4 && file_path.compare(file_path.size() - 4, 4, ".jpg") == 0) content_type = "image/jpeg";
    else if (file_path.size() >= 4 && file_path.compare(file_path.size() - 4, 4, ".png") == 0) content_type = "image/png";

    return content_type;
}


HttpResponse http_get(std::string file_path, HttpResponse response, HttpRequest request) {
    if (request.url == "/" || is_directory(file_path) ) {
        const char* error_message = "401 Unauthorized";
        create_http_response(&response, 401, "text/plain", error_message, std::strlen(error_message));
    }

    std::ifstream file(file_path.c_str(), std::ios::binary | std::ios::ate);
    if (file) {
        std::ifstream::pos_type file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<char> file_content(file_size);
        file.read(file_content.data(), file_size);
        std::string content_type = get_content_type(file_path);

        create_http_response(&response, 200, content_type.c_str(), file_content.data(), file_size);
        
    } else {
        const char* error_message = "404 Not Found";
        create_http_response(&response, 404, "text/plain", error_message, std::strlen(error_message));
    }
    return response;
}

HttpResponse http_post(std::string file_path, HttpResponse response, HttpRequest request) {
    // Vérification si l'URL est la racine ou un répertoire (non autorisé)
    if (request.url == "/" || is_directory(file_path)) {
        const char* error_message = "401 Unauthorized";
        create_http_response(&response, 401, "text/plain", error_message, std::strlen(error_message));
        return response;
    }

    std::ifstream check_file(file_path.c_str()); // Le file donné a un chemin qui existe ? (define le true en ouvrant le file)
    if (check_file) {
        check_file.close();
        const char* error_message = "400 Bad Request - File already exists";
        create_http_response(&response, 400, "text/plain", error_message, std::strlen(error_message));
    }

    if (request.body.empty()) {
        const char* error_message = "400 Bad Request - No data received";
        create_http_response(&response, 400, "text/plain", error_message, std::strlen(error_message));
        return response;
    }

    // Ouverture du fichier pour écriture en mode binaire
    std::ofstream file(file_path.c_str(), std::ios::binary);
    if (file) {
        file.write(request.body.c_str(), request.body.size());
        file.close();

        const char* success_message = "201 Created - File Saved";
        create_http_response(&response, 201, "text/plain", success_message, std::strlen(success_message));
    } else {
        const char* error_message = "500 Internal Server Error - Failed to save file";
        create_http_response(&response, 500, "text/plain", error_message, std::strlen(error_message));
    }

    return response;
}

HttpResponse http_put(std::string file_path, HttpResponse response, HttpRequest request) {
    std::ifstream file(file_path.c_str());

    if (file) {
        file.close(); // Fermer le fichier avant de le modifier
        std::ofstream out_file(file_path.c_str(), std::ios::binary | std::ios::trunc);
        if (out_file) {
            out_file.write(request.body.data(), request.body.size()); // Write the content to the file
            out_file.close();

            const char* success_message = "200 OK - File Modified";
            create_http_response(&response, 200, "text/plain", success_message, std::strlen(success_message));
        } else {
            const char* error_message = "500 Internal Server Error - File Modification Failed";
            create_http_response(&response, 500, "text/plain", error_message, std::strlen(error_message));
        }
    } else {
        return http_post(file_path, response, request);
    }
    return response;
}


HttpResponse http_delete(std::string file_path, HttpResponse response, HttpRequest request) {
    if (request.url == "/" || is_directory(file_path) ) {
        const char* error_message = "401 Unauthorized";
        create_http_response(&response, 401, "text/plain", error_message, std::strlen(error_message));
        return response;
    }

    std::ifstream file(file_path.c_str()); // Le file donné a un chemin qui existe ? (define le true en ouvrant le file)
    if (file) {
        file.close();
        if (std::remove(file_path.c_str()) == 0) {
            const char* success_message = "200 OK - File Deleted";
            create_http_response(&response, 200, "text/plain", success_message, std::strlen(success_message));
        } else {
            const char* error_message = "500 Internal Server Error - File Deletion Failed";
            create_http_response(&response, 500, "text/plain", error_message, std::strlen(error_message));
        }
    } else {
        const char* error_message = "404 Not Found";
        create_http_response(&response, 404, "text/plain", error_message, std::strlen(error_message));
    }

    return response;
}

// HttpResponse http_number(std::string file_path, HttpResponse response, HttpRequest request) {
//     // Ensure the path is a directory
//     if (fs::is_directory(file_path)) {
//         // Count the number of entities in the directory
//         size_t count = 0;
//         for (const auto& entry : fs::directory_iterator(file_path)) {
//             ++count;
//         }

//         // Create response with the count
//         std::string count_str = std::to_string(count);
//         create_http_response(&response, 200, "text/plain", count_str.c_str(), count_str.size());
//     } else {
//         const char* error_message = "404 Not Found";
//         create_http_response(&response, 404, "text/plain", error_message, std::strlen(error_message));
//     }
//     return response;
// }