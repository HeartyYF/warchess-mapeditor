#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->special->hide();
    QGraphicsTileItem::curtile = &curtile;
    QGraphicsTileItem::curchar = &curchar;
    connect(ui->load, SIGNAL(triggered()), this, SLOT(loadMap()));
    connect(ui->save, SIGNAL(triggered()), this, SLOT(saveMap()));
    connect(ui->saveto, SIGNAL(triggered()), this, SLOT(savetoMap()));
    connect(ui->undo, SIGNAL(triggered()), this, SLOT(undo()));
    connect(ui->toggle, SIGNAL(triggered()), this, SLOT(toggle()));
    map = nullptr;
    path.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMap()
{
    const QString filename = QFileDialog::getOpenFileName(this, "选择存档文件", ".", "存档(*.json)");
    if(filename.size() == 0)
    {
        return;
    }
    if(map != nullptr)
    {
        delete scene;
        delete view;
        disconnect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(on_listWidget_currentItemChanged(QListWidgetItem*, QListWidgetItem*)));
        disconnect(ui->special, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(on_special_currentItemChanged(QListWidgetItem*, QListWidgetItem*)));
        ui->listWidget->clear();
        ui->special->clear();
        connect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(on_listWidget_currentItemChanged(QListWidgetItem*, QListWidgetItem*)));
        connect(ui->special, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(on_special_currentItemChanged(QListWidgetItem*, QListWidgetItem*)));
        saveMap();
        delete map;
    }
    path = filename;
    QGraphicsTileItem::lastmodified = nullptr;
    QGraphicsTileItem::prev = nullptr;
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::gray);
    view = new QGraphicsView(scene, ui->centralwidget);
    view->resize(QSize(1024, 720));
    view->move(QPoint(256, 0));
    view->setTransformationAnchor(QGraphicsView::NoAnchor);
    view->show();
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QString value = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonObject json = document.object();
    map = new Map(json, QFileInfo(file).path(), ui->listWidget, ui->special);
    map->summonItems(scene);
    QGraphicsTileItem::view = view;
    curtile = map->defTile;
    QGraphicsTileItem::defTile = map->defTile;
    curchar = map->getChara();
}

void MainWindow::saveMap()
{
    if(map == nullptr)
    {
        QMessageBox message(QMessageBox::NoIcon, "警告", "当前未打开任何地图！");
        message.exec();
        return;
    }
    QFile file(path);
    file.open(QFile::WriteOnly);
    QJsonDocument document(map->toJson());
    file.write(document.toJson());
}

void MainWindow::savetoMap()
{
    if(map == nullptr)
    {
        QMessageBox message(QMessageBox::NoIcon, "警告", "当前未打开任何地图！");
        message.exec();
        return;
    }
    const QString filename = QFileDialog::getSaveFileName(this, "选择存档文件", ".", "存档(*.json)");
    if(filename.size() == 0)
    {
        return;
    }
    path = filename;
    QFile file(path);
    file.open(QFile::WriteOnly);
    QJsonDocument document(map->toJson());
    file.write(document.toJson());
}

void MainWindow::undo()
{
    if(QGraphicsTileItem::lastmodified == nullptr)
    {
        return;
    }
    QGraphicsTileItem::undo();
}

void MainWindow::toggle()
{
    Map::mode = !Map::mode;
    ui->special->setVisible(Map::mode);
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    curtile = map->find(current);
}

void MainWindow::on_special_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    curchar = map->findc(current);
}

