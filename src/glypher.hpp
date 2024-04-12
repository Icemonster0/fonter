#ifndef GLYPHER_HPP
#define GLYPHER_HPP

#include <string>
#include <optional>
#include <list>

#include "glyph.hpp"
#include "font.hpp"

class Glypher {
public:
    std::list<Glyph> glyphify(const std::string &input_text, const Font &font);

private:
    std::list<Glyph> glyph_list;

    void slice_input_text(const std::string &input_text);

    void apply_fnr_rules(const Font &font);
    void apply_attach_rules(const Font &font);

    std::_List_const_iterator<Glyph> find_str_list_in_glyph_list(
        const std::list<std::string> &str_list, const std::list<Glyph> &glyph_list,
        std::vector<std::string> &vars, const Font &font);
    bool parse_find_var(const std::string &word, int &type);
    bool parse_replace_var(const std::string &word, std::string &value,
        const std::vector<std::string> &vars);
};

#endif /* end of include guard: GLYPHER_HPP */
