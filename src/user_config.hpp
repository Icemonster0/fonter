#ifndef USER_CONFIG_HPP
#define USER_CONFIG_HPP

#include "lib/clom.hpp"
#include "lib/glm.hpp"

namespace config_parser {
    glm::vec4 parse_color(std::string input);
} /* config_parser */

struct UserConfig {
    CLOM2_GENERAL_SETTING(background_color, --background-color, -bg, glm::vec4, #ffffffff, Hex code of the page color (in the format #rrggbbaa), config_parser::parse_color);
    CLOM2_SETTING_STRING(font_path, --font, -f, , Path to the font file to be used (mandatory));
    CLOM2_SETTING_INT(margin, --margin, -m, 50, Page margin width in pixels);
    CLOM2_SETTING_STRING(output_file, --output, -o, fonter_output.png, Path where the image will be saved);
    CLOM2_SETTING_FLOAT(scale, --scale, -s, 1.0, Scale factor for the page contents (does not apply to margin and page width));
    CLOM2_SETTING_STRING(text_input, --text, -t, , Raw text to be rendered (mandatory));
    CLOM2_SETTING_INT(page_width, --width, -w, 1000, Page width in pixels (including margins));
};

#endif /* end of include guard: USER_CONFIG_HPP */
