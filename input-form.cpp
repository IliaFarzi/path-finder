#include "input-form.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

InputForm::InputForm(PathManager &manager) : QDialog(), pathManager(manager) {
    QVBoxLayout *layout = new QVBoxLayout;

    QFormLayout *formLayout = new QFormLayout;
    userIDInput = new QLineEdit;
    fromInput = new QLineEdit;
    toInput = new QLineEdit;

    formLayout->addRow("User ID:", userIDInput);
    formLayout->addRow("From:", fromInput);
    formLayout->addRow("To:", toInput);
    layout->addLayout(formLayout);

    QPushButton *submitButton = new QPushButton("Submit");
    layout->addWidget(submitButton);
    connect(submitButton, &QPushButton::clicked, this, &InputForm::handleSubmit);

    setLayout(layout);
}

void InputForm::handleSubmit() {
    string userID = userIDInput->text().toStdString();
    string from = fromInput->text().toStdString();
    string to = toInput->text().toStdString();

    if (!userID.empty() && !from.empty() && !to.empty()) {
        pathManager.appendPath("data/requastes.txt", userID, from, to);
        QMessageBox::information(this, "Success", "Path added successfully!");
        close();
    } else {
        QMessageBox::warning(this, "Input Error", "Please fill all the fields.");
    }
}
