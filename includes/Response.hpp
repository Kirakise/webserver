#include <fstream>
#include <string>
#include <vector>
std::string readFile(std::string& filename);

std::string getListingResponse(std::string path);

std::string getMimeType(std::string path);

class Response{
    public:
    uint8_t code;
    uint64_t Content_length;
    std::string Content_type;
    std::string Content;
    std::vector <std::string> headers;

    Response(){}
    Response(uint8_t code, uint64_t Content_length, std::string Content_type, std::string Content) : 
    code(code),
    Content_length(Content_length),
    Content_type(Content_type), 
    Content(Content) {}

    std::string getCodeText(uint8_t code) {
        switch (code) {
        case 200:
            return "Unauthorized";
            break;
        case 201:
            return "Created";
            break;
        case 202:
            return "Accepted";
            break;
        case 204:
            return "No Content";
            break;
        case 403:
            return "Forbidden";
            break;
        case 404:
            return "Not found";
            break;
        }
    }

    std::string getResponse() {
        std::string s = "HTTP/1.1 " + std::to_string(code) + " " + getCodeText(code) + "\r\n";
        for (int i = 0; i < headers.size(); i++)
            s.append(headers[i] + "\r\n");
        if (Content.size() != 0){
            s.append("Content-type: " + Content_type + "\r\n");
            s.append("Content-length: " + std::to_string(Content_length) + "\r\n\r\n");
            s.append(Content);
        }
        return s;
    }
};