#include "http_parser.h"

HttpResponse http_get(std::string file_path, HttpResponse response, HttpRequest request);
HttpResponse http_post(std::string file_path, HttpResponse response, HttpRequest request);
HttpResponse http_put(std::string file_path, HttpResponse response, HttpRequest request);
HttpResponse http_delete(std::string file_path, HttpResponse response, HttpRequest request);
// HttpResponse http_number(std::string file_path, HttpResponse response, HttpRequest request);