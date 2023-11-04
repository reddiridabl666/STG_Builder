#pragma once

#include <memory>

#include "GameObject.hpp"
#include "ObjectEntry.hpp"

struct ObjectEntryFactory {
    static std::unique_ptr<ui::ObjectEntry> create(const nl::json& json, GameObjectTag tag) {
        if (tag != GameObjectTag::Player) {
            auto res = json.template get<std::unique_ptr<ui::CommonEntry>>();
            res->get_stats().from_json(json);
            return res;
        }

        auto res = json.template get<std::unique_ptr<ui::PlayerEntry>>();
        res->get_stats().from_json(json);
        return res;
    }
};
