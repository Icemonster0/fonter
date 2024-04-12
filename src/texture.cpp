#include "texture.hpp"

#include <cstdint>
#include <algorithm>
#include <memory>

#include "lib/stb.hpp"
#include "error_log.hpp"
#include "math.hpp"

// public

Texture::Texture() : tex_size(glm::ivec2(0)) {}

Texture::Texture(std::string path) {
    load(path);
}

Texture::Texture(glm::vec4 value, glm::ivec2 size) {
    clear(value, size);
}

int Texture::load(std::string path) {
    glm::ivec2 size;
    int channels;
    const int force_channels = 4;
    uint8_t *data = stbi_load(path.c_str(), &size.x, &size.y, &channels, force_channels);

    if (data == nullptr) {
        error_log.error = 1;
        error_log.log.append("Failed to open file ").append(path).append("\n");
        return 1;
    }

    set_size(size);
    for (int x = 0; x < tex_size.x; ++x) {
        for (int y = 0; y < tex_size.y; ++y) {
            int offset = (tex_size.y-y-1) * tex_size.x + x;

            // transform to range [0; 1]
            float r = float(*(data + offset * 4 + 0)) / 255.f;
            float g = float(*(data + offset * 4 + 1)) / 255.f;
            float b = float(*(data + offset * 4 + 2)) / 255.f;
            float a = float(*(data + offset * 4 + 3)) / 255.f;

            set_pixel(glm::ivec2(x, y), glm::saturate(glm::vec4 {r, g, b, a}));
        }
    }

    free(data);
    return 0;
}

int Texture::save(std::string path) const {
    uint8_t *data = (uint8_t*)malloc(tex_size.x * tex_size.y * 4);
    for (int x = 0; x < tex_size.x; ++x) {
        for (int y = 0; y < tex_size.y; ++y) {
            int offset = (y * tex_size.x + x) * 4;
            glm::vec4 color = get_pixel(glm::ivec2(x, y));
            data[offset + 0] = (uint8_t)std::clamp((int)(color.r * 255), 0, 255);
            data[offset + 1] = (uint8_t)std::clamp((int)(color.g * 255), 0, 255);
            data[offset + 2] = (uint8_t)std::clamp((int)(color.b * 255), 0, 255);
            data[offset + 3] = (uint8_t)std::clamp((int)(color.a * 255), 0, 255);
        }
    }

    stbi_flip_vertically_on_write(1);
    int result = stbi_write_png(path.c_str(), tex_size.x, tex_size.y, 4, data, tex_size.x * 4);
    free(data);

    if (result == 0) {
        error_log.error = 1;
        error_log.log.append("PNG write failure: ").append(path).append("\n");
    }

    return result;
}

glm::ivec2 Texture::get_size() const {
    return tex_size;
}

void Texture::resize(glm::ivec2 new_size) {
    Texture new_tex {glm::vec4(0), new_size};
    glm::vec2 scale = glm::vec2(new_size) / glm::vec2(tex_size);

    for (int x = 0; x < new_size.x; ++x) {
        for (int y = 0; y < new_size.y; ++y) {
            new_tex.set_pixel(glm::ivec2(x, y), this->sample(glm::vec2(x, y), scale, glm::vec2(0)));
        }
    }

    *this = new_tex;
}

void Texture::crop(glm::ivec2 box_min, glm::ivec2 box_max) {
    glm::ivec2 box_min_tmp = glm::clamp(glm::min(box_min, box_max), glm::ivec2(0), tex_size);
    box_max = glm::clamp(glm::max(box_max, box_min), glm::ivec2(0), tex_size);
    box_min = box_min_tmp;

    Texture new_tex {glm::vec4(0), box_max - box_min};

    int old_x = box_min.x, new_x = 0;
    while (old_x < box_max.x) {
        int old_y = box_min.y, new_y = 0;
        while (old_y < box_max.y) {
            new_tex.set_pixel({new_x, new_y}, this->get_pixel({old_x, old_y}));
            ++old_y;
            ++new_y;
        }
        ++old_x;
        ++new_x;
    }

    *this = new_tex;
}

void Texture::clear(glm::vec4 value) {
    tex_data = std::vector<std::vector<glm::vec4>> {
        static_cast<long unsigned int>(tex_size.x),
        std::vector<glm::vec4> {
            static_cast<long unsigned int>(tex_size.y),
            value
        }
    };
}

void Texture::clear(glm::vec4 value, glm::ivec2 size) {
    tex_data = std::vector<std::vector<glm::vec4>> {
        static_cast<long unsigned int>(size.x),
        std::vector<glm::vec4> {
            static_cast<long unsigned int>(size.y),
            value
        }
    };
    tex_size = size;
}

glm::vec4 Texture::get_pixel(glm::ivec2 pixel) const {
    return tex_data[pixel.x][pixel.y];
}

void Texture::set_pixel(glm::ivec2 pixel, glm::vec4 value) {
    tex_data[pixel.x][pixel.y] = value;
}

glm::vec4 Texture::sample(glm::vec2 coords) const {
    // interpolate value at coords

    glm::ivec2 a_pos = glm::clamp(glm::ivec2 {glm::floor(coords.x), glm::floor(coords.y)}, glm::ivec2 {0}, get_size()-1);
    glm::ivec2 b_pos = glm::clamp(glm::ivec2 {glm::ceil(coords.x), glm::floor(coords.y)}, glm::ivec2 {0}, get_size()-1);
    glm::ivec2 c_pos = glm::clamp(glm::ivec2 {glm::floor(coords.x), glm::ceil(coords.y)}, glm::ivec2 {0}, get_size()-1);
    glm::ivec2 d_pos = glm::clamp(glm::ivec2 {glm::ceil(coords.x), glm::ceil(coords.y)}, glm::ivec2 {0}, get_size()-1);

    glm::vec2 pixel_fract_coord = glm::fract(coords);
    glm::vec2 interp {math::quad_interp(pixel_fract_coord.x), math::quad_interp(pixel_fract_coord.y)};
    glm::vec4 ab = glm::mix(get_pixel(a_pos), get_pixel(b_pos), interp.x);
    glm::vec4 cd = glm::mix(get_pixel(c_pos), get_pixel(d_pos), interp.x);
    return glm::mix(ab, cd, interp.y);
}

glm::vec4 Texture::sample(glm::vec2 box_min, glm::vec2 box_max) const {
    // calculate average value in box

    // check if the box overlaps the texture
    bool is_overlapping = (box_min.x <= get_size().x && 0 <= box_max.x && box_min.y <= get_size().y && 0 <= box_max.y);
    if (!is_overlapping) {
        return glm::vec4 {0};
    }

    glm::vec4 values_sum {0};
    float weights_sum = 0;

    glm::ivec2 min_pixel = glm::clamp(glm::ivec2(glm::floor(box_min)), glm::ivec2(0), tex_size-1);
    glm::ivec2 max_pixel = glm::clamp(glm::ivec2(glm::ceil(box_max)), glm::ivec2(0), tex_size-1);

    for (int x = min_pixel.x; x <= max_pixel.x; ++x) {
        for (int y = min_pixel.y; y <= max_pixel.y; ++y) {
            // calculate overlapping area
            glm::vec2 pix_min {x, y};
            glm::vec2 pix_max {x+1, y+1};
            glm::vec2 diff_min = glm::saturate(box_min - pix_min);
            glm::vec2 diff_max = glm::saturate(pix_max - box_max);
            // using abs in case min is greater than max
            glm::vec2 overlap = glm::abs(glm::vec2(1) - diff_min - diff_max);
            float area = overlap.x * overlap.y; // = weight

            values_sum += area * get_pixel(glm::ivec2(x, y));
            weights_sum += area;
        }
    }

    if (weights_sum == 0) return glm::vec4(0);

    return values_sum / weights_sum;
}

glm::vec4 Texture::sample(glm::vec2 coords, glm::vec2 scale, glm::vec2 translation) const {
    // use the box sampling method (sample(vec2 bmin, vec2 bmax)) to stretch or squash the texture

    glm::bvec2 stretch {scale.x > 1.f, scale.y > 1.f};

    glm::vec2 inv_scale = 1.f / scale;

    // // test begin
    // coords = coords * inv_scale - translation;
    // // printf("%f %f\n", coords.x, coords.y);
    // if (glm::all(glm::greaterThanEqual(coords, glm::vec2(0)) && glm::lessThanEqual(coords, glm::vec2(get_size())))) {
    //     return glm::vec4 {0, 1, 0, 1};
    // } else return glm::vec4 {0};
    // // test end

    glm::vec2 box_min = inv_scale * (coords - translation);
    glm::vec2 box_max {
        inv_scale.x * (coords.x + (stretch.x ? scale.x : 1.f) - translation.x),
        inv_scale.y * (coords.y + (stretch.y ? scale.y : 1.f) - translation.y)
    };

    return sample(box_min, box_max);
}

// private

void Texture::set_size(glm::ivec2 size) {
    clear(glm::vec4 {}, size);
}
