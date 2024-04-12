#include "fonter.hpp"

#include <cstdio>

#include "error_log.hpp"

Fonter::Fonter(UserConfig p_cfg) : cfg(p_cfg) {}

void Fonter::run() {
    Font font = loader.load(cfg.font_path);
    if (error_log.error) return;

    std::list<Glyph> glyphs = glypher.glyphify(cfg.text_input, font);
    if (error_log.error) return;

    Texture image = compositor.render_image(glyphs, font, cfg);
    if (error_log.error) return;

    image.save(cfg.output_file);

    printf("Done!\n");
}
