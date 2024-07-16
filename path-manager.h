#ifndef PATH_MANAGER_H
#define PATH_MANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <QPair>
#include <QString>
#include <QDate>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsPathItem>

using namespace std;

struct Path {
    int roadID;
    string from;
    string to;
    double distance;
    string traffic;
    int travelTime;
};

struct AVLNode {
    Path path;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(Path p) : path(p), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
public:
    AVLNode *root;

    AVLTree() : root(nullptr) {}

    int height(AVLNode *N);
    int max(int a, int b);
    AVLNode *rightRotate(AVLNode *y);
    AVLNode *leftRotate(AVLNode *x);
    int getBalance(AVLNode *N);
    AVLNode *insert(AVLNode *node, Path path);
    void inorder(AVLNode *root, vector<Path> &paths);
};

class LinkedList {
public:
    struct ListNode {
        Path path;
        ListNode *next;

        ListNode(Path p) : path(p), next(nullptr) {}
    };

    ListNode *head;

    LinkedList() : head(nullptr) {}

    void insert(Path path);
    void display();
};

class PathManager : public QObject {
    Q_OBJECT

    AVLTree avlTree;
    LinkedList linkedList;
    map<int, Path> pathMap;
    map<string, vector<pair<string, int>>> graph;
    int argc;
    char **argv;

public:
    PathManager(int argc, char *argv[]);
    void create(const string &filePath);
    void displayGraph(QGraphicsScene *scene);
    void displayAVL();
    void displayList();
    void appendPath(const string &filePath, const string &userID, const string &from, const string &to);
    void processRequests(const string &requestsFilePath, const string &outputFilePath);
    int findShortestPath(const string &from, const string &to);
};

#endif // PATH_MANAGER_H
