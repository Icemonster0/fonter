#include "glyph.hpp"

GlyphNode::GlyphNode(std::string p_symbol) : symbol(p_symbol), attached(nullptr) {}

GlyphNode::GlyphNode(std::string p_symbol, GlyphNode *p_attached) : symbol(p_symbol), attached(p_attached) {}

GlyphNode::~GlyphNode() {delete attached;}

GlyphNode::GlyphNode(const GlyphNode &other) {
    symbol = other.symbol;
    if (other.attached == nullptr) {
        attached = nullptr;
    } else {
        attached = new GlyphNode(*(other.attached));
    }
}

GlyphNode &GlyphNode::operator=(const GlyphNode &other) {
    return *(new GlyphNode(other));
}

Glyph::Glyph(std::string p_symbol) : base(new GlyphNode(p_symbol)) {}

Glyph::Glyph(std::string p_symbol, GlyphNode *p_attached) : base(new GlyphNode(p_symbol, p_attached)) {}

Glyph::~Glyph() {delete base;}

Glyph::Glyph(const Glyph &other) {
    if (other.base == nullptr) {
        base = nullptr;
    } else {
        base = new GlyphNode(*(other.base));
    }
}

Glyph &Glyph::operator=(const Glyph &other) {
    return *(new Glyph(other));
}
