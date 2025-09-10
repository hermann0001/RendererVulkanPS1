#pragma once
#include <chrono>
#include <core/Logger.hpp>

namespace RV::Platform { class Window; }
namespace RV {

    class Application {
    public:
        Application();
        ~Application() = default;
    
        void run();

    private:
        void update(float dt);
        void render();
    
    private:
        std::unique_ptr<Platform::Window> window_;
        std::chrono::steady_clock::time_point last_;
    
    };
}