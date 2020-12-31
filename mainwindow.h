#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <windows.h>
#include <zoom_sdk.h>
#include <auth_service_interface.h>
#include <meeting_service_interface.h>
#include <customized_ui/customized_ui_mgr.h>
#include <customized_ui/customized_video_container.h>

namespace zoom = ZOOM_SDK_NAMESPACE;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow
        : public QMainWindow
        , public zoom::IAuthServiceEvent
        , public zoom::IMeetingServiceEvent
        , public zoom::ICustomizedUIMgrEvent
        , public zoom::ICustomizedVideoContainerEvent
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void loginUser();
    void setupMeeting();
    void setupVideo();

    Ui::MainWindow *ui;

    std::wstring appKey;
    std::wstring appSecret;
    std::wstring userEmail;
    std::wstring userPassword;

    zoom::IAuthService* authService;
    zoom::ICustomizedVideoContainer *videoContainer;

    // Overrides for zoom::IAuthServiceEvent
    void onAuthenticationReturn(zoom::AuthResult ret) override;
    void onLoginRet(zoom::LOGINSTATUS ret, zoom::IAccountInfo *accountInfo) override;
    void onLogout() override {}
    void onZoomIdentityExpired() override {}
    void onZoomAuthIdentityExpired() override {}

    // Overrides for zoom::IMeetingServiceEvent
    void onMeetingStatusChanged(zoom::MeetingStatus status, int result) override;
    void onMeetingStatisticsWarningNotification(zoom::StatisticsWarningType) override {}
    void onMeetingSecureKeyNotification(const char *, int, zoom::IMeetingExternalSecureKeyHandler *) override {}
    void onMeetingParameterNotification(const zoom::MeetingParameter *) override {}

    // Overrides for zoom::ICustomizedUIMgrEvent
    void onVideoContainerDestroyed(zoom::ICustomizedVideoContainer *) override {};
    void onShareRenderDestroyed(zoom::ICustomizedShareRender *) override {};

    // Overrides for zoom::ICustomizedVideoContainerEvent
    void onRenderUserChanged(zoom::IVideoRenderElement *, unsigned int) override {};
    void onRenderDataTypeChanged(zoom::IVideoRenderElement *, zoom::VideoRenderDataType) override {};
    void onLayoutNotification(RECT) override {};
    void onVideoRenderElementDestroyed(zoom::IVideoRenderElement *) override {};
    void onWindowMsgNotification(UINT, WPARAM, LPARAM) override {};
    void onSubscribeUserFail(int, zoom::IVideoRenderElement*) override {};

public slots:
    void initZoomSDK();
};

#endif // MAINWINDOW_H
