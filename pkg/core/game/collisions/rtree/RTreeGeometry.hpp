#pragma once

#include <string>

#include "Framed.hpp"
#include "Identifiable.hpp"
#include "RTree.hpp"
#include "Updatable.hpp"

class RTreeGeometry : public Identifiable<std::string>, public Updatable {
  public:
    RTreeGeometry(const std::string& id, std::unique_ptr<Framed>&& frame, RTree<>& rtree);
    void update() override;

  private:
    std::unique_ptr<Framed> frame_;
    FloatBox old_frame_;
    RTree<>& rtree_;
};
