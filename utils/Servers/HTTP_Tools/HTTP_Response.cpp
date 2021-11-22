#include "HTTP_Response.h"

HTTP_Response::HTTP_Response(const std::string &dir_path)
: m_dir_path(dir_path)
{

}

void HTTP_Response::add_header(const std::string &key, const std::string &value) 
{
    m_misc_headers.insert(std::make_pair(key, value));
}

std::string HTTP_Response::write_responses(HTTP_Request &req)
{
    std::string response;
    std::string file_path = std::string(this->m_dir_path + "/" + req.get_uri());

    std::cout << file_path << std::endl;
}
