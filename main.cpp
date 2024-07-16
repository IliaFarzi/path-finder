#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
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

    AVLNode(Path p) : path(p), left(nullptr), right(nullptr), height(1) {
    }
};

class AVLTree {
public:
    AVLNode *root;

    AVLTree() : root(nullptr) {
    }

    // Helper functions for AVL tree
    int height(AVLNode *N) {
        return N ? N->height : 0;
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    AVLNode *rightRotate(AVLNode *y) {
        AVLNode *x = y->left;
        AVLNode *T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode *leftRotate(AVLNode *x) {
        AVLNode *y = x->right;
        AVLNode *T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    int getBalance(AVLNode *N) {
        return N ? height(N->left) - height(N->right) : 0;
    }

    AVLNode *insert(AVLNode *node, Path path) {
        if (!node)
            return new AVLNode(path);
        if (path.roadID < node->path.roadID)
            node->left = insert(node->left, path);
        else if (path.roadID > node->path.roadID)
            node->right = insert(node->right, path);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);
        if (balance > 1 && path.roadID < node->left->path.roadID)
            return rightRotate(node);
        if (balance < -1 && path.roadID > node->right->path.roadID)
            return leftRotate(node);
        if (balance > 1 && path.roadID > node->left->path.roadID) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && path.roadID < node->right->path.roadID) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inorder(AVLNode *root, vector<Path> &paths) {
        if (root != nullptr) {
            inorder(root->left, paths);
            paths.push_back(root->path);
            inorder(root->right, paths);
        }
    }
};

struct ListNode {
    Path path;
    ListNode *next;

    ListNode(Path p) : path(p), next(nullptr) {
    }
};

class LinkedList {
public:
    ListNode *head;

    LinkedList() : head(nullptr) {
    }

    void insert(Path path) {
        ListNode *newNode = new ListNode(path);
        newNode->next = head;
        head = newNode;
    }

    void display() {
        ListNode *current = head;
        while (current != nullptr) {
            cout << current->path.roadID << " ";
            current = current->next;
        }
    }
};

class PathManager {
    AVLTree avlTree;
    LinkedList linkedList;
    map<int, Path> pathMap;
    int argc;
    char **argv;

public:
    PathManager(int argc, char *argv[]) : argc(argc), argv(argv) {
    }

    void create(const string &filePath) {
    ifstream file(filePath);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Path path;
        string token;
        getline(ss, token, ',');
        path.roadID = stoi(token.substr(token.find(":") + 1));
        getline(ss, token, ',');
        path.from = token.substr(token.find(":") + 1);
        getline(ss, token, ',');
        path.to = token.substr(token.find(":") + 1);
        getline(ss, token, ',');
        path.distance = stod(token.substr(token.find(":") + 1));
        getline(ss, token, ',');
        path.traffic = token.substr(token.find(":") + 1);
        getline(ss, token, ',');
        path.travelTime = stoi(token.substr(token.find(":") + 1));

        avlTree.root = avlTree.insert(avlTree.root, path);
        linkedList.insert(path);
        pathMap[path.roadID] = path;
    }
    file.close();
}

    void display() {
        // Placeholder for now
        QApplication app(argc, argv);
        QWidget window;
        QVBoxLayout *layout = new QVBoxLayout();

        vector<Path> paths;
        avlTree.inorder(avlTree.root, paths);

        for (const auto &path: paths) {
            QString pathInfo = QString("RoadID: %1, From: %2, To: %3, Distance: %4, Traffic: %5, TravelTime: %6")
                    .arg(path.roadID)
                    .arg(QString::fromStdString(path.from))
                    .arg(QString::fromStdString(path.to))
                    .arg(path.distance)
                    .arg(QString::fromStdString(path.traffic))
                    .arg(path.travelTime);
            QLabel *label = new QLabel(pathInfo);
            layout->addWidget(label);
        }

        window.setLayout(layout);
        window.setWindowTitle("Path Information");
        window.show();

        app.exec();
    }

    void displayAVL() {
        vector<Path> paths;
        avlTree.inorder(avlTree.root, paths);
        for (const auto &path: paths) {
            cout << path.roadID << " ";
        }
    }

    void displayList() {
        linkedList.display();
    }
};

int main(int argc, char *argv[]) {
    PathManager pm(argc, argv);
    pm.create("data/traffic-paths.txt");

    cout << "AVL Tree Inorder Traversal: ";
    pm.displayAVL();
    cout << "\n";

    cout << "Linked List: ";
    pm.displayList();
    cout << "\n";

    pm.display();
    return 0;
}
