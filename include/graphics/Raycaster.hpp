#pragma once
#include "graphics/Renderer.hpp"
#include "graphics/Map.hpp"
#include "ecs/Entity.hpp"
#include <cmath>
#include <vector>

namespace Engine::Graphics {

    struct Player {
        float x = 12.0f;
        float y = 12.0f;
        float z = 0.5f; 
        float dirX = -1.0f;
        float dirY = 0.0f;
        float planeX = 0.0f;
        float planeY = 0.66f; 
        float pitch = 0.0f;   
    };

    /**
     * @brief Motore di Raycasting modulare con supporto sprite.
     */
    class Raycaster {
    public:
        void render(Renderer& renderer, const Map& map, const Player& player, ECS::EntityManager& entities);

    private:
        std::vector<float> m_zBuffer;
    };

}
