#include <spdlog/spdlog.h>

#include "engine/core/GameApp.h"

using namespace engine::core;

int main(int argc, char* argv[])
{
    // spdlog::set_level(spdlog::level::trace);

    GameApp app;
    app.run();
    return 0;
}