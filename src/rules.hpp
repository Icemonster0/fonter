#ifndef RULES_HPP
#define RULES_HPP

#include <list>
#include <string>

struct FNRrule {
    std::list<std::string> find, replace;
};

struct AttachRule {
    std::string base, attach;
};

#endif /* end of include guard: RULES_HPP */
