#include <QApplication>
#include "path-manager.h"
#include "input-form.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    PathManager pm(argc, argv);
    pm.create("data/traffic-paths.txt");
    pm.displayGraph();

    InputForm form(pm);
    form.show();

    return app.exec();
}
