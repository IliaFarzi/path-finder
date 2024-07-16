#include <QApplication>
#include "main-window.h"
#include "path-manager.h"
#include "input-form.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidgetItem>
#include <fstream>
#include <sstream>

using namespace std;

MainWindow::MainWindow(PathManager &manager, QWidget *parent)
    : QMainWindow(parent), pathManager(manager) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    tableWidget = new QTableWidget(0, 5, this);
    tableWidget->setHorizontalHeaderLabels({"User ID", "Date", "From", "To", "Travel Time"});
    layout->addWidget(tableWidget);

    QPushButton *updateButton = new QPushButton("Update Requasts Table", this);
    layout->addWidget(updateButton);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::processRequests);

    QPushButton *showGraphButton = new QPushButton("Show Map", this);
    layout->addWidget(showGraphButton);
    connect(showGraphButton, &QPushButton::clicked, this, &MainWindow::showGraph);

    QPushButton *showInputFormButton = new QPushButton("Add New Requasts", this);
    layout->addWidget(showInputFormButton);
    connect(showInputFormButton, &QPushButton::clicked, this, &MainWindow::showInputForm);

    graphicsView = new QGraphicsView(this);
    layout->addWidget(graphicsView);

    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);

    setCentralWidget(centralWidget);

    processRequests();
}

void MainWindow::updateTable() {
    tableWidget->setRowCount(0);
    ifstream file("data/requastes.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        string userID, date, from, to, travelTimeStr;
        getline(ss, token, ',');
        userID = token.substr(token.find(":") + 1);
        getline(ss, token, ',');
        from = token.substr(token.find(":") + 1);
        getline(ss, token, ',');
        to = token.substr(token.find(":") + 1);
        getline(ss, token, ',');
        travelTimeStr = token.substr(token.find(":") + 1);
        getline(ss, token, ',');
        date = token.substr(token.find(":") + 1);

        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(userID)));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(date)));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(from)));
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(to)));
        if (travelTimeStr == "0") {
            tableWidget->setItem(row, 4, new QTableWidgetItem("No way"));
        } else {
            tableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(travelTimeStr)));
        }
    }
    file.close();
}

void MainWindow::processRequests() {
    pathManager.processRequests("data/requastes.txt", "data/requastes.txt");
    updateTable();
}

void MainWindow::showGraph() {
    scene->clear();
    pathManager.displayGraph(scene);
}

void MainWindow::showInputForm() {
    InputForm form(pathManager);
    form.exec();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    PathManager pathManager(argc, argv);
    pathManager.create("data/traffic-paths.txt");

    MainWindow mainWindow(pathManager);
    mainWindow.show();

    return app.exec();
}
