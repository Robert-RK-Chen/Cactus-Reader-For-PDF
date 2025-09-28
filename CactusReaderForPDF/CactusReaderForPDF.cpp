#include "CactusReaderForPDF.h"
#include "SimpleHandler.h"

CactusReaderForPDF::CactusReaderForPDF(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    CefRefPtr<SimpleHandler> handler(new SimpleHandler());
    CefBrowserSettings browser_settings;
    CefWindowInfo window_info;
    std::string url = "https://www4.bing.com";

    HWND wnd = (HWND)this->winId();
    QRect rect = this->rect();
    CefRect winRect(rect.x(), rect.y(), rect.width(), rect.height());
    window_info.SetAsChild(wnd, winRect);

    CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, nullptr, nullptr);
}

void CactusReaderForPDF::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (SimpleHandler::GetInstance())
    {
        CefRefPtr<CefBrowser> browser = SimpleHandler::GetInstance()->GetBrowser();
        if (browser.get())
        {
            HWND wnd = browser->GetHost()->GetWindowHandle();
            if (wnd)
            {
                QRect rect = this->rect();
                ::MoveWindow(wnd, rect.x(), rect.y(), rect.width() * devicePixelRatio(), rect.height() * devicePixelRatio(), true);
            }
        }
    }
}