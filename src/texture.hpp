#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>

#include "lib/glm.hpp"

class Texture {
public:
    Texture();
    Texture(std::string path);
    Texture(glm::vec4 value, glm::ivec2 size);

    int load(std::string path);
    int save(std::string path) const;

    glm::ivec2 get_size() const;
    void resize(glm::ivec2 size);
    void crop(glm::ivec2 box_min, glm::ivec2 box_max);

    void clear(glm::vec4 value);
    void clear(glm::vec4 value, glm::ivec2 size);

    glm::vec4 get_pixel(glm::ivec2 pixel) const;
    void set_pixel(glm::ivec2 pixel, glm::vec4 value);

    glm::vec4 sample(glm::vec2 coords) const;
    glm::vec4 sample(glm::vec2 box_min, glm::vec2 box_max) const;
    glm::vec4 sample(glm::vec2 coords, glm::vec2 scale, glm::vec2 translation) const;

private:
    std::vector<std::vector<glm::vec4>> tex_data;
    glm::ivec2 tex_size;

    void set_size(glm::ivec2 size);
};

#endif /* end of include guard: TEXTURE_HPP */
