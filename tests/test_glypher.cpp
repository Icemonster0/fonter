#include "../src/loader.hpp"
#include "../src/glypher.hpp"
#include "../src/error_log.hpp"

void print_glyph(const GlyphNode &glyph) {
    printf("%s  ", glyph.symbol.c_str());
    if (glyph.attached) {
        print_glyph(*glyph.attached);
    }
}

int main(int argc, char const *argv[]) {

    if (argc != 3) {
        printf("Please specify a font path and some input text\n");
        return 0;
    }

    Loader loader;
    Font font = loader.load(argv[1]);

    if (error_log.error) {
        printf("Error:\n%s", error_log.log.c_str());
        return error_log.error;
    }

    Glypher glypher;
    std::list<Glyph> glyphs = glypher.glyphify(argv[2], font);

    printf("Here are the parsed glyphs:\n");
    for (const auto &glyph : glyphs) {
        print_glyph(*glyph.base);
        printf("\n");
    }

    return 0;
}
