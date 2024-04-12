#include "../src/loader.hpp"
#include "../src/error_log.hpp"

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        printf("Please specify a font path\n");
        return 0;
    }

    Loader loader;
    Font font = loader.load(argv[1]);

    if (error_log.error) {
        printf("Error:\n%s", error_log.log.c_str());
        return error_log.error;
    }

    printf("Symbols (name : type)\n");
    for (const auto &[name, symbol] : font.symbols) {
        printf("%s : %d\n", name.c_str(), symbol.type);
    }
    printf("\n");

    printf("FNR Rules (find : replace)\n");
    for (const auto &fr : font.fnr_rules) {
        for (const auto &find : fr.find)
            printf("%s ", find.c_str());
        printf(": ");
        for (const auto &replace : fr.replace)
            printf("%s ", replace.c_str());
        printf("\n");
    }
    printf("\n");

    printf("Attach Rules (base : attach)\n");
    for (const auto &ar : font.attach_rules) {
        printf("%s : %s\n", ar.base.c_str(), ar.attach.c_str());
    }
    printf("\n");

    return 0;
}
