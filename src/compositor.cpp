#include "compositor.hpp"

#include "error_log.hpp"

Texture Compositor::render_image(const std::list<Glyph> &glyphs, const Font &font, UserConfig cfg) {
    printf("Calculating layout ...\n");

    float text_block_height;
    std::multimap<std::string, glm::vec2> layout = calculate_layout(glyphs, text_block_height, font, cfg);
    if (error_log.error) return Texture();

    printf("Rendering image ...\n");

    Texture image(
        cfg.background_color,
        glm::ivec2(cfg.page_width, int(text_block_height * cfg.scale + 2.f * cfg.margin))
    );

    // for (const auto &val : layout) {
    //     printf("%s %f %f\n", val.first.c_str(), val.second.x, val.second.y);
    // }

    const glm::ivec2 img_size = image.get_size();
    for (int x = 0; x < img_size.x; ++x) {
        for (int y = 0; y < img_size.y; ++y) {
            glm::vec4 color = image.get_pixel({x, y});

            for (const auto &[name, pos] : layout) {
                const Symbol *symbol = font.get_symbol(name);
                // coordinates are in a space where y < 0, we transform that to y < img_size.y - margin
                glm::vec2 pos_transformed {pos.x * cfg.scale + cfg.margin, pos.y * cfg.scale + img_size.y - cfg.margin};
                glm::vec4 sample = symbol->texture.sample(glm::vec2(x, y), glm::vec2(cfg.scale), pos_transformed);
                color = glm::vec4 {glm::mix(color.rgb(), sample.rgb(), sample.a), glm::clamp(color.a + sample.a, 0.f, 1.f)};
            }

            image.set_pixel({x, y}, color);
        }
    }

    return std::move(image);
}

std::multimap<std::string, glm::vec2> Compositor::calculate_layout(const std::list<Glyph> &glyphs_source, float &text_block_height, const Font &font, UserConfig cfg) {
    // coordinate space: 0 < x < text_size.x ; -text_size.y < y < 0

    std::list<Glyph> glyphs = glyphs_source;
    std::multimap<std::string, glm::vec2> layout;

    insert_newlines(glyphs, font, cfg);
    if (error_log.error) return layout;

    glm::vec2 insert_pos {0, -font.glyph_size};

    for (const Glyph &glyph : glyphs) {
        const Symbol *symbol = font.get_symbol(glyph.base->symbol);

        if (symbol->type == (int)SymbolType::SPACE) {
            insert_pos.x += font.space_width;
            // printf("space: %s %f %f\n", glyph.symbol.c_str(), insert_pos.x, insert_pos.y);
            continue;
        } else if (symbol->type == (int)SymbolType::TAB) {
            insert_pos.x += font.tab_width;
            // printf("tab: %s %f %f\n", glyph.symbol.c_str(), insert_pos.x, insert_pos.y);
            continue;
        } else if (symbol->type == (int)SymbolType::NEWLINE) {
            insert_pos.x = 0;
            insert_pos.y -= font.line_spacing;
            // printf("newline: %s %f %f\n", glyph.symbol.c_str(), insert_pos.x, insert_pos.y);
            continue;
        }

        layout.emplace(glyph.base->symbol, insert_pos - symbol->anchor_point);
        // printf("base: %s %f %f\n", glyph.symbol.c_str(), insert_pos.x, insert_pos.y);

        insert_attachments(*glyph.base, insert_pos + symbol->attach_point - symbol->anchor_point, layout, font);

        insert_pos = insert_pos + symbol->next_anchor_point - symbol->anchor_point;
    }

    text_block_height = std::fabs(insert_pos.y);

    return layout;
}

void Compositor::insert_newlines(std::list<Glyph> &glyphs, const Font &font, UserConfig cfg) {
    const float text_block_width = (cfg.page_width - 2.f * cfg.margin) / cfg.scale;

    float cur_pos = 0;
    bool first_word = true;

    for (auto it = glyphs.begin(); it != glyphs.end(); ++it) {

        if (it->base->symbol == "\\n") {
            cur_pos = 0;
            first_word = true;
            continue;
        } else if (it->base->symbol == "\\t") {
            cur_pos += font.tab_width;
            first_word = false;
            continue;
        } else if (it->base->symbol == " ") {
            cur_pos += font.space_width;
            first_word = false;
            continue;
        }

        const Symbol *sym = font.get_symbol(it->base->symbol);
        cur_pos += sym->next_anchor_point.x - sym->anchor_point.x;

        // printf("%s  %f  %f\n", it->symbol.c_str(), cur_pos, text_block_width);

        if (cur_pos > text_block_width && it->base->symbol != "\\n") {
            if (first_word) {
                it = glyphs.emplace(it, "\\n");
                ++it;
                cur_pos = sym->next_anchor_point.x - sym->anchor_point.x;
                first_word = true;
            } else {
                for (--it; it->base->symbol != " " && it->base->symbol != "\\t"; --it) {}
                it->base->symbol = "\\n";
                cur_pos = 0;
                first_word = true;
            }
        }
    }
}


void Compositor::insert_attachments(const GlyphNode &glyph_node, glm::vec2 insert_pos, std::multimap<std::string, glm::vec2> &layout, const Font &font) {
    if (glyph_node.attached) {
        const Symbol *symbol = font.get_symbol(glyph_node.attached->symbol);
        layout.emplace(glyph_node.attached->symbol, insert_pos - symbol->anchor_point);
        // printf("attd: %s %f %f\n", glyph.attached->symbol.c_str(), insert_pos.x, insert_pos.y);

        insert_attachments(*glyph_node.attached, insert_pos + symbol->attach_point - symbol->anchor_point, layout, font);
    }
}
