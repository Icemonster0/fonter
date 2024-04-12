#include "font.hpp"

Font::Font() {}

Font::Font(std::map<std::string, Symbol> p_symbols, std::list<FNRrule> p_fnr_rules,
    std::list<AttachRule> p_attach_rules, float p_sw, float p_tw, float p_ls, float p_gs)
    : symbols(p_symbols), fnr_rules(p_fnr_rules), attach_rules(p_attach_rules),
      space_width(p_sw), tab_width(p_tw), line_spacing(p_ls), glyph_size(p_gs) {

    refresh_font();
}

const Symbol *Font::get_symbol(std::string name) const {
    std::map<std::string, Symbol>::const_iterator pos = symbols.find(name);
    if (pos == symbols.end()) {
        return &unknown_symbol;
    } else {
        return &pos->second;
    }
}

void Font::refresh_font() {
    symbols.emplace(" ", Symbol {Texture(), glm::vec2(0), glm::vec2(0), glm::vec2(0), (int)SymbolType::SPACE});
    symbols.emplace("\\n", Symbol {Texture(), glm::vec2(0), glm::vec2(0), glm::vec2(0), (int)SymbolType::NEWLINE});
    symbols.emplace("\\t", Symbol {Texture(), glm::vec2(0), glm::vec2(0), glm::vec2(0), (int)SymbolType::TAB});
    unknown_symbol = Symbol {generate_missing_texture(glm::vec2(glyph_size)),
        glm::vec2(0, 0), glm::vec2(0, glyph_size), glm::vec2(glyph_size, 0), (int)SymbolType::UNKNOWN};
}

Texture Font::generate_missing_texture(glm::ivec2 size) {
    // generate transparent texture with a red frame to represent a missing texture
    Texture tex(glm::vec4(0), size);
    for (int x = 0; x < tex.get_size().x; ++x) {
        for (int y = 0; y < tex.get_size().y; ++y) {
            if (x == 0 || x == tex.get_size().x-1 || y == 0 || y == tex.get_size().y-1)
                tex.set_pixel({x, y}, {1, 0, 0, 1});
        }
    }
    return tex;
}
