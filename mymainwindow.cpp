#include "mymainwindow.h"
#include "./ui_mymainwindow.h"
#include "testtab.h"

#include "myoglwidget.h"

MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    auto testTab = new TestTab{this};
    ui->verticalLayoutTest->addWidget(testTab);

    auto oglWidget = new MyOglWidget{this};
    ui->verticalLayoutOpebnGL->addWidget(oglWidget);
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

