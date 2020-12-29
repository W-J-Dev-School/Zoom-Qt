#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <windows.h>
#include <zoom_sdk.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void initZoomSDK();
};

#endif // MAINWINDOW_H
