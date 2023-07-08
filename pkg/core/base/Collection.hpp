#pragma once

#include <memory>
#include <vector>

template <typename T>
using Collection = std::vector<std::shared_ptr<T>>;
