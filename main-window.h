#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "path-manager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(PathManager &manager, QWidget *parent = nullptr);

    private slots:
        void updateTable();
    void processRequests();
    void showGraph();
    void showInputForm();

private:
    PathManager &pathManager;
    QTableWidget *tableWidget;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
