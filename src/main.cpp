#include <cstdio>
#include <filesystem>

#include "lib/clom.hpp"

#include "fonter.hpp"
#include "user_config.hpp"
#include "error_log.hpp"

int main(int argc, char const *argv[]) {

    // Process command line options
    CLOM2_SET_ARGS(argc, argv);
    CLOM2_CHECK_FOR_HELP_BEGIN(help, h);
    UserConfig cfg;
    CLOM2_CHECK_FOR_HELP_END();

    // Check if the options are valid
    if (cfg.font_path.length() == 0 || cfg.text_input.length() == 0) {
        printf("Please provide the options `--font` and `--text`\n");
        return 1;
    }

    // Process and validate output directory string
    std::size_t last_sep = cfg.output_file.find_last_of("\\/");
    std::string out_dir = (last_sep == std::string::npos) ? "." : cfg.output_file.substr(0, last_sep);
    if (!std::filesystem::is_directory(std::filesystem::path(out_dir))) {
        printf("Directory doesn't exist: %s\n", out_dir.c_str());
        return 7;
    }

    // Run the engine
    Fonter fonter = Fonter(cfg);
    fonter.run();

    // Report errors
    if (error_log.error) {
        printf("\nError:\n%s\n", error_log.log.c_str());
        return error_log.error;
    }

    return 0;
}
