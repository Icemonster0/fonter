#include "glypher.hpp"

#include "error_log.hpp"

std::list<Glyph> Glypher::glyphify(const std::string &input_text, const Font &font) {
    printf("Slicing input text ...\n");
    slice_input_text(input_text);
    if (error_log.error) return std::move(glyph_list);
    printf("Applying FNR rules ...\n");
    apply_fnr_rules(font);
    if (error_log.error) return std::move(glyph_list);
    printf("Applying attach rules ...\n");
    apply_attach_rules(font);
    if (error_log.error) return std::move(glyph_list);

    return std::move(glyph_list);
}

void Glypher::slice_input_text(const std::string &input_text) {
    for (const char &character : input_text) {
        if (character == '\n') {
            glyph_list.emplace_back(std::string {"\\n"});
        } else if (character == '\t') {
            glyph_list.emplace_back(std::string {"\\t"});
        } else {
            glyph_list.emplace_back(std::string {character});
        }
    }
}

void Glypher::apply_fnr_rules(const Font &font) {
    for (const FNRrule &rule : font.fnr_rules) {
        std::vector<std::string> vars;
        auto it = find_str_list_in_glyph_list(rule.find, glyph_list, vars, font);

        while (it != glyph_list.end()) {
            auto insert_pos = glyph_list.erase(it, std::next(it, rule.find.size()));
            for (const auto &str : rule.replace) {
                std::string var_value = "";
                if (parse_replace_var(str, var_value, vars))
                    glyph_list.emplace(insert_pos, var_value);
                else glyph_list.emplace(insert_pos, str);
                if (error_log.error) return;
            }
            it = find_str_list_in_glyph_list(rule.find, glyph_list, vars, font);
        }
    }
}

void Glypher::apply_attach_rules(const Font &font) {
    for (const AttachRule &rule : font.attach_rules) {
        for (auto it = glyph_list.rbegin(); it != glyph_list.rend(); ++it) {

            int type = 0;
            bool is_var = parse_find_var(rule.attach, type);
            const Symbol *sym = font.get_symbol(it->base->symbol);

            if (!is_var && it->base->symbol == rule.attach ||
                 is_var && sym->type == type) {

                auto next_it = std::next(it);
                is_var = parse_find_var(rule.base, type);
                sym = font.get_symbol(next_it->base->symbol);

                if (next_it != glyph_list.rend() &&
                    (!is_var && next_it->base->symbol == rule.base ||
                      is_var && sym->type == type)) {
                    next_it->base->attached = it->base;
                    it->base = nullptr;
                    it = --std::reverse_iterator(glyph_list.erase(--(it.base())));
                }
            }
        }
    }
}

std::_List_const_iterator<Glyph> Glypher::find_str_list_in_glyph_list(
    const std::list<std::string> &strs, const std::list<Glyph> &glyphs,
    std::vector<std::string> &vars, const Font &font) {

    auto str_it = strs.begin();
    auto glyph_found = glyphs.end();

    vars.clear();

    for (auto glyph_it = glyphs.begin(); glyph_it != glyphs.end(); ++glyph_it) {

        int type = 0;
        bool is_var = parse_find_var(*str_it, type);
        const Symbol *sym = font.get_symbol(glyph_it->base->symbol);

        if (!is_var && glyph_it->base->symbol == (*str_it) ||
             is_var && sym->type == type) {

            glyph_found = glyph_it;
            if (is_var) vars.emplace_back(glyph_it->base->symbol);
            ++glyph_it;
            ++str_it;
            if (str_it == strs.end()) return glyph_found;
            if (glyph_it == glyphs.end()) return glyphs.end();

            is_var = parse_find_var(*str_it, type);
            sym = font.get_symbol(glyph_it->base->symbol);

            while (!is_var && glyph_it->base->symbol == (*str_it) ||
                    is_var && sym->type == type) {
                if (is_var) vars.emplace_back(glyph_it->base->symbol);
                ++glyph_it;
                ++str_it;
                if (str_it == strs.end()) return glyph_found;
                if (glyph_it == glyphs.end()) return glyphs.end();

                is_var = parse_find_var(*str_it, type);
                sym = font.get_symbol(glyph_it->base->symbol);
            }

            glyph_it = glyph_found;
            str_it = strs.begin();
            vars.clear();
        }
    }

    return glyphs.end();
}

bool Glypher::parse_find_var(const std::string &word, int &type) {

    if (word.at(0) != '$') return false;
    try {
        type = std::stoi(word.substr(1));
    } catch (...) {
        return false;
    }
    return true;
}

bool Glypher::parse_replace_var(const std::string &word, std::string &value,
    const std::vector<std::string> &vars) {

    if (word.at(0) != '$') return false;
    int idx;
    try {
        idx = std::stoi(word.substr(1));
    } catch (...) {
        return false;
    }
    if (idx > vars.size() || idx < 1) {
        error_log.log.append("There are only ").append(std::to_string(vars.size()))
            .append(" variables, but $").append(std::to_string(idx))
            .append(" requested! : in FNR_RULE\n");
        error_log.error = 11;
        return false;
    }
    value = vars[idx-1];
    return true;
}
