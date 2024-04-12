#ifndef FONT_HPP
#define FONT_HPP

#include <map>
#include <string>
#include <list>

#include "symbol.hpp"
#include "rules.hpp"

struct Font {
    Font();
    Font(std::map<std::string, Symbol> p_symbols, std::list<FNRrule> p_fnr_rules,
        std::list<AttachRule> p_attach_rules, float p_sw, float p_tw, float p_ls, float p_gs);

    std::map<std::string, Symbol> symbols;
    std::list<FNRrule> fnr_rules;
    std::list<AttachRule> attach_rules;
    float space_width, tab_width, line_spacing, glyph_size;

    const Symbol *get_symbol(std::string name) const;

    void refresh_font();

private:
    Symbol unknown_symbol {Texture(), glm::vec2(0), glm::vec2(0), glm::vec2(0), (int)SymbolType::UNKNOWN};

    Texture generate_missing_texture(glm::ivec2 size);
};

#endif /* end of include guard: FONT_HPP */
