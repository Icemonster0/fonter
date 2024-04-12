#ifndef FONTER_HPP
#define FONTER_HPP

#include "user_config.hpp"
#include "loader.hpp"
#include "glypher.hpp"
#include "compositor.hpp"

class Fonter {
public:
    Fonter(UserConfig p_cfg);
    void run();

private:
    UserConfig cfg;

    Loader loader;
    Glypher glypher;
    Compositor compositor;
};

#endif /* end of include guard: FONTER_HPP */
