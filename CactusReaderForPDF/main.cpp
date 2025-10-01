#include "CactusReaderForPDF.h"
#include "SimpleApp.h"
#include "include/cef_app.h"


int main(int argc, char *argv[])
{
    void* sandbox_info = nullptr;

    const HINSTANCE h_instance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));
    const CefMainArgs main_args(h_instance);

    const int exit_code = CefExecuteProcess(main_args, nullptr, sandbox_info);
    if (exit_code >= 0)
    {
        return exit_code;
    }

    CefSettings settings;
    settings.no_sandbox = true;
    settings.multi_threaded_message_loop = true;
    settings.no_sandbox = true;

    CefRefPtr<SimpleApp> app(new SimpleApp);
    CefInitialize(main_args, settings, app.get(), nullptr);

    QApplication a(argc, argv);
    CactusReaderForPDF window;
    window.show();

    a.exec();
    CefShutdown();
    return 0;
}