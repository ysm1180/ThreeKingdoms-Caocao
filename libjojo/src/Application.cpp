#include "Application.h"

namespace jojogame {
Application* Application::s_sharedApplication = nullptr;

Application* Application::getInstance()
{
    return s_sharedApplication;
}

Application::Application(HINSTANCE instance)
{
    _hInstance = instance;
    s_sharedApplication = this;
}

Application::~Application()
{
    s_sharedApplication = nullptr;
}

HINSTANCE Application::getHInstance()
{
    return _hInstance;
}

}
