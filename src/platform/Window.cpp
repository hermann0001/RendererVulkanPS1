#include "Window.hpp"
#include <core/Logger.hpp>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstdio>

namespace RV::Platform {

static bool g_glfwInited = false;

void Window::onError(int code, const char* desc) {
    LOG_ERROR("GLFW error [{}]: {}", std::to_string(code), desc);
}

void Window::ensureGlfwInit() {
    if (!g_glfwInited) {
        glfwSetErrorCallback(&Window::onError);
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        g_glfwInited = true;
    }
}

Window::Window(int width, int height, const std::string& title) {
    ensureGlfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) throw std::runtime_error("Failed to create GLFW window");
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
}

bool Window::isOpen() const {
    return window_ && !glfwWindowShouldClose(window_);
}

void Window::poll() {
    glfwPollEvents();
}

void Window::getFrameBufferSize(int& w, int& h) const {
    glfwGetFramebufferSize(window_, &w, &h);
}
}