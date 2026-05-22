#pragma once
#include "core/WindowAdapter.hpp"
#include "graphics/Map.hpp"
#include <cmath>

namespace Engine::Graphics {

    struct Player {
        float x = 12.0f;
        float y = 12.0f;
        float dirX = -1.0f;
        float dirY = 0.0f;
        float planeX = 0.0f;
        float planeY = 0.66f; // FOV 66 gradi stile Doom
    };

    /**
     * @brief Motore di Raycasting per il rendering 2.5D.
     * Super ottimizzato per C++.
     */
    class Raycaster {
    public:
        Raycaster(Core::WindowAdapter& window);
        
        void render(const Map& map, const Player& player);

    private:
        Core::WindowAdapter& m_window;
    };

}
