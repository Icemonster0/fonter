#include "../src/texture.hpp"
#include "../src/error_log.hpp"

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        printf("Please specify an image path\n");
        return 0;
    }

    Texture tex {glm::vec4(0.8f, 0.2f, 0.9f, 1.f), glm::ivec2(10, 8)};
    tex.set_pixel(glm::ivec2(1, 1), glm::vec4(1.f, 0.5f, 0.f, 1.f));
    tex.save("image1.png");

    tex.clear({0, 0, 1, 0.2}, {400, 100});
    tex.save("image2.png");

    tex.load(std::string(argv[1]));
    if (error_log.error) {
        printf("Error log:\n%s\n", error_log.log.c_str());
        return error_log.error;
    }
    tex.save("image3.png");

    Texture tex_copy1 = tex;
    Texture tex_copy2 = tex;
    Texture tex_copy3 = tex;

    tex.resize({1200, 1200});
    tex.save("image4.png");

    tex_copy1.resize({30, 30});
    tex_copy1.save("image5.png");

    tex_copy2.resize({900, 150});
    tex_copy2.save("image6.png");

    tex_copy3.crop({150, 0}, {10, 400});
    tex_copy3.save("image7.png");
    tex_copy3.resize({300, 300});
    tex_copy3.save("image8.png");

    return 0;
}
