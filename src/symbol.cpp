#include "symbol.hpp"

Symbol::Symbol(Texture p_tex, glm::vec2 p_anchor, glm::vec2 p_attach, glm::vec2 p_next, int p_type)
    : texture(p_tex), anchor_point(p_anchor), attach_point(p_attach), next_anchor_point(p_next), type(p_type) {}
