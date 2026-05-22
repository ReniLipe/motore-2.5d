#include "core/WindowAdapter.hpp"
#include <iostream>

namespace Engine::Core {

    WindowAdapter::WindowAdapter(const std::string& title, int width, int height)
        : m_title(title), m_width(width), m_height(height) {}

    WindowAdapter::~WindowAdapter() {
        cleanup();
    }

    bool WindowAdapter::init() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            return false;
        }

        m_window = SDL_CreateWindow(
            m_title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_width,
            m_height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if (!m_window) return false;

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        // Attiva la modalità mouse relativo per FPS
        SDL_SetRelativeMouseMode(SDL_TRUE);

        return m_renderer != nullptr;
    }


    void WindowAdapter::cleanup() {
        if (m_renderer) SDL_DestroyRenderer(m_renderer);
        if (m_window) SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void WindowAdapter::clear(uint8_t r, uint8_t g, uint8_t b) {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
        SDL_RenderClear(m_renderer);
    }

    void WindowAdapter::present() {
        SDL_RenderPresent(m_renderer);
    }

}
