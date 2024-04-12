#ifndef GLYPH_HPP
#define GLYPH_HPP

#include <string>

struct Glyph;

struct GlyphNode {
    GlyphNode(std::string symbol);
    GlyphNode(std::string symbol, GlyphNode *attached);
    ~GlyphNode();

    std::string symbol;
    GlyphNode *attached;
};

struct Glyph {
    Glyph(std::string symbol);
    Glyph(std::string symbol, GlyphNode *attached);
    ~Glyph();
    GlyphNode *base;
};

#endif /* end of include guard: GLYPH_HPP */
