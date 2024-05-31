#ifndef GLYPH_HPP
#define GLYPH_HPP

#include <string>

struct Glyph;

struct GlyphNode {
    GlyphNode(std::string symbol);
    GlyphNode(std::string symbol, GlyphNode *attached);
    ~GlyphNode();
    GlyphNode(const GlyphNode &other);
    GlyphNode &operator=(const GlyphNode &other);

    std::string symbol;
    GlyphNode *attached;
};

struct Glyph {
    Glyph(std::string symbol);
    Glyph(std::string symbol, GlyphNode *attached);
    ~Glyph();
    Glyph(const Glyph &other);
    Glyph &operator=(const Glyph &other);

    GlyphNode *base;
};

#endif /* end of include guard: GLYPH_HPP */
