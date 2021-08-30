#ifndef QGRAPHICSTILEITEM_H
#define QGRAPHICSTILEITEM_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include "tile.h"
#include "character.h"
#include "map.h"

class QGraphicsTileItem: public QGraphicsItemGroup
{
    int x, y;
    QGraphicsPixmapItem bg;
    QGraphicsPixmapItem chara;
    QGraphicsPixmapItem fg;
    Tile* tile;
    static QPointF clickpos;
    static QPointF dragpos;
public:
    static Tile** curtile;
    static Tile* defTile;
    static Character** curchar;
    static QGraphicsTileItem* lastmodified;
    static Tile* prev;
    static QGraphicsView* view;
    static void undo();
    static void redo();
    QGraphicsTileItem(int i, int j, Tile* _tile);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void setShapeMode(QGraphicsPixmapItem::ShapeMode mode);
    void changeTile(Tile* _tile = *curtile);
    void setCharImage(const QPixmap pixmap);
};

#endif // QGRAPHICSTILEITEM_H
