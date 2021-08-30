#include "qgraphicstileitem.h"

QGraphicsTileItem::QGraphicsTileItem(int i, int j, Tile* _tile):
    x(i), y(j), tile(_tile)
{
    bg.setPixmap(*tile->getbImage());
    fg.setPixmap(*tile->getfImage());
    addToGroup(&bg);
    addToGroup(&chara);
    addToGroup(&fg);
    setAcceptDrops(true);
}

void QGraphicsTileItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    clickpos = event->screenPos();
    dragpos = event->screenPos();
}

void QGraphicsTileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if((event->screenPos() - clickpos).manhattanLength() <= 10)
    {
        if(Map::mode)
        {
            if(event->button() == Qt::LeftButton)
            {
                Map::charinfo[x][y] = *curchar;
                chara.setPixmap(*(*curchar)->getImage());
            }
            else if(event->button() == Qt::RightButton)
            {
                Map::charinfo[x][y] = nullptr;
                chara.setPixmap(*Tile::nullImage());
            }
        }
        else
        {
            if(event->button() == Qt::LeftButton)
            {
                changeTile();
            }
            else if(event->button() == Qt::RightButton)
            {
                changeTile(defTile);
            }
        }
    }
}

void QGraphicsTileItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    auto trans = event->lastScreenPos() - dragpos;
    if(trans.manhattanLength() > 10)
    {
        dragpos = event->lastScreenPos();
        view->translate(trans.rx(), trans.ry());
    }
}

void QGraphicsTileItem::changeTile(Tile* _tile)
{
    prev = tile;
    tile = _tile;
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
    chara.setShapeMode(mode);
}

void QGraphicsTileItem::setCharImage(const QPixmap pixmap)
{
    chara.setPixmap(pixmap);
}

Tile** QGraphicsTileItem::curtile = nullptr;
Tile* QGraphicsTileItem::defTile = nullptr;
Character** QGraphicsTileItem::curchar = nullptr;
Tile* QGraphicsTileItem::prev = nullptr;
QGraphicsTileItem* QGraphicsTileItem::lastmodified = nullptr;
QPointF QGraphicsTileItem::clickpos;
QPointF QGraphicsTileItem::dragpos;
QGraphicsView* QGraphicsTileItem::view = nullptr;
