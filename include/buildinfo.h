#pragma once

#include <string>

const struct build_info {
    std::string app_name = "Ridiculously Over-Engineered Chess Engine";
    int ver_major = 0;
    int ver_minor = 0;
    int ver_build = 7;
    friend std::ostream& operator<< (std::ostream& os, const build_info& bi);
} bi;
