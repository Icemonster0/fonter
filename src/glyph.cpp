#include "glyph.hpp"

GlyphNode::GlyphNode(std::string p_symbol) : symbol(p_symbol), attached(nullptr) {}

GlyphNode::GlyphNode(std::string p_symbol, GlyphNode *p_attached) : symbol(p_symbol), attached(p_attached) {}

GlyphNode::~GlyphNode() {delete attached;}

Glyph::Glyph(std::string p_symbol) : base(new GlyphNode(p_symbol)) {}

Glyph::Glyph(std::string p_symbol, GlyphNode *p_attached) : base(new GlyphNode(p_symbol, p_attached)) {}

Glyph::~Glyph() {delete base;}
