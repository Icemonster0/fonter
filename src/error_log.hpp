#ifndef ERROR_LOG
#define ERROR_LOG

#include <string>

struct ErrorLog {
    int error = 0;
    std::string log = "";
};

extern ErrorLog error_log;

#endif /* end of include guard: ERROR_LOG */
