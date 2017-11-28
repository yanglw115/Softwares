#include "StdAfx.h"

UINT_PTR HWUpdateApp::m_IDEventTimerTest = 0;

void callback_http_response(enumRequestType nType, string &strdata)
{
    cout << strdata;
}

void CALLBACK HWUpdateApp::AppTimerProc(
    _In_ HWND     hwnd,
    _In_ UINT     uMsg,
    _In_ UINT_PTR idEvent,
    _In_ DWORD    dwTime)
{
    if (idEvent == m_IDEventTimerTest) {
        LOG(INFO) << "Time up ......";
    }
}

HWUpdateApp::HWUpdateApp()
    : m_pLogSystem(nullptr)
{

}


HWUpdateApp::~HWUpdateApp()
{
}


void HWUpdateApp::init()
{
    m_pLogSystem = HWSingleTon<HWLog>::getInstance();
    LOG(INFO) << "test....";
    LOG(WARNING) << "test....";

    HWHttpClient::init_curl_lib(); // 只在主线程初始化一次即可

    std::unique_ptr<HWHttpClient> client(new HWHttpClient);
    client.get()->registerHttpResponseCallback(callback_http_response);
    client.get()->send_request(Type_Request_Unknown, string(), Method_Request_Get);

    m_IDEventTimerTest = SetTimer(NULL, 0, 3 * 1000, this->AppTimerProc);
}
