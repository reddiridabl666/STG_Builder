#include "Leaderboards.hpp"

namespace engine {
LeaderboardsView::LeaderboardsView(std::unique_ptr<Displayable>&& bg, std::unique_ptr<Text>&& num,
                                   std::unique_ptr<Text>&& name, std::unique_ptr<Text>&& score,
                                   std::unique_ptr<Text>&& day, std::unique_ptr<Button>&& back) {}
void LeaderboardsView::set(const LeaderboardsManager::list&, assets::Manager& assets) {}
LeaderboardsView::Entry::Entry(size_t id, const LeaderboardsManager::Entry& entry, const TextProps& text_props_,
                               const nl::json& bg_props_, assets::Manager& assets) {}
}  // namespace engine
