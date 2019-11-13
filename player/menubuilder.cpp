#include "menubuilder.h"
#include "musicapp.h"


std::shared_ptr<QMenu> MenuBuilder::createSubMenu(std::shared_ptr<QMenu> baseMenu, const char* title) noexcept {
    std::shared_ptr<QMenu> subMenu(baseMenu->addMenu(QString(title)));
    return subMenu;
}

QList<QAction*> MenuBuilder::addActionsToMenu(std::shared_ptr<QMenu> menu, const playlistMapT &playlistsMap) noexcept {
    QList<QAction*> listOfActions;

    for (const auto& playlist : playlistsMap) {
        listOfActions.append(new QAction(QString(playlist.first.c_str()), menu.get()));
    }
    menu->addActions(listOfActions);

    return listOfActions;
}

QList<QAction*> MenuBuilder::addActionsToMenu(std::shared_ptr<QMenu> menu, const std::string &item) noexcept {
    QList<QAction*> listOfActions;
    listOfActions.append(new QAction(QString(item.c_str()), menu.get()));
    menu->addActions(listOfActions);

    return listOfActions;
}
