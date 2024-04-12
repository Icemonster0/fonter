#ifndef LOADER_HPP
#define LOADER_HPP

#include <string>
#include <fstream>

#include "font.hpp"

class Loader {
public:
    Font load(std::string path);

private:
    Font font;

    void parse(std::ifstream &file, std::string directory_path);
    void parse_symbol(std::ifstream &file, std::string directory_path);
    void parse_fnr_rule(std::ifstream &file);
    void parse_attach_rule(std::ifstream &file);

    void check_for_comment(std::ifstream &file);
    bool validate_symbol_name(std::string name, std::string context, bool strict);

    void token_error(std::string token, std::string context);
    void syntax_error(std::string info, std::string context);
};

#endif /* end of include guard: LOADER_HPP */
