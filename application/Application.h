/*****************************************************************//**
 * \file   Application.h
 * \brief  暴露给外部main.cpp使用的接口
 * 这里需要对窗口、输入事件、渲染、音频等管理
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include "../../util/common/opengl.h"
#include "../../util/logger/log.h"

#define glApp App::getInstance()

class App
{
    using ResizeCallBack = void(*)(int width, int height);
    using KeyCallBack = void(*)(int key, int scancode, int action, int mods);
    using MouseCallBack = void(*)(int button, int action, double x, double y);
    using CursorCallBack = void(*)(double xpos, double ypos);
    using ScrollCallBack = void(*)(double offset);
private:
    App() {}
    int _width{ 0 };
    int _height{ 0 };
    SDL_Window* _window{ nullptr };
    SDL_GLContext _context{ nullptr };
    ResizeCallBack _ResizeCallBack{ nullptr }; // 窗口大小改变回调函数指针
    KeyCallBack _KeyCallBack{ nullptr }; // 键盘按键回调函数指针
    MouseCallBack _MouseCallBack{ nullptr }; // 鼠标按键回调函数指针
    CursorCallBack _CursorCallBack{ nullptr }; // 鼠标移动回调函数指针
    ScrollCallBack _ScrollCallBack{ nullptr }; // 鼠标滚轮回调函数指针

public:
    // 删除拷贝构造函数和赋值操作符，防止被复制
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    // 静态方法来获取单例对象
    static App& getInstance() {
        static App instance;
        return instance;
    }
    inline void setWindowSize(const int width, const int height) {
        _width = width;
        _height = height;
    }
    inline int getWidth() const { return _width; }
    inline int getHeight() const { return _height; }
    inline SDL_Window* getWindow() const { return _window; }
    inline SDL_GLContext getContext() const { return _context; }
    void init(const int& width = 800, const int& height = 600, const char* title = "OpenGL App")
    {
        _width = width;
        _height = height;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            LOG_WARN("Failed to initialize SDL: {}", SDL_GetError());
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        // 创建SDL窗口
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        _window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
        if (_window == nullptr)
        {
            LOG_WARN("Failed to create SDL window: {}", SDL_GetError());
            SDL_Quit();
            assert(false);
        }

        // 创建OpenGL上下文
        _context = SDL_GL_CreateContext(_window);
        if (_context == nullptr) {
            LOG_WARN("Failed to create OpenGL context: {}", SDL_GetError());
            SDL_DestroyWindow(_window);
            SDL_Quit();
            assert(false);
        }

        SDL_GL_MakeCurrent(_window, _context);
        SDL_GL_SetSwapInterval(0); // Enable vsync

        // 初始化GLAD
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            LOG_WARN("Failed to initialize GLAD");
            SDL_GL_DeleteContext(_context);
            SDL_DestroyWindow(_window);
            SDL_Quit();
            assert(false);
        }

        // 设置OpenGL视口
        glViewport(0, 0, width, height);
    }
    void handleEvent(SDL_Event& event)
    {
        // 处理键盘事件
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            keyCallBack(event.key.keysym.sym, event.key.keysym.scancode, event.type, 0);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) { // 处理鼠标按键事件
            mouseCallBack(event.button.button, event.type, event.button.x, event.button.y);
        }
        else if (event.type == SDL_MOUSEMOTION) { // 处理鼠标移动事件
            cursorCallBack(event.motion.x, event.motion.y);
        }
        else if (event.type == SDL_MOUSEWHEEL) { // 处理鼠标滚轮事件
            scrollCallBack(event.wheel.y);
        }
        else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            int new_width = event.window.data1;
            int new_height = event.window.data2;
            frameBufferSizeCallBack(new_width, new_height); // 窗口大小回调函数
        }

    }

    bool update()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event); // 处理ImGui事件，不写导致ImGui无法正常工作
            if (event.type == SDL_QUIT) {
                return false;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(_window)) {
                return false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                {   // 按下 ESC 键
                    return false;
                }
            }
            handleEvent(event);
        }

        SDL_GL_SwapWindow(_window);  // 交换缓冲区
        return true;
    }
    void destroy()
    {
        // SDL2
        SDL_GL_DeleteContext(_context);
        SDL_DestroyWindow(_window);
        SDL_Quit();
        // ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    inline void setResizeCallBack(ResizeCallBack callback) { _ResizeCallBack = callback; } // 设置窗口大小改变回调函数
    inline void setKeyCallBack(KeyCallBack callback) { _KeyCallBack = callback; } // 设置按键回调函数
    inline void setMouseCallBack(MouseCallBack callback) { _MouseCallBack = callback; } // 设置鼠标按键回调函数
    inline void setCursorCallBack(CursorCallBack callback) { _CursorCallBack = callback; } // 设置渲染回调函数
    inline void setScrollCallBack(ScrollCallBack callback) { _ScrollCallBack = callback; } // 设置鼠标滚轮回调函数

private:
    inline void frameBufferSizeCallBack(int width, int height)
    {
        assert(glApp._ResizeCallBack);
        glApp._ResizeCallBack(width, height);
    }
    inline void keyCallBack(int key, int scancode, int action, int mods)
    {
        assert(glApp._KeyCallBack);
        glApp._KeyCallBack(key, scancode, action, mods);
    }
    inline void mouseCallBack(int button, int action, double x, double y)
    {
        assert(glApp._MouseCallBack);
        glApp._MouseCallBack(button, action, x, y);
    }
    inline void cursorCallBack(double xpos, double ypos)
    {
        assert(glApp._CursorCallBack);
        glApp._CursorCallBack(xpos, ypos);
    }
    inline void scrollCallBack(double yoffset)
    {
        assert(glApp._ScrollCallBack);
        glApp._ScrollCallBack(yoffset);
    }
};

#endif // Application.h