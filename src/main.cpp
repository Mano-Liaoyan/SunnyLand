#include <spdlog/spdlog.h>

#include "engine/core/GameApp.h"

using namespace engine::core;

int main(int argc, char* argv[])
{
    spdlog::set_level(spdlog::level::debug);

    GameApp app;
    app.run();
    return 0;
}