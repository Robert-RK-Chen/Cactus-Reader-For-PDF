#pragma once

#include <list>
#include "include/cef_client.h"

using BrowserList = std::list<CefRefPtr<CefBrowser>>;

class SimpleHandler : public CefClient, public CefDisplayHandler, public CefLifeSpanHandler, public CefLoadHandler
{
public:
    explicit SimpleHandler();
    ~SimpleHandler() override;

    // Provide access to the single global instance of this object
    static SimpleHandler* GetInstance();
    CefRefPtr<CefBrowser> GetBrowser();

    // CefClient methods
    CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

    // CefDisplayHandler methods
    void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
    HWND GetBrowserWindowHandler();

    // CefLifeSpanHandler methods
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
    bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame,
                       int popup_id,
                       const CefString& target_url,
                       const CefString& target_frame_name,
                       WindowOpenDisposition target_disposition,
                       bool user_gesture,
                       const CefPopupFeatures& popupFeatures,
                       CefWindowInfo& windowInfo,
                       CefRefPtr<CefClient>& client,
                       CefBrowserSettings& settings,
                       CefRefPtr<CefDictionaryValue>& extra_info,
                       bool* no_javascript_access) override;

    // CefLoadHandler methods
    void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                     const CefString& errorText, const CefString& failedUrl) override;

    // Request that all existing browser windows close.
    void CloseAllBrowsers(bool force_close);
    bool IsClosing() const { return is_closing; }

private:
    bool is_closing = false;

    // List of existing browser windows. Only accessed on the CEF UI thread
    BrowserList browser_list;

    // Include the default reference counting implementation
    IMPLEMENT_REFCOUNTING(SimpleHandler);
};