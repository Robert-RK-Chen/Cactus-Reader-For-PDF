#include "SimpleApp.h"
#include "include/wrapper/cef_helpers.h"

SimpleApp::SimpleApp() = default;

void SimpleApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();
}