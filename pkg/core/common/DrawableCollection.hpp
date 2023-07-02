#pragma once

#include <memory>
#include <vector>

#include "Drawable.hpp"

using DrawableCollection = std::vector<std::unique_ptr<Drawable>>;
