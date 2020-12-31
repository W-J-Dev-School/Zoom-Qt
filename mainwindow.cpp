#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cstdlib>
#include <iostream>
#include <QPushButton>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <customized_ui/zoom_customized_ui.h>

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
    zoom::SDKError err;

    //
    // Config
    //

    QFile configFile;
    configFile.setFileName("config.json");
    configFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString configText = configFile.readAll();
    configFile.close();

    QJsonDocument configJSON = QJsonDocument::fromJson(configText.toUtf8());
    QJsonObject config = configJSON.object();

    this->appKey = config["app_key"].toString().toStdWString();
    this->appSecret = config["app_secret"].toString().toStdWString();
    this->userEmail = config["user_email"].toString().toStdWString();
    this->userPassword = config["user_password"].toString().toStdWString();

    //
    // Init SDK
    //

    zoom::InitParam initParam;
    initParam.strWebDomain = L"https://zoom.us";
    initParam.strSupportUrl = L"https://zoom.us";
    initParam.emLanguageID = ZOOM_SDK_NAMESPACE::LANGUAGE_English;
    initParam.enableLogByDefault = true;
//    initParam.obConfigOpts.optionalFeatures = (1 << 5); // Use Custom UI

    err = zoom::InitSDK(initParam);
    if (err == zoom::SDKERR_SUCCESS)
    {
        std::cout << "init succeeded" << std::endl;
    }
    else
    {
        std::cerr << "init failed" << std::endl;
        return;
    }

//    //
//    // Create UI
//    //

//    zoom::ICustomizedUIMgr *ui_mgr= NULL;
//    err = zoom::CreateCustomizedUIMgr(&ui_mgr);
//    if (err == zoom::SDKERR_SUCCESS)
//    {
//        std::cout << "ui manager succeeded" << std::endl;
//    }
//    else
//    {
//        std::cerr << "ui manager failed" << std::endl;
//        return;
//    }

//    ui_mgr->SetEvent(this);

    //
    // Auth
    //

    err = zoom::CreateAuthService(&this->authService);
    if (err == zoom::SDKERR_SUCCESS)
    {
        std::cout << "auth service succeeded" << std::endl;
    }
    else
    {
        std::cerr << "auth service failed" << std::endl;
        return;
    }

    this->authService->SetEvent(this);

    zoom::AuthParam authParam;
    authParam.appKey = appKey.c_str();
    authParam.appSecret = appSecret.c_str();

    err = this->authService->SDKAuth(authParam);
    if (err == zoom::SDKERR_SUCCESS)
    {
        std::cout << "auth succeeded" << std::endl;
    }
    else
    {
        std::cerr << "auth failed" << std::endl;
        return;
    }
}

void MainWindow::onAuthenticationReturn(zoom::AuthResult ret)
{
    switch (ret)
    {
    case zoom::AUTHRET_SUCCESS:
        std::cout << "auth result: success" << std::endl;
        this->loginUser();
        break;
    case zoom::AUTHRET_KEYORSECRETWRONG:
        std::cout << "auth result: key or secret wrong" << std::endl;
        break;
    default:
        std::cout << "auth result: " << ret << std::endl;
        break;
    }
}

void MainWindow::onLoginRet(zoom::LOGINSTATUS ret, zoom::IAccountInfo *)
{
    switch (ret)
    {
    case zoom::LOGIN_IDLE:
        std::cout << "login status: idle" << std::endl;
        break;
    case zoom::LOGIN_PROCESSING:
        std::cout << "login status: processing" << std::endl;
        break;
    case zoom::LOGIN_SUCCESS:
        std::cout << "login status: success" << std::endl;
        //std::wcout << accountInfo->GetDisplayName() << std::endl;
        this->setupMeeting();
        break;
    case zoom::LOGIN_FAILED:
        std::cerr << "login status: failed" << std::endl;
        break;
    default:
        std::cout << "login result: " << ret << std::endl;
        break;
    }
}

void MainWindow::loginUser()
{
    zoom::SDKError err;

    //
    // User Login
    //

    zoom::LoginParam loginParam;
    loginParam.loginType = zoom::LoginType::LoginType_Email;
    loginParam.ut.emailLogin.userName = userEmail.c_str();
    loginParam.ut.emailLogin.password = userPassword.c_str();
    loginParam.ut.emailLogin.bRememberMe = false;

    err = this->authService->Login(loginParam);
    if (err == zoom::SDKERR_SUCCESS)
    {
        std::cout << "login succeeded" << std::endl;
    }
    else
    {
        std::cerr << "login failed: " << err << std::endl;
        return;
    }
}

void MainWindow::setupMeeting()
{
    zoom::SDKError err;

    //
    // Create Meeting Service
    //

    zoom::IMeetingService *meetingService = NULL;
    err = zoom::CreateMeetingService(&meetingService);
    if (err == zoom::SDKERR_SUCCESS)
    {
        std::cout << "meeting service succeeded" << std::endl;
    }
    else
    {
        std::cerr << "meeting service failed" << std::endl;
        return;
    }

    meetingService->SetEvent(this);

    //
    // Create Meeting
    //

    zoom::StartParam startParam;
    startParam.userType = zoom::SDK_UT_NORMALUSER;
    startParam.param.normaluserStart.meetingNumber = 6375532764;

    err = meetingService->Start(startParam);
    if (err == zoom::SDKERR_SUCCESS)
    {
        std::cout << "meeting succeeded" << std::endl;
    }
    else
    {
        std::cerr << "meeting failed: " << err << std::endl;
        return;
    }


//    //
//    // Join Meeting
//    //

//    zoom::JoinParam joinParam;
//    joinParam.userType = zoom::SDK_UT_NORMALUSER;
//    joinParam.param.normaluserJoin.meetingNumber = 6375532764;
//    joinParam.param.normaluserJoin.userName = L"App";
//    joinParam.param.normaluserJoin.psw = L"5Nhdbj";

//    err = meetingService->Join(joinParam);
//    if (err == zoom::SDKERR_SUCCESS)
//    {
//        std::cout << "meeting succeeded" << std::endl;
//    }
//    else
//    {
//        std::cerr << "meeting failed: " << err << std::endl;
//        return;
//    }
}

void onMeetingStatusChanged(zoom::MeetingStatus status, int)
{
    std::cout << "meeting status: " << status << std::endl;
}
