#include "user_config.hpp"

glm::vec4 config_parser::parse_color(std::string input) {
    if (input.length() != 9 || input.at(0) != '#') {
        printf("Incorrectly formatted hex code: '%s' ('#rrggbbaa' expected)\n", input.c_str());
        exit(1);
    }
    glm::ivec4 byte_form {0};
    try {
        byte_form.r = std::stoi(input.substr(1, 2), nullptr, 16);
        byte_form.g = std::stoi(input.substr(3, 2), nullptr, 16);
        byte_form.b = std::stoi(input.substr(5, 2), nullptr, 16);
        byte_form.a = std::stoi(input.substr(7, 2), nullptr, 16);
    } catch (...) {
        printf("Color code contains non-hexadecimal values: '%s'\n", input.c_str());
        exit(1);
    }
    return glm::saturate(glm::vec4(byte_form) / 255.f);
}
