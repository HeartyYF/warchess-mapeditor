#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QJsonDocument>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include "map.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadMap();
    void saveMap();
    void savetoMap();
    void undo();
    void toggle();
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_special_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::MainWindow *ui;
    QString path;
    QGraphicsScene* scene;
    QGraphicsView* view;
    Map* map;
    Tile* curtile;
    Character* curchar;
};
#endif // MAINWINDOW_H
