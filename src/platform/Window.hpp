#pragma once
#include <string>

// forward declaration;
struct GLFWwindow; 

namespace RV::Platform {
class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool isOpen() const;
    void poll();
    GLFWwindow* handle() const { return window_; }
    void getFrameBufferSize(int& w, int& h) const;

private:
    static void ensureGlfwInit();
    static void onError(int code, const char* desc);

private:
    GLFWwindow* window_ = nullptr;
};
}