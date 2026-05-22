#pragma once
#include "core/WindowAdapter.hpp"
#include "graphics/Map.hpp"
#include "ecs/Entity.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

namespace Engine::Editor {

    class Editor {
    public:
        Editor(Core::WindowAdapter& window, Graphics::Map& map, ECS::EntityManager& entities);
        ~Editor();

        void init();
        void handleEvent(SDL_Event& event);
        void render();
        void cleanup();

        bool isVisible() const { return m_showMapEditor; }
        void setVisible(bool visible) { m_showMapEditor = visible; }

    private:
        Core::WindowAdapter& m_window;
        Graphics::Map& m_map;
        ECS::EntityManager& m_entities;
        bool m_showMetrics = false;
        bool m_showMapEditor = true;
        
        char m_planetName[64] = "Pianeta X";
        float m_atmoColor[3] = {0.1f, 0.0f, 0.0f};
        int m_selectedTool = 1; // 1 = Muro, 0 = Vuoto
    };

}
