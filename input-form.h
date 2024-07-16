#ifndef INPUT_FORM_H
#define INPUT_FORM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "path-manager.h"

class InputForm : public QDialog {
    Q_OBJECT

public:
    InputForm(PathManager &manager);

    private slots:
        void handleSubmit();

private:
    QLineEdit *userIDInput;
    QLineEdit *fromInput;
    QLineEdit *toInput;
    PathManager &pathManager;
};

#endif // INPUT_FORM_H
