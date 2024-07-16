#include "path-manager.h"
#include <QApplication>
#include <cmath>
#include <set>

PathManager::PathManager(int argc, char *argv[]) : argc(argc), argv(argv) {
}

void PathManager::create(const std::string &filePath) {
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Path path;
        std::string token;
        std::getline(ss, token, ',');
        path.roadID = std::stoi(token.substr(token.find(":") + 1));
        std::getline(ss, token, ',');
        path.from = token.substr(token.find(":") + 1);
        std::getline(ss, token, ',');
        path.to = token.substr(token.find(":") + 1);
        std::getline(ss, token, ',');
        path.distance = std::stod(token.substr(token.find(":") + 1));
        std::getline(ss, token, ',');
        path.traffic = token.substr(token.find(":") + 1);
        std::getline(ss, token, ',');
        path.travelTime = std::stoi(token.substr(token.find(":") + 1));

        avlTree.root = avlTree.insert(avlTree.root, path);
        linkedList.insert(path);
        pathMap[path.roadID] = path;
    }
    file.close();
}

void PathManager::displayGraph() {
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);

    const int radius = 80;
    const int sceneSize = 1000;
    const double pi = 3.14159265358979323846;

    std::vector<QColor> colors = {
        Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow, Qt::darkRed, Qt::darkGreen, Qt::darkBlue
    };

    std::map<std::string, QGraphicsEllipseItem *> nodeMap;
    std::vector<QPair<std::string, QGraphicsEllipseItem *> > nodes;
    for (const auto &pair: pathMap) {
        const Path &path = pair.second;

        if (nodeMap.find(path.from) == nodeMap.end()) {
            QGraphicsEllipseItem *fromNode = scene.addEllipse(0, 0, radius * 1.5, radius, QPen(Qt::black),
                                                              QBrush(Qt::white));
            nodeMap[path.from] = fromNode;
            nodes.push_back(qMakePair(path.from, fromNode));
        }

        if (nodeMap.find(path.to) == nodeMap.end()) {
            QGraphicsEllipseItem *toNode = scene.addEllipse(0, 0, radius * 1.5, radius, QPen(Qt::black),
                                                            QBrush(Qt::white));
            nodeMap[path.to] = toNode;
            nodes.push_back(qMakePair(path.to, toNode));
        }
    }

    int nodeCount = nodes.size();
    for (int i = 0; i < nodeCount; ++i) {
        double angle = 2 * pi * i / nodeCount;
        int x = sceneSize / 2 + (sceneSize / 3) * std::cos(angle) - (radius * 1.5) / 2;
        int y = sceneSize / 2 + (sceneSize / 3) * std::sin(angle) - radius / 2;
        nodes[i].second->setRect(x, y, radius * 1.5, radius);

        QGraphicsTextItem *label = scene.addText(QString::fromStdString(nodes[i].first), QFont("Arial", 14));
        label->setDefaultTextColor(Qt::black);
        label->setPos(x + (radius * 0.4), y + (radius * 0.3));
    }

    std::set<std::pair<int, int> > occupiedPositions;
    std::map<std::string, int> nodeColorIndex;
    std::map<std::pair<std::string, std::string>, int> pathCount;
    int colorIndex = 0;

    for (const auto &pair: pathMap) {
        const Path &path = pair.second;
        QGraphicsEllipseItem *fromNode = nodeMap[path.from];
        QGraphicsEllipseItem *toNode = nodeMap[path.to];

        if (fromNode && toNode) {
            if (nodeColorIndex.find(path.from) == nodeColorIndex.end()) {
                nodeColorIndex[path.from] = colorIndex++;
            }
            QPen pen(colors[nodeColorIndex[path.from] % colors.size()], 2);
            fromNode->setPen(pen);
            toNode->setPen(pen);

            pathCount[{path.from, path.to}]++;

            QLineF line(fromNode->rect().center(), toNode->rect().center());

            double angle = std::atan2(line.dy(), line.dx());
            line.setP1(QPointF(fromNode->rect().center().x() + (radius * 0.75) * std::cos(angle),
                               fromNode->rect().center().y() + (radius * 0.5) * std::sin(angle)));
            line.setP2(QPointF(toNode->rect().center().x() - (radius * 0.75) * std::cos(angle),
                               toNode->rect().center().y() - (radius * 0.5) * std::sin(angle)));

            QGraphicsLineItem *edge = scene.addLine(line, pen);

            QString edgeLabel = QString("%1min/%2km").arg(path.travelTime).arg(path.distance);
            QGraphicsTextItem *edgeText = scene.addText(edgeLabel, QFont("Arial", 10));
            edgeText->setDefaultTextColor(colors[nodeColorIndex[path.from] % colors.size()]);

            QPointF labelPos((line.p1().x() + line.p2().x()) / 2, (line.p1().y() + line.p2().y()) / 2);
            int attempt = 0;
            while (occupiedPositions.count({
                       static_cast<int>(labelPos.x()), static_cast<int>(labelPos.y())
                   }) && attempt < 10) {
                labelPos += QPointF(10, 10);
                attempt++;
            }
            occupiedPositions.insert({static_cast<int>(labelPos.x()), static_cast<int>(labelPos.y())});
            edgeText->setPos(labelPos);
        }
    }

    view.setScene(&scene);
    view.setFixedSize(sceneSize, sceneSize);
    view.show();
    QApplication::exec();
}

void PathManager::displayAVL() {
    std::vector<Path> paths;
    avlTree.inorder(avlTree.root, paths);
    for (const auto &path: paths) {
        std::cout << path.roadID << " ";
    }
}

void PathManager::displayList() {
    linkedList.display();
}

void PathManager::appendPath(const std::string &filePath, const std::string &userID, const std::string &from,
                             const std::string &to) {
    std::ofstream file(filePath, std::ios::app);
    if (file.is_open()) {
        QDate date = QDate::currentDate();
        std::stringstream ss;
        ss << "\nUserID:" << userID << ",From:" << from << ",To:" << to << ",TravelTime:0,Date:" << date.
                toString("yyyy-MM-dd").toStdString() << ",Alt:0\n";
        file << ss.str();
        file.close();
        std::cout << "New path appended to the file successfully.\n";
    } else {
        std::cerr << "Unable to open file.\n";
    }
}

int AVLTree::height(AVLNode *N) {
    return N ? N->height : 0;
}

int AVLTree::max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode *AVLTree::rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode *AVLTree::leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int AVLTree::getBalance(AVLNode *N) {
    return N ? height(N->left) - height(N->right) : 0;
}

AVLNode *AVLTree::insert(AVLNode *node, Path path) {
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

void AVLTree::inorder(AVLNode *root, std::vector<Path> &paths) {
    if (root != nullptr) {
        inorder(root->left, paths);
        paths.push_back(root->path);
        inorder(root->right, paths);
    }
}

void LinkedList::insert(Path path) {
    ListNode *newNode = new ListNode(path);
    newNode->next = head;
    head = newNode;
}

void LinkedList::display() {
    ListNode *current = head;
    while (current != nullptr) {
        std::cout << current->path.roadID << " ";
        current = current->next;
    }
}
