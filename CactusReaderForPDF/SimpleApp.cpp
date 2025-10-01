#include "SimpleApp.h"
#include "include/wrapper/cef_helpers.h"

SimpleApp::SimpleApp() = default;

void SimpleApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();
}

void SimpleApp::OnBeforeCommandLineProcessing(
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line)
{

    // 解决CORS和文件访问问题的命令行参数
    command_line->AppendSwitch("allow-file-access-from-files");
    command_line->AppendSwitch("disable-web-security");
    command_line->AppendSwitch("allow-universal-access-from-files");
    command_line->AppendSwitch("disable-site-isolation-trials");
    command_line->AppendSwitch("disable-features=OutOfBlinkCors");
}