#include "RIM.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <QtWidgets>

using namespace std;
int ReadPrj(QString fileName);

RIM::RIM(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    QObject::connect(ui.openAction, &QAction::triggered, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), "D:\\AlignmentTools\\ini", QObject::tr("All Files (*.prj)"));
        if (!fileName.isEmpty()) {
            // Do something with the selected file here
            qDebug() << "Selected file:" << fileName;
            ReadPrj(fileName);
        }
        });

    QObject::connect(ui.helloAction, &QAction::triggered, [&]() {
        QMessageBox::question(this, "Message", "Hello!", QMessageBox::Yes | QMessageBox::No);
        });


}

RIM::~RIM()
{}

int ReadPrj(QString fileName) {
    // Open the text file for reading
    ifstream file(fileName.toStdString());

    // Create a vector to hold the strings
    vector<string> lines;

    // Read the file line by line
    string line;
    while (getline(file, line)) {
        // Add each line to the vector
        lines.push_back(line);
    }

    // Close the file
    file.close();

    // Print out the lines in the vector
    QString s = "";
    for (string str : lines) {
        qDebug() << str.c_str();
        s = s + str.c_str() + QString("\n");
    }

    QMessageBox::question(nullptr, fileName, s, QMessageBox::Yes | QMessageBox::No);


    return 0;
}
