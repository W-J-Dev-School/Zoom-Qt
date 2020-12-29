#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QPushButton>
#include <cstdlib>

namespace zoom = ZOOM_SDK_NAMESPACE;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->initZoomBtn, &QPushButton::clicked, this, &MainWindow::initZoomSDK);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initZoomSDK()
{
    // std::cout << "Hello from initZoomSDK" << std::endl;

    zoom::InitParam initParam;
    std::memset(&initParam, 0, sizeof(zoom::InitParam));

    initParam.strWebDomain = L"https://zoom.us";
    initParam.strSupportUrl = L"https://zoom.us";
    initParam.emLanguageID = ZOOM_SDK_NAMESPACE::LANGUAGE_English;
    initParam.enableLogByDefault = true;

    zoom::SDKError err = zoom::InitSDK(initParam);
    if (err == zoom::SDKERR_SUCCESS)
    {
        std::cout << "init succeeded" << std::endl;
    }
    else
    {
        std::cerr << "init failed" << std::endl;
    }
}
