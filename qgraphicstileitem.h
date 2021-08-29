#ifndef QGRAPHICSTILEITEM_H
#define QGRAPHICSTILEITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include "tile.h"
#include "map.h"

class QGraphicsTileItem: public QGraphicsItemGroup
{
    int x, y;
    QGraphicsPixmapItem bg;
    QGraphicsPixmapItem fg;
    QGraphicsPixmapItem layer;
    Tile* tile;
public:
    static Tile** curtile;
    static QGraphicsTileItem* lastmodified;
    static Tile* prev;
    static void undo();
    static void redo();
    QGraphicsTileItem(int i, int j, Tile* _tile);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void setShapeMode(QGraphicsPixmapItem::ShapeMode mode);
    void changeTile();
};

#endif // QGRAPHICSTILEITEM_H
