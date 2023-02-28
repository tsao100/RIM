#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RIM.h"

class RIM : public QMainWindow
{
    Q_OBJECT

public:
    RIM(QWidget *parent = nullptr);
    ~RIM();

private:
    Ui::RIMClass ui;

};
