#include "messagewindow.h"
#include "ui_messagewindow.h"

MessageWindow::MessageWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageWindow)
{
    ui->setupUi(this);
    ui->label_info->setWordWrap(true);
}

MessageWindow::~MessageWindow()
{
    delete ui;
}

void MessageWindow::setText(QString text)
{
    ui->label_info->setText(text);
}
