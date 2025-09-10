#include "Application.hpp"
#include <platform/Window.hpp>
#include <core/Logger.hpp>
#include <thread>
#include <stdexcept>

using clock_st = std::chrono::steady_clock;

namespace RV {
Application::Application() 
    : window_(std::make_unique<RV::Platform::Window>(1280, 720, "RV PS1"))
    , last_(clock_st::now()) {}


void Application::run() {
    using RV::Core::Log::Logger;
    using RV::Core::Log::Level;

    LOG_INFO("App::run() entered");
    while (window_->isOpen()) {
        window_->poll();

        auto now = clock_st::now();
        std::chrono::duration<float> dt = now - last_;
        last_ = now;

        update(dt.count());
        render();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    LOG_INFO("App::run() exiting");
}

void Application::update(float dt) {
    // TODO: input/camera/animazioni
}

void Application::render() {
    // TODO: chiamiamo vulkan
}

}
int main(int argc, char** argv) {
    RV::Application app;
    try {
        LOG_INFO("Starting application...");
        app.run();
        LOG_INFO("Closing gracefully...");
    } catch (const std::exception& e) {
        LOG_FATAL("Unhandled exception: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
    
}