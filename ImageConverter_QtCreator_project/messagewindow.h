#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QDialog>

namespace Ui {
class MessageWindow;
}

class MessageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MessageWindow(QWidget *parent = nullptr);
    ~MessageWindow();

private:
    Ui::MessageWindow *ui;

public slots:
    void setText(QString text);

};

#endif // MESSAGEWINDOW_H
