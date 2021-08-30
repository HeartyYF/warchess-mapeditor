#ifndef MAP_H
#define MAP_H

#include <QJsonObject>
#include <QJsonArray>
#include <QListWidget>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsScene>
#include <QMap>
#include <list>
#include <vector>
#include <algorithm>
#include "qgraphicstileitem.h"
#include "tile.h"
#include "character.h"
using std::list;
using std::vector;
using std::fill;

class Map
{
    QString name;
    QHash<QListWidgetItem*, Tile*> tilefind;
    QMap<QString, Tile*> tilename;
    QHash<QListWidgetItem*, Character*> charfind;
    QMap<QString, Character*> charname;
    list<Tile*> tilelist;
    list<Character*> charlist;
    QJsonArray win;
    QJsonArray lose;
    int width;
    int height;

public:
    Map(const QJsonObject& mapjson, const QDir curdir, QListWidget* listwidget, QListWidget* special);
    ~Map();
    Tile* find(QListWidgetItem* const&) const;
    Tile* find(const QString&) const;
    Character* findc(QListWidgetItem* const&) const;
    Character* findc(const QString&) const;
    Character* getChara() const;
    void summonItems(QGraphicsScene*& scene);
    static vector<vector<Tile*>> mapinfo;
    static vector<vector<Character*>> charinfo;
    static bool mode;
    QJsonObject toJson();
    Tile* defTile;
};

#endif // MAP_H
