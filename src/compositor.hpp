#ifndef COMPOSITOR_HPP
#define COMPOSITOR_HPP

#include <list>
#include <map>
#include <string>

#include "texture.hpp"
#include "font.hpp"
#include "glyph.hpp"
#include "user_config.hpp"

class Compositor {
public:
    Texture render_image(const std::list<Glyph> &glyphs, const Font &font, UserConfig cfg);

private:
    std::multimap<std::string, glm::vec2> calculate_layout(const std::list<Glyph> &glyphs_source, float &text_block_height, const Font &font, UserConfig cfg);
    void insert_newlines(std::list<Glyph> &glyphs, const Font &font, UserConfig cfg);
    void insert_attachments(const GlyphNode &glyph_node, glm::vec2 insert_pos, std::multimap<std::string, glm::vec2> &layout, const Font &font);
};

#endif /* end of include guard: COMPOSITOR_HPP */
