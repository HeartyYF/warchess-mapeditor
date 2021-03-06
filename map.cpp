#include "map.h"

Map::Map(const QJsonObject& mapjson, const QDir curdir, QListWidget* listwidget, QListWidget* special)
{
    name = mapjson["name"].toString();
    width = mapjson["width"].toInt();
    height = mapjson["height"].toInt();
    win = mapjson["win"].toArray();
    lose = mapjson["lose"].toArray();
    if(mapjson.contains("bgm"))
    {
        bgm = mapjson["bgm"].toString();
    }
    mapinfo.resize(width);
    charinfo.resize(width);
    for(auto& i : mapinfo)
    {
        i.resize(height);
        fill(i.begin(), i.end(), nullptr);
    }
    for(auto& i : charinfo)
    {
        i.resize(height);
        fill(i.begin(), i.end(), nullptr);
    }
    const QJsonObject tiles = mapjson["tile"].toObject();
    const QStringList names = tiles.keys();
    for(const QString& name : names)
    {
        const auto tilejson = tiles[name].toObject();
        Tile* tile = new Tile(name, tilejson, curdir);
        tilelist.emplace_back(tile);
        tilename.insert(name, tile);
        if(tilejson.find("pos") != tilejson.end())
        {
            auto pos = (tiles[name].toObject())["pos"].toArray();
            for(const QJsonValue& i : pos)
            {
                auto o = i.toObject();
                mapinfo[o["x"].toInt()][o["y"].toInt()] = tile;
            }
        }
    }
    defTile = tilename[mapjson["default"].toString()];
    for(auto i = tilelist.begin(); i != tilelist.end(); ++i)
    {
        if(*i == defTile)
        {
            qSwap(*tilelist.begin(), *i);
            break;
        }
    }
    for(auto& i : mapinfo)
    {
        for(auto& j : i)
        {
            if(j == nullptr)
            {
                j = defTile;
            }
        }
    }
    for(Tile*& tile : tilelist)
    {
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(QSize(64, 100));
        listwidget->addItem(item);
        QWidget* w = new QWidget();
        auto hlayout = new QHBoxLayout();
        auto bimageLabel = new QLabel();
        bimageLabel->setPixmap(*(tile->getbImage()));
        hlayout->addWidget(bimageLabel);
        auto fimageLabel = new QLabel();
        fimageLabel->setPixmap(*(tile->getfImage()));
        hlayout->addWidget(fimageLabel);
        auto label = new QLabel();
        label->setText(tile->getName());
        hlayout->addWidget(label);
        w->setLayout(hlayout);
        listwidget->setItemWidget(item, w);
        tilefind.insert(item, tile);
    }
    const QJsonObject enemy = mapjson["enemy"].toObject();
    const QStringList enemynames = enemy.keys();
    for(const QString& name : enemynames)
    {
        const auto enemyjson = enemy[name].toObject();
        Character* chara = new Character(name, enemyjson, false, curdir);
        charlist.emplace_back(chara);
        charname.insert(name, chara);
        if(enemyjson.find("pos") != enemyjson.end())
        {
            auto pos = (enemy[name].toObject())["pos"].toArray();
            for(const QJsonValue& i : pos)
            {
                auto o = i.toObject();
                charinfo[o["x"].toInt()][o["y"].toInt()] = chara;
            }
        }
    }
    const QJsonObject ally = mapjson["ally"].toObject();
    const QStringList allynames = ally.keys();
    for(const QString& name : allynames)
    {
        const auto allyjson = ally[name].toObject();
        Character* chara = new Character(name, allyjson, true, curdir);
        charlist.emplace_back(chara);
        charname.insert(name, chara);
        if(allyjson.find("pos") != allyjson.end())
        {
            auto pos = (ally[name].toObject())["pos"].toArray();
            for(const QJsonValue& i : pos)
            {
                auto o = i.toObject();
                charinfo[o["x"].toInt()][o["y"].toInt()] = chara;
            }
        }
    }
    for(Character*& chara : charlist)
    {
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(QSize(64, 100));
        special->addItem(item);
        QWidget* w = new QWidget();
        auto hlayout = new QHBoxLayout();
        auto imageLabel = new QLabel();
        imageLabel->setPixmap(*(chara->getImage()));
        hlayout->addWidget(imageLabel);
        auto label = new QLabel();
        label->setText(chara->getName());
        hlayout->addWidget(label);
        w->setLayout(hlayout);
        special->setItemWidget(item, w);
        charfind.insert(item, chara);
    }
}

Map::~Map()
{
    for(Tile*& tile : tilelist)
    {
        delete tile;
    }
}

Tile* Map::find(QListWidgetItem* const& item) const
{
    return tilefind[item];
}

Tile* Map::find(const QString& name) const
{
    return tilename[name];
}

Character* Map::findc(QListWidgetItem* const& item) const
{
    return charfind[item];
}

Character* Map::findc(const QString& name) const
{
    return charname[name];
}

Character* Map::getChara() const
{
    return *charlist.begin();
}

void Map::summonItems(QGraphicsScene*& scene)
{
    for(auto i = 0; i < width; ++i)
    {
        for(auto j = 0; j < height; ++j)
        {
            QGraphicsTileItem* curItem = new QGraphicsTileItem(i, j, mapinfo[i][j]);
            curItem->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
            curItem->setBoundingRegionGranularity(0.25);
            curItem->setPos(j * 64, i * 64);
            if(charinfo[i][j] != nullptr)
            {
                curItem->setCharImage(*charinfo[i][j]->getImage());
            }
            scene->addItem(curItem);
        }
    }
}

QJsonObject Map::toJson()
{
    QHash<Tile*, QJsonObject*> tilejsons;
    QHash<Tile*, QJsonArray*> tilepos;
    QHash<Character*, QJsonObject*> charjsons;
    QHash<Character*, QJsonArray*> charpos;
    QJsonObject json;
    json.insert("name", name);
    json.insert("width", QJsonValue(width));
    json.insert("height", QJsonValue(height));
    json.insert("default", defTile->getName());
    for(auto& i : tilelist)
    {
        QJsonObject* tilejson = new QJsonObject;
        if(i->getbPath().size())
        {
            tilejson->insert("bimage", i->getbPath());
        }
        if(i->getbPath().size())
        {
            tilejson->insert("fimage", i->getfPath());
        }
        tilejson->insert("pass", QJsonValue(i->canPass()));
        if(i->canPass())
        {
            tilejson->insert("cost", QJsonValue(i->getCost()));
            tilejson->insert("damage", QJsonValue(i->getDamage()));
        }
        QJsonArray* emptyarray = new QJsonArray;
        tilepos.insert(i, emptyarray);
        tilejsons.insert(i, tilejson);
    }
    for(auto i = 0; i < width; ++i)
    {
        for(auto j = 0; j < height; ++j)
        {
            if(mapinfo[i][j] == defTile)
            {
                continue;
            }
            QJsonObject cur;
            cur.insert("x", QJsonValue(i));
            cur.insert("y", QJsonValue(j));
            tilepos[mapinfo[i][j]]->append(cur);
        }
    }
    QJsonObject tiles;
    for(auto& i : tilelist)
    {
        tilejsons[i]->insert("pos", QJsonValue(*tilepos[i]));
        tiles.insert(i->getName(), QJsonValue(*tilejsons[i]));
        delete tilepos[i];
        delete tilejsons[i];
    }
    json.insert("tile", QJsonValue(tiles));
    for(auto& i : charlist)
    {
        QJsonObject* charjson = new QJsonObject;
        charjson->insert("image", i->getPath());
        if(!i->isAlly())
        {
            charjson->insert("hp", QJsonValue(i->gethp()));
            charjson->insert("atk", QJsonValue(i->getatk()));
            charjson->insert("def", QJsonValue(i->getdef()));
            charjson->insert("mov", QJsonValue(i->getmov()));
            charjson->insert("range", QJsonValue(i->getrange()));
        }
        QJsonArray* emptyarray = new QJsonArray;
        charpos.insert(i, emptyarray);
        charjsons.insert(i, charjson);
    }
    for(auto i = 0; i < width; ++i)
    {
        for(auto j = 0; j < height; ++j)
        {
            if(charinfo[i][j] == nullptr)
            {
                continue;
            }
            QJsonObject cur;
            cur.insert("x", QJsonValue(i));
            cur.insert("y", QJsonValue(j));
            charpos[charinfo[i][j]]->append(cur);
        }
    }
    QJsonObject enemy;
    QJsonObject ally;
    for(auto& i : charlist)
    {
        charjsons[i]->insert("pos", QJsonValue(*charpos[i]));
        if(i->isAlly())
        {
            ally.insert(i->getName(), QJsonValue(*charjsons[i]));
        }
        else
        {
            enemy.insert(i->getName(), QJsonValue(*charjsons[i]));
        }
        delete charpos[i];
        delete charjsons[i];
    }
    json.insert("enemy", QJsonValue(enemy));
    json.insert("ally", QJsonValue(ally));
    json.insert("win", QJsonValue(win));
    json.insert("lose", QJsonValue(lose));
    if(!bgm.isEmpty())
    {
        json.insert("bgm", bgm);
    }
    return json;
}

vector<vector<Tile*>> Map::mapinfo;
vector<vector<Character*>> Map::charinfo;
bool Map::mode = false;
