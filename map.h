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
using std::list;
using std::vector;
using std::fill;

class Map
{
    QString name;
    QMap<QListWidgetItem*, Tile*> tilefind;
    QMap<QString, Tile*> tilename;
    list<Tile*> tilelist;
    int width;
    int height;

public:
    Map(const QJsonObject& mapjson, const QDir curdir, QListWidget* listwidget);
    ~Map();
    Tile* find(QListWidgetItem* const&) const;
    Tile* find(const QString&) const;
    void summonItems(QGraphicsScene*& scene);
    static vector<vector<Tile*>> mapinfo;
    QJsonObject toJson();
    Tile* defTile;
};

#endif // MAP_H
