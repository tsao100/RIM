#include "RIM.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <QtWidgets>
#include "..\AlignmentClassLibraryTest\AlignmentLib.h"

using namespace std;
int ReadPrj(QString fileName, Ui_RIMClass& ui);
int ReadHALD(const char* filename);
int ReadVALD(QString fileName);
TrackProject tp;
TrackCenterline* tcl;

RIM::RIM(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    // Create a dockable TextEdit widget
    QTextEdit* textEdit = new QTextEdit(this);
    //setCentralWidget(textEdit);
    QDockWidget* dock = new QDockWidget(tr("Dockable TextEdit"), this);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(textEdit);
    addDockWidget(Qt::LeftDockWidgetArea, dock);


    QObject::connect(ui.newAction, &QAction::triggered, [&]() {
        //Todo: Unload RIM. initialize for the new RIM. Do not know how to implement so far. March 1st, 2023.
        QMessageBox::question(this, "Menu", "New!", QMessageBox::Yes | QMessageBox::No);
        });

    QObject::connect(ui.openAction, &QAction::triggered, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), "D:\\AlignmentTools\\ini", QObject::tr("All Files (*.rim)"));
        if (!fileName.isEmpty()) {
            // Do something with the selected file here
            qDebug() << "Selected file:" << fileName;
            //tp.loadFromFile(fileName.toStdString());
        }
        });

    QObject::connect(ui.saveAction, &QAction::triggered, [&]() {
        QMessageBox::question(this, "Menu", "Save!", QMessageBox::Yes | QMessageBox::No);
        QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), "D:\\AlignmentTools\\ini", QObject::tr("All Files (*.rim)"));
        if (!fileName.isEmpty()) {
            // Do something with the selected file here
            qDebug() << "Selected file:" << fileName;
            //tp.saveToFile(fileName.toStdString());
        }
        });

    QObject::connect(ui.saveAsAction, &QAction::triggered, [&]() {
        QMessageBox::question(this, "Menu", "Save As!", QMessageBox::Yes | QMessageBox::No);
        QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), "D:\\AlignmentTools\\ini", QObject::tr("All Files (*.rim)"));
        if (!fileName.isEmpty()) {
            // Do something with the selected file here
            qDebug() << "Selected file:" << fileName;
            //tp.saveToFile(fileName.toStdString());
        }
        });

    QObject::connect(ui.importAction, &QAction::triggered, [&]() {
        //QMessageBox::question(this, "Menu", "Import...", QMessageBox::Yes | QMessageBox::No);
        //QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), "D:\\AlignmentTools\\ini", QObject::tr("All Files (*.prj)"));
        //if (!fileName.isEmpty()) {
            // Do something with the selected file here
            //qDebug() << "Selected file:" << fileName;
        //ReadPrj(fileName);
        ReadPrj("D:\\AlignmentTools\\ini\\CP04.prj", ui);
        //}
        });


    
    // Add items to the list control
    //ui.listWidget->addItem("Item 1");
    //ui.listWidget->addItem("Item 2");
    //ui.listWidget->addItem("Item 3");
    //ui.listWidget->setSelectionMode(ui.listWidget->MultiSelection);


    // Connect a slot to the list control's item selection signal
    QObject::connect(ui.listWidget, &QListWidget::itemSelectionChanged, [&]() {
        // Get the selected item(s)
        QList<QListWidgetItem*> selectedItems = ui.listWidget->selectedItems();

        // Do something with the selected item(s)
        for (QListWidgetItem* selectedItem : selectedItems) {
            qDebug() << "Selected item: " << selectedItem->text();
            tcl = tp.getCenterline(selectedItem->text().toStdString());
        }
        });

}

RIM::~RIM()
{}

struct Record {
    char Plat[3];
    char UpDown;
    char TSC[2];
    char Easting[16];
    char Northing[17];
    char Chainage[15];
    char ContinuousChainage[15];
    char Azimuth[13];
    char length[15];
    char RadiusCurveType[8];
    char CircularCurveNo[9];
    double Cant;
    double GaugeWidenning;
    double SpeedLimit;
    char Text1[25];
    char Text2[25];
    double Real1;
    double Real2;
};

struct _Record {
    char Plat[3];
    char UpDown;
    char TSC[2];
    double Easting;
    double Northing;
    double Chainage;
    double ContinuousChainage;
    double Azimuth;
    double length;
    double Radius;
    char CurveType[8];
    char CircularCurveNo[9];
    double Cant;
    double GaugeWidenning;
    double SpeedLimit;
    char Text1[25];
    char Text2[25];
    double Real1;
    double Real2;
};

bool isNumeric(char ct[8]) {
    // Iterate through each character in the string
    for (int i = 0; i<8 && ct[i] != ' '; i++) {
        // Check if the current character is not a digit
        if (!isdigit(ct[i])) {
            return false;
        }
    }
    // All characters are digits
    return true;
}

int ReadPrj(QString fileName, Ui_RIMClass& ui) {
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
        //s = s + str.c_str() + QString("\n");
        //path += str;
        ReadHALD(str.c_str());
        ui.listWidget->addItem(QString(str.c_str()));
    }

    //QMessageBox::question(nullptr, fileName, s, QMessageBox::Yes | QMessageBox::No);


    return 0;
}

int ReadVALD(QString fileName) {
    return 0;
}

void serializeRecords(const std::vector<Record>& records, const char* filename) {
    // Open the file for writing in binary mode
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Failed to open file for writing\n";
        return;
    }

    // Write the number of records to the file
    //size_t numRecords = records.size();
    //file.write(reinterpret_cast<const char*>(&numRecords), sizeof(numRecords));

    // Write each record to the file
    for (const Record& record : records) {
        file.write(record.Plat, sizeof(record.Plat));
        file.write(&record.UpDown, sizeof(record.UpDown));
        file.write(record.TSC, sizeof(record.TSC));
        file.write(record.Easting, sizeof(record.Easting));
        file.write(record.Northing, sizeof(record.Northing));
        file.write(record.Chainage, sizeof(record.Chainage));
        file.write(record.ContinuousChainage, sizeof(record.ContinuousChainage));
        file.write(record.Azimuth, sizeof(record.Azimuth));
        file.write(record.length, sizeof(record.length));
        file.write(record.RadiusCurveType, sizeof(record.RadiusCurveType));
        file.write(record.CircularCurveNo, sizeof(record.CircularCurveNo));
        file.write(reinterpret_cast<const char*>(&record.Cant), sizeof(record.Cant));
        file.write(reinterpret_cast<const char*>(&record.GaugeWidenning), sizeof(record.GaugeWidenning));
        file.write(reinterpret_cast<const char*>(&record.SpeedLimit), sizeof(record.SpeedLimit));
        file.write(record.Text1, sizeof(record.Text1));
        file.write(record.Text2, sizeof(record.Text2));
        file.write(reinterpret_cast<const char*>(&record.Real1), sizeof(record.Real1));
        file.write(reinterpret_cast<const char*>(&record.Real2), sizeof(record.Real2));
    }

    // Close the file
    file.close();
}

_Record convertRecord(Record r) {
    _Record rec;
    memcpy(rec.Plat, r.Plat, sizeof(rec.Plat));
    rec.UpDown = r.UpDown;
    memcpy(rec.TSC, r.TSC, sizeof(rec.TSC));
    rec.Easting = atof(r.Easting);
    rec.Northing = atof(r.Northing);
    rec.Chainage = atof(r.Chainage);
    rec.ContinuousChainage = atof(r.ContinuousChainage);

    Azimuth azi(r.Azimuth);
    rec.Azimuth = azi.ToDegrees();

    rec.length = atof(r.length);

    QString strRadiusCurveType(r.RadiusCurveType);
    bool ok;
    double radius = strRadiusCurveType.toDouble(&ok);

    if (isNumeric(r.RadiusCurveType))
    {
        rec.Radius = atof(r.RadiusCurveType);
        memcpy(rec.CurveType, "ARC", sizeof("ARC"));
    }
    else
    {
        memcpy(rec.CurveType, r.RadiusCurveType, sizeof(rec.CurveType));
    }    
    memcpy(rec.CircularCurveNo, r.CircularCurveNo, sizeof(rec.CircularCurveNo));
    rec.Cant = r.Cant;
    rec.GaugeWidenning = r.GaugeWidenning;
    rec.SpeedLimit = r.SpeedLimit;
    memcpy(rec.Text1, r.Text1, sizeof(rec.Text1));
    memcpy(rec.Text2, r.Text2, sizeof(rec.Text2));
    rec.Real1 = r.Real1;
    rec.Real2 = r.Real2;
    return rec;
}

std::vector<_Record> deserializeFile(const char* filename) {
    std::vector<Record> result;
    std::vector<_Record> _result;
    std::ifstream file(filename, std::ios::binary);
    if (file) {
        // determine file size
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // read records
        size_t recordSize = sizeof(Record);
        size_t numRecords = fileSize / recordSize;
        result.resize(numRecords);
        _result.resize(numRecords);
        for (size_t i = 0; i < numRecords; ++i) {
            Record& record = result[i];
            _Record& _record = _result[i];
            file.read(record.Plat, 3);
            file.read(&record.UpDown, 1);
            file.read(record.TSC, 2);
            file.read(record.Easting, 16);
            file.read(record.Northing, 17);
            file.read(record.Chainage, 15);
            file.read(record.ContinuousChainage, 15);
            file.read(record.Azimuth, 13);
            file.read(record.length, 15);
            file.read(record.RadiusCurveType, 8);
            file.read(record.CircularCurveNo, 9);
            file.read(reinterpret_cast<char*>(&record.Cant), sizeof(double));
            file.read(reinterpret_cast<char*>(&record.GaugeWidenning), sizeof(double));
            file.read(reinterpret_cast<char*>(&record.SpeedLimit), sizeof(double));
            file.read(record.Text1, 25);
            file.read(record.Text2, 25);
            file.read(reinterpret_cast<char*>(&record.Real1), sizeof(double));
            file.read(reinterpret_cast<char*>(&record.Real2), sizeof(double));
            _record = convertRecord(record);
        }
    }
    return _result;
}

int ReadHALD(const char* filename) {
    vector<_Record> result;
    TrackCenterline* tcl = new TrackCenterline(filename);
    string path("D:\\AlignmentTools\\DataTable\\");
    path += filename;
    result = deserializeFile(path.c_str());
    for (size_t i = 0; i < result.size() - 1; i++)
    {
        CTangent* t = NULL;
        CCircularArc* c = NULL;
        CSpiral* sp = NULL;
        CHalfSine* hs = NULL;
        string tsc = "";
        // Switch statement to handle different types of curves
        switch (GetCurveType(result[i].CurveType)) {
        case acLib::STRAIGHT:
            t = new CTangent(Point(result[i].Easting, result[i].Northing),
                result[i].Chainage, result[i].Azimuth, result[i].length);
            tcl->addCurve(t);
            break;
        case acLib::SPIRAL:
            tsc = string(result[i].TSC, 2);
            if (tsc == "TS") {
                sp = new CSpiral(Point(result[i].Easting, result[i].Northing),
                    result[i].Chainage, result[i].Azimuth, result[i].length, INFINITY,
                    result[i + 1].Radius * LeftRightByAzimuth(result[i].Azimuth, result[i + 1].Azimuth));
            }
            if (tsc == "CS") {
                sp = new CSpiral(Point(result[i].Easting, result[i].Northing),
                    result[i].Chainage, result[i].Azimuth, result[i].length,
                    result[i - 1].Radius * LeftRightByAzimuth(result[i].Azimuth, result[i + 1].Azimuth), INFINITY);
            }
            tcl->addCurve(sp);
            break;
        case acLib::ARC:
            c = new CCircularArc(Point(result[i].Easting, result[i].Northing),
                result[i].Chainage, result[i].Azimuth, result[i].length,
                result[i].Radius * LeftRightByAzimuth(result[i].Azimuth, result[i + 1].Azimuth));
            tcl->addCurve(c);
            break;
        case acLib::HALFSINE:
            tsc = string(result[i].TSC, 2);
            if (tsc == "TS") {
                hs = new CHalfSine(Point(result[i].Easting, result[i].Northing),
                    result[i].Chainage, result[i].Azimuth, result[i].length, INFINITY,
                    result[i + 1].Radius * LeftRightByAzimuth(result[i].Azimuth, result[i + 1].Azimuth));
            }
            if (tsc == "CS") {
                hs = new CHalfSine(Point(result[i].Easting, result[i].Northing),
                    result[i].Chainage, result[i].Azimuth, result[i].length, INFINITY,
                    result[i - 1].Radius * LeftRightByAzimuth(result[i].Azimuth, result[i + 1].Azimuth));
            }
            tcl->addCurve(hs);
            break;
        case acLib::CUBICECI:
            tsc = string(result[i].TSC, 2);
            if (tsc == "TS") {
                sp = new CSpiral(Point(result[i].Easting, result[i].Northing),
                    result[i].Chainage, result[i].Azimuth, result[i].length, INFINITY,
                    result[i + 1].Radius * LeftRightByAzimuth(result[i].Azimuth, result[i + 1].Azimuth));
            }
            if (tsc == "CS") {
                sp = new CSpiral(Point(result[i].Easting, result[i].Northing),
                    result[i].Chainage, result[i].Azimuth, result[i].length, INFINITY,
                    result[i - 1].Radius * LeftRightByAzimuth(result[i].Azimuth, result[i + 1].Azimuth));
            }
            tcl->addCurve(sp);
            break;
        case acLib::CUBICJPN:
            tsc = string(result[i].TSC, 2);
            if (tsc == "TS") {
                sp = new CSpiral(Point(result[i].Easting, result[i].Northing),
                    result[i].Chainage, result[i].Azimuth, result[i].length, INFINITY,
                    result[i + 1].Radius * LeftRightByAzimuth(result[i].Azimuth, result[i + 1].Azimuth));
            }
            if (tsc == "CS") {
                sp = new CSpiral(Point(result[i].Easting, result[i].Northing),
                    result[i].Chainage, result[i].Azimuth, result[i].length,
                    result[i - 1].Radius * LeftRightByAzimuth(result[i].Azimuth, result[i + 1].Azimuth), INFINITY);
            }
            tcl->addCurve(sp);
            break;
        default:
            //cout << "Unknown curve type" << endl;
            break;
        }

    }
    tp.AddCurve(tcl);

    return 0;
}
