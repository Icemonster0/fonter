#include "loader.hpp"

#include <cstdio>
#include <iostream>

#include "error_log.hpp"

Font Loader::load(std::string path) {
    std::ifstream file {path};
    if (!file.is_open()) {
        error_log.error = 1;
        error_log.log.append("Failed to open file ").append(path).append("\n");
        return std::move(font);
    }

    std::size_t last_sep = path.find_last_of("\\/");
    std::string directory_path = (last_sep == std::string::npos) ? "./" : path.substr(0, last_sep+1);

    printf("Parsing font file ...\n");
    parse(file, directory_path);

    file.close();
    return std::move(font);
}

#define READ_TO(value, info, context) check_for_comment(file); if (!(file >> value)) {syntax_error(info, context); return;}

void Loader::parse(std::ifstream &file, std::string directory_path) {
    std::string word;

    // preamble
    READ_TO(word, "SPACE_WIDTH/SW_ expected", "preamble");
    if (word != "SPACE_WIDTH" && word != "SW_") {syntax_error("SPACE_WIDTH/SW_ expected", "preamble"); return;}
    READ_TO(font.space_width, "value expected", "SPACE_WIDTH/SW_");
    READ_TO(word, "TAB_WIDTH/TW_ expected", "preamble");
    if (word != "TAB_WIDTH" && word != "TW_") {syntax_error("TAB_WIDTH/TW_ expected", "preamble"); return;}
    READ_TO(font.tab_width, "value expected", "TAB_WIDTH/TW_");
    READ_TO(word, "LINE_SPACING/LS_ expected", "preamble");
    if (word != "LINE_SPACING" && word != "LS_") {syntax_error("LINE_SPACING/LS_ expected", "preamble"); return;}
    READ_TO(font.line_spacing, "value expected", "LINE_SPACING/LS_");
    READ_TO(word, "GLYPH_SIZE/GS_ expected", "preamble");
    if (word != "GLYPH_SIZE" && word != "GS_") {syntax_error("GLYPH_SIZE/GS_ expected", "preamble"); return;}
    READ_TO(font.glyph_size, "value expected", "GLYPH_SIZE/GS_");
    font.refresh_font();

    // main body
    check_for_comment(file);
    while (file >> word) {
        if (word == "SYMBOL" || word == "S_")
            parse_symbol(file, directory_path);
        else if (word == "FNR_RULE" || word == "FR_")
            parse_fnr_rule(file);
        else if (word == "ATTACH_RULE" || word == "AR_")
            parse_attach_rule(file);
        else {
            token_error(word, "global scope");
            return;
        }
        if (error_log.error) break;
        check_for_comment(file);
    }
}

void Loader::parse_symbol(std::ifstream &file, std::string directory_path) {
    std::string name, tex_path;
    int type;
    glm::vec2 anchor, attach, next;
    int left, right, top, bottom;


    READ_TO(name, "bad name", "symbol entry");
    if (!validate_symbol_name(name, "symbol entry", true)) return;

    std::string context = std::string("definition of symbol '").append(name).append("'");

    READ_TO(type, "type should be an integer", context);
    READ_TO(tex_path, "bad texture path", context);
    READ_TO(anchor.x, "anchor.x should be a number", context);
    READ_TO(anchor.y, "anchor.y should be a number", context);
    READ_TO(attach.x, "attachment_point.x should be a number", context);
    READ_TO(attach.y, "attachment_point.y should be a number", context);
    READ_TO(next.x, "next_attach_point.x should be a number", context);
    READ_TO(next.y, "next_attach_point.y should be a number", context);

    Texture tex {directory_path + tex_path};
    if (error_log.error) return;
    left = bottom = 0;
    right = tex.get_size().x;
    top = tex.get_size().y;

    check_for_comment(file);
    std::string word;
    while (file >> word) {
        if (word == "LEFT" || word == "L_") {
            READ_TO(left, "LEFT/L_ should be followed by an integer", name);
        } else if (word == "RIGHT" || word == "R_") {
            READ_TO(right, "RIGHT/R_ should be followed by an integer", name);
        } else if (word == "TOP" || word == "T_") {
            READ_TO(top, "TOP/T_ should be followed by an integer", name);
        } else if (word == "BOTTOM" || word == "B_") {
            READ_TO(bottom, "BOTTOM/B_ should be followed by an integer", name);
        } else if (word == "END" || word == "E_") {
            break;
        } else {
            token_error(word, name);
            return;
        }
        check_for_comment(file);
    }

    tex.crop({left, bottom}, {right, top});

    font.symbols.emplace(name, Symbol {tex, anchor, attach, next, type});
}

void Loader::parse_fnr_rule(std::ifstream &file) {
    std::list<std::string> find, replace;

    std::string find_error = "FIND/F_ expected";
    std::string replace_error = "symbol or REPLACE/R_ expected";
    std::string end_error = "symbol or END/E_ expected";
    std::string context = "find-and-replace rule";

    std::string word;

    READ_TO(word, find_error, context);
    if (word != "FIND" && word != "F_") {
        syntax_error(find_error, context);
        return;
    }

    while (true) {
        READ_TO(word, replace_error, context);
        if (word == "REPLACE" || word == "R_") break;
        if (!validate_symbol_name(word, context, false)) return;
        find.emplace_back(word);
    }

    while (true) {
        READ_TO(word, end_error, context);
        if (word == "END" || word == "E_") break;
        if (!validate_symbol_name(word, context, false)) return;
        replace.push_back(word);
    }

    if (find.empty()) {
        syntax_error("no find-symbols provided", context);
        return;
    }
    // replace is allowed to be empty; used for deletion
    // if (replace.empty()) {
    //     syntax_error("no replace-symbols provided", context);
    //     return;
    // }

    font.fnr_rules.push_back(FNRrule {find, replace});
}

void Loader::parse_attach_rule(std::ifstream &file) {
    std::string base, attach;

    std::string context = "attach rule";

    READ_TO(base, "bad base symbol", context);
    if (!validate_symbol_name(base, context, false)) return;
    READ_TO(attach, "bad attach symbol", context);
    if (!validate_symbol_name(attach, context, false)) return;

    font.attach_rules.push_back(AttachRule {base, attach});
}

#undef READ_TO

void Loader::check_for_comment(std::ifstream &file) {
    char next_char;

    // skip whitespace
    next_char = file.peek();
    while (next_char == ' ' || next_char == '\n' || next_char == '\t') {
        file.get();
        next_char = file.peek();
    }
    // printf("Next char: %c\n", file.peek());

    if (next_char != '/') return;
    // printf("potential comment detected\n");

    file.get(); // read '/'
    next_char = file.get();

    switch (next_char) {
        case '*': {
            // printf("block comment detected\n");
            while (file.get(next_char)) {
                if (next_char == '*' && file.peek() == '/') {
                    file.get(next_char); // read '/'
                    check_for_comment(file);
                    return;
                }
            } return;
        }
        case '/': {
            // printf("line comment detected\n");
            std::string line;
            getline(file, line);
            check_for_comment(file);
            return;
        }
        default: {
            file.seekg(-2, file.cur); return;
        }
    }
}

bool Loader::validate_symbol_name(std::string name, std::string context, bool strict) {
    if (
        name == "SPACE_WIDTH" || name == "SW_" ||
        name == "TAB_WIDTH" || name == "TW_" ||
        name == "LINE_SPACING" || name == "LS_" ||
        name == "GLYPH_SIZE" || name == "GS_" ||
        name == "SYMBOL" || name == "S_" ||
        name == "LEFT" || name == "L_" ||
        name == "RIGHT" || name == "R_" ||
        name == "TOP" || name == "T_" ||
        name == "BOTTOM" || name == "B_" ||
        name == "FNR_RULE" || name == "FR_" ||
        name == "ATTACH_RULE" || name == "AR_" ||
        strict && (name == " " || name == "\\n" || name == "\\t")
    ) {
        syntax_error(std::string("the word '").append(name).append("' is reserved and cannot be used as a symbol name"), context);
        return false;
    }
    else return true;
}

void Loader::token_error(std::string token, std::string context) {
    error_log.error = 2;
    error_log.log.append("Unrecognized token in font file: ").append(token)
        .append(": in ").append(context).append("\n");
}

void Loader::syntax_error(std::string info, std::string context) {
    error_log.error = 2;
    error_log.log.append("Syntax error in font file: ").append(info)
        .append(": in ").append(context).append("\n");
}
