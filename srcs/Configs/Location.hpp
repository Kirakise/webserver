#ifndef LOCATION_HPP
# define LOCATION_HPP
#include <vector>
#include <string>
#include <vector>

class Location{

    public:

    Location();
    ~Location();

    std::vector < std::string> locations;
    std::string _root;
    std::vector < std::string > allowedMethods;
    std::vector <std::string> indexes;
    std::string _cgi_pass;
    size_t clientBodyBufferSize;
    std::vector < Location > locs;

    std::string filename;

    bool open_scope;
    bool next_loc;
    
    size_t ParseLocation(std::string file);
    static Location* newLocation() {Location *newloc = new Location; return newloc;};
    void ParseDirects(std::string buf);
    void ParseTokens(std::string str);

    void allow_methods(std::string str);
    void index(std::string str);
    void root(std::string str);
    void closed_scope(std::string str);
    void next_location(std::string str);
    void cgi_pass(std::string str);
    void client_body_bufsize(std::string str);
};

#endif