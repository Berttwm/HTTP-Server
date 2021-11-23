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

    // Check if file path works
    std::ifstream ifs(file_path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    if(ifs.good())
    {
    
        /* HTTP_Response creation */
        // step 1: HTTP OK header 
        auto status_code_pair = statuscode_map.find(OK);
        response.append(HTTP_VERSION);
        response.append(" ");
        response.append(std::to_string(status_code_pair->first));
        response.append(" ");
        response.append(status_code_pair->second);
        response.append(LINE_ENDING);

        // step 2: Actual Content (from index.html for our simple server)
        response.append("Content-Length: ");
        response.append(std::to_string(content.size()));
        response.append(LINE_ENDING);    // two CRLF sequences needed to indicate start of content
        response.append(LINE_ENDING);
        response.append(content);


        // std::cout << response << std::endl;

    return response;
    } else {
        // 404 error
        response.append(HTTP_VERSION);
        response.append(" 404 Not Found");
        response.append(LINE_ENDING);
        response.append("Content-Length: ");
        response.append(std::to_string(NOT_FOUND_ERROR_MESSAGE.size()));
        response.append(LINE_ENDING);
        response.append(LINE_ENDING);
        response.append(NOT_FOUND_ERROR_MESSAGE);
    } 

}
