#include "core/WindowAdapter.hpp"
#include "editor/Editor.hpp"
#include "graphics/Map.hpp"
#include "graphics/Raycaster.hpp"
#include "ecs/Entity.hpp"
#include "core/AssetManager.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

int main(int argc, char* argv[]) {
    using namespace Engine;

    Core::WindowAdapter window("Horror 2.5D Engine - Master Tool", 1280, 720);
    if (!window.init()) return -1;

    // Sistemi Core
    Graphics::Map map(24, 24);
    map.generateDummy();
    ECS::EntityManager entities;
    Graphics::Raycaster raycaster(window);
    Graphics::Player player;

    Editor::Editor editor(window, map, entities);
    editor.init();

    bool running = true;
    SDL_Event event;
    uint32_t lastTime = SDL_GetTicks();
    
    bool cursorCaptured = true;
    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;

    while (running) {
        uint32_t frameStart = SDL_GetTicks();
        uint32_t currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        
        // Cap deltaTime per evitare salti enormi se il PC rallenta
        deltaTime = std::min(deltaTime, 0.05f);
        lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            editor.handleEvent(event);
            if (event.type == SDL_QUIT) running = false;
            
            // Toggle Cattura Mouse (ESC)
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                cursorCaptured = !cursorCaptured;
                SDL_SetRelativeMouseMode(cursorCaptured ? SDL_TRUE : SDL_FALSE);
                // Se sblocchiamo il mouse, mostriamo l'editor se era nascosto
                if (!cursorCaptured) editor.setVisible(true);
            }

            // Mouse Look
            if (cursorCaptured && event.type == SDL_MOUSEMOTION) {
                float mouseX = (float)event.motion.xrel;
                float rotSpeed = mouseX * 0.002f;

                float oldDirX = player.dirX;
                player.dirX = player.dirX * std::cos(-rotSpeed) - player.dirY * std::sin(-rotSpeed);
                player.dirY = oldDirX * std::sin(-rotSpeed) + player.dirY * std::cos(-rotSpeed);
                
                float oldPlaneX = player.planeX;
                player.planeX = player.planeX * std::cos(-rotSpeed) - player.planeY * std::sin(-rotSpeed);
                player.planeY = oldPlaneX * std::sin(-rotSpeed) + player.planeY * std::cos(-rotSpeed);
            }
        }

        // Movimento (solo se il mouse è catturato, ovvero siamo in "Play Mode")
        if (cursorCaptured) {
            const uint8_t* state = SDL_GetKeyboardState(NULL);
            float moveSpeed = 4.0f * deltaTime; // Rallentato un pochino
            
            if (state[SDL_SCANCODE_W]) {
                if (map.getTile(int(player.x + player.dirX * moveSpeed), int(player.y)) == 0) player.x += player.dirX * moveSpeed;
                if (map.getTile(int(player.x), int(player.y + player.dirY * moveSpeed)) == 0) player.y += player.dirY * moveSpeed;
            }
            if (state[SDL_SCANCODE_S]) {
                if (map.getTile(int(player.x - player.dirX * moveSpeed), int(player.y)) == 0) player.x -= player.dirX * moveSpeed;
                if (map.getTile(int(player.x), int(player.y - player.dirY * moveSpeed)) == 0) player.y -= player.dirY * moveSpeed;
            }
            if (state[SDL_SCANCODE_A]) {
                float strafeDirX = -player.dirY;
                float strafeDirY = player.dirX;
                if (map.getTile(int(player.x + strafeDirX * moveSpeed), int(player.y)) == 0) player.x += strafeDirX * moveSpeed;
                if (map.getTile(int(player.x), int(player.y + strafeDirY * moveSpeed)) == 0) player.y += strafeDirY * moveSpeed;
            }
            if (state[SDL_SCANCODE_D]) {
                float strafeDirX = player.dirY;
                float strafeDirY = -player.dirX;
                if (map.getTile(int(player.x + strafeDirX * moveSpeed), int(player.y)) == 0) player.x += strafeDirX * moveSpeed;
                if (map.getTile(int(player.x), int(player.y + strafeDirY * moveSpeed)) == 0) player.y += strafeDirY * moveSpeed;
            }
        }

        // Rendering
        window.clear(
            (uint8_t)(map.atmosphereColor[0] * 255),
            (uint8_t)(map.atmosphereColor[1] * 255),
            (uint8_t)(map.atmosphereColor[2] * 255)
        );

        raycaster.render(map, player);
        
        // Render Editor (solo se mouse sbloccato o forzato)
        if (!cursorCaptured || editor.isVisible()) {
            editor.render();
        }

        window.present();

        // Limitatore FPS (Rallenta il loop se troppo veloce)
        uint32_t frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    Core::AssetManager::getInstance().cleanup();
    editor.cleanup();
    window.cleanup();
    return 0;
}
