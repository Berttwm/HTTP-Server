#include "HTTP_Request.h"

HTTP_Request::HTTP_Request(const std::string &content)
: m_root_file_name("index.html") // standard search
{
    this->extract(content);
}

Method HTTP_Request::get_method()
{
    return m_method;
}

std::string HTTP_Request::get_uri()
{
    return m_uri;
}

std::string HTTP_Request::get_header(const std::string &key)
{
    auto res = m_misc_headers.find(key);
    if (res == m_misc_headers.end()) {
        perror("HTTP Headers has no value associated with this given key");
        exit(EXIT_FAILURE);
    }
    return res->second;
}
/* extract important information from HTTP request */
void HTTP_Request::extract(const std::string &content)
{
    size_t pos_curr = 0;
    size_t pos_prev = 0;

    // Step 1: Extract Method
    pos_curr = content.find_first_of(' ', pos_prev);
    auto res = method_map.find(content.substr(pos_prev, pos_curr - pos_prev));
    if(res == method_map.end()) 
    {
        perror("No HTTP-Method substring found");
        exit(EXIT_FAILURE);
    }
    m_method = res->second;

    // Step 2: Extract URI
    pos_curr += 1; // advance pos_curr by 1
    pos_prev = pos_curr;
    pos_curr = content.find_first_of(' ', pos_prev);
    if (pos_curr == std::string::npos) {
        perror("No URI substring found");
        exit(EXIT_FAILURE);
    }
    m_uri = content.substr(pos_prev, pos_curr - pos_prev);

    // Step 3: Extract Version
    pos_curr += 1; // advance by one character (currently sitting on whitespace)
    pos_prev = pos_curr;
    pos_curr = content.find_first_of(LINE_ENDING, pos_prev);
    if (pos_curr == std::string::npos || content.substr(pos_prev, (pos_curr - pos_prev) - 1) == HTTP_VERSION) {
        perror("No valid HTTP-Version substring found");
        exit(EXIT_FAILURE);
    }

    // Step 4: Getting all additional headers 
    pos_curr += 2; // advance to next line (currently sitting on CRLF sequence -> '\r\n')
    pos_prev = pos_curr;

    std::string key; // for m_misc_headers (key)
    std::string value; // for m_misc_headers (value)

    while(pos_curr != std::string::npos)
    {
        pos_curr = content.find_first_of(':', pos_prev);
        if (pos_curr == std::string::npos) {
            break;
        }
        key = content.substr(pos_prev, pos_curr - pos_prev);
        pos_curr += 2; // advance by two characters (currently sitting on a colon followed by whitespace)
        pos_prev = pos_curr;

        // value
        pos_curr = content.find_first_of(LINE_ENDING, pos_prev);
        if (pos_curr == std::string::npos) {
            break;
        }
        value = content.substr(pos_prev, pos_curr - pos_prev);
        pos_curr += 2; // advance to next line (currently sitting on CRLF sequence -> '\r\n')
        pos_prev = pos_curr;

        m_misc_headers.insert(std::make_pair(key, value));
        std::cout<< key << "::" << value << std::endl;
    }
    // Completed Extraction

}
