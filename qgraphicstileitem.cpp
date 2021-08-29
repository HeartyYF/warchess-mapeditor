#include "qgraphicstileitem.h"

QGraphicsTileItem::QGraphicsTileItem(int i, int j, Tile* _tile):
    x(i), y(j), tile(_tile)
{
    bg.setPixmap(*tile->getbImage());
    fg.setPixmap(*tile->getfImage());
    addToGroup(&bg);
    addToGroup(&fg);
    addToGroup(&layer);
}

void QGraphicsTileItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    changeTile();
}

void QGraphicsTileItem::changeTile()
{
    prev = tile;
    tile = *curtile;
    bg.setPixmap(*tile->getbImage());
    fg.setPixmap(*tile->getfImage());
    lastmodified = this;
    Map::mapinfo[x][y] = tile;
}

void QGraphicsTileItem::undo()
{
    qSwap(lastmodified->tile, prev);
    lastmodified->bg.setPixmap(*lastmodified->tile->getbImage());
    lastmodified->fg.setPixmap(*lastmodified->tile->getfImage());
    Map::mapinfo[lastmodified->x][lastmodified->y] = lastmodified->tile;
}

void QGraphicsTileItem::setShapeMode(QGraphicsPixmapItem::ShapeMode mode)
{
    bg.setShapeMode(mode);
    fg.setShapeMode(mode);
    layer.setShapeMode(mode);
}

Tile** QGraphicsTileItem::curtile = nullptr;
Tile* QGraphicsTileItem::prev = nullptr;
QGraphicsTileItem* QGraphicsTileItem::lastmodified = nullptr;
