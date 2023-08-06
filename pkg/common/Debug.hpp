#pragma once

#define LOG(str) std::clog << __FILE__ << ":" << __LINE__ << ":" << str << '\n'

#define FAIL(str) \
    LOG(str);     \
    exit(-1);
