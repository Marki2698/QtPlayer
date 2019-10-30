#ifndef MENUBUILDER_H
#define MENUBUILDER_H
#include <QMenu>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include "musicapp.h"
#include "types.h"


namespace MenuBuilder {
    std::shared_ptr<QMenu> createSubMenu(std::shared_ptr<QMenu> baseMenu, const char* title) noexcept;
    QList<QAction*> addActionsToMenu(std::shared_ptr<QMenu> menu, const songsMapT<>& playlistsMap) noexcept;
    QList<QAction*> addActionsToMenu(std::shared_ptr<QMenu> menu, const std::string& item) noexcept; // maybe use templates with traits instead of overloading

}

#endif // MENUBUILDER_H
