#include <memory>

#include "GlWindow.h"

int main()
{
    std::unique_ptr<GlWindow> app = std::make_unique<GlWindow>();
    app->Show();
}