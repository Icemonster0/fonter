#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "lib/glm.hpp"

#include "texture.hpp"

enum class SymbolType {SPACE = 0, NEWLINE = -1, TAB = -2, UNKNOWN = -3};

struct Symbol {
    Symbol(Texture p_tex, glm::vec2 p_anchor, glm::vec2 p_attach, glm::vec2 p_next, int p_type);

    Texture texture;
    glm::vec2 anchor_point;
    glm::vec2 attach_point; // point to attach a modifier symbol to
    glm::vec2 next_anchor_point; // point to attach the next symbol to
    int type; // O = space, -1 = line break, -2 = tab, -3 = unknown
};

#endif /* end of include guard: SYMBOL_HPP */
