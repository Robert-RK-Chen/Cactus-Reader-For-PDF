#pragma once

#include "include/cef_app.h"
#include "qobject.h"

class SimpleApp : public QObject, public CefApp, public CefBrowserProcessHandler
{
    Q_OBJECT

public:
    SimpleApp();

    void OnContextInitialized() override;
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

private:
    // Include the default reference counting implementation
    IMPLEMENT_REFCOUNTING(SimpleApp);
};