#ifndef HTTPTools_Response_H
#define HTTPTools_Response_H

#include <string>
#include <map>

#include "HTTP_Info.h"
#include "HTTP_Request.h"

class HTTP_Response
{
private:
    std::string m_dir_path;
    std::unordered_map<std::string, std::string> m_misc_headers;
public:
    HTTP_Response(const std::string &dir_path);
    void add_header(const std::string &key, const std::string &value);
    std::string write_responses(HTTP_Request &req);
};

#endif // HTTPTools_Response_H