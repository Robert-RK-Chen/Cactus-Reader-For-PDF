#include "SimpleHandler.h"

#include <sstream>
#include <string>

#include "include/base/cef_callback.h"
#include "include/cef_app.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace
{
    SimpleHandler* g_instance = nullptr;
    std::string GetDataURI(const std::string& data, const std::string& mime_type)
    {
        return "data:" + mime_type + ";base64," + CefURIEncode(CefBase64Encode(data.data(), data.size()), false).ToString();
    }
}

SimpleHandler::SimpleHandler()
{
    DCHECK(!g_instance);
    g_instance = this;
}

SimpleHandler::~SimpleHandler()
{
    g_instance = nullptr;
}

SimpleHandler* SimpleHandler::GetInstance()
{
    return g_instance;
}

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
    CEF_REQUIRE_UI_THREAD();

    if (auto browser_view = CefBrowserView::GetForBrowser(browser))
    {
        CefRefPtr<CefWindow> window = browser_view->GetWindow();
        if (window)
        {
            window->SetTitle(title);
        }
    }
}

HWND SimpleHandler::GetBrowserWindowHandler()
{
    if (!browser_list.empty())
    {
        return browser_list.front()->GetHost()->GetWindowHandle();
    }
    return NULL;
}

CefRefPtr<CefBrowser> SimpleHandler::GetBrowser()
{
    if (!browser_list.empty())
    {
        return browser_list.front();
    }
    return nullptr;
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    browser_list.push_back(browser);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    if (browser_list.size() == 1)
    {
        is_closing = true;
    }
    return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    for (BrowserList::iterator bit = browser_list.begin(); bit != browser_list.end(); ++bit)
    {
        if ((*bit)->IsSame(browser))
        {
            browser_list.erase(bit);
            break;
        }
    }
    if (browser_list.empty())
    {
        CefQuitMessageLoop();
    }
}

bool SimpleHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  int popup_id,
                                  const CefString& target_url,
                                  const CefString& target_frame_name,
                                  CefLifeSpanHandler::WindowOpenDisposition target_disposition,
                                  bool user_gesture,
                                  const CefPopupFeatures& popupFeatures,
                                  CefWindowInfo& windowInfo,
                                  CefRefPtr<CefClient>& client,
                                  CefBrowserSettings& settings,
                                  CefRefPtr<CefDictionaryValue>& extra_info,
                                  bool* no_javascript_access)
{
    browser->GetMainFrame()->LoadURL(target_url);
    return true;
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                                const CefString& errorText, const CefString& failedUrl)
{
    CEF_REQUIRE_UI_THREAD();

    if (errorCode == ERR_ABORTED)
    {
        return;
    }
    std::stringstream ss;
    ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL "
        << std::string(failedUrl) << " with error " << std::string(errorText)
        << " (" << errorCode << ").</h2></body></html>";

    frame->LoadURL(GetDataURI(ss.str(), "text/html"));
}

void SimpleHandler::CloseAllBrowsers(bool force_close)
{
    if (!CefCurrentlyOn(TID_UI))
    {
        CefPostTask(TID_UI, base::BindOnce(&SimpleHandler::CloseAllBrowsers, this, force_close));
        return;
    }
    if (browser_list.empty())
    {
        return;
    }

    for (BrowserList::const_iterator it = browser_list.begin(); it != browser_list.end(); ++it)
    {
        (*it)->GetHost()->CloseBrowser(force_close);
    }
}