#pragma once
#include <SDL.h>
#include <string>

namespace Engine::Core {

    /**
     * @brief Adapter per l'interfaccia della finestra.
     * Segue il pattern Adapter per isolare SDL2 dal resto del motore.
     */
    class WindowAdapter {
    public:
        WindowAdapter(const std::string& title, int width, int height);
        ~WindowAdapter();

        bool init();
        void cleanup();
        
        SDL_Window* getWindow() const { return m_window; }
        SDL_Renderer* getRenderer() const { return m_renderer; }

        void clear(uint8_t r, uint8_t g, uint8_t b);
        void present();

    private:
        std::string m_title;
        int m_width;
        int m_height;
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
    };

}
