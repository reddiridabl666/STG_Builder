#pragma once

#include "ObjectOptionsFactory.hpp"

struct Level {
    // background
    std::string name;
    ObjectOptionsFactory::res_type objects;

    void prepare(const GameField&);
};
