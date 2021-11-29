#pragma once

#include <QWidget>

namespace Ui {
class TestTab;
}

class TestTab : public QWidget
{
    Q_OBJECT

public:
    explicit TestTab(QWidget *parent = nullptr);
    ~TestTab();

private slots:
    void on_pushButtonTestAxisSystem_clicked();

    void on_pushButtonTestAll_clicked();

private:
    Ui::TestTab *ui;
};

