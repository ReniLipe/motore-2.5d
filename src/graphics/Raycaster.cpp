#include "graphics/Raycaster.hpp"

namespace Engine::Graphics {

    Raycaster::Raycaster(Core::WindowAdapter& window) : m_window(window) {}

    void Raycaster::render(const Map& map, const Player& player) {
        SDL_Renderer* renderer = m_window.getRenderer();
        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);

        for (int x = 0; x < w; x++) {
            // Calcola posizione e direzione del raggio
            float cameraX = 2 * x / (float)w - 1;
            float rayDirX = player.dirX + player.planeX * cameraX;
            float rayDirY = player.dirY + player.planeY * cameraX;

            // In quale tile ci troviamo
            int mapX = int(player.x);
            int mapY = int(player.y);

            // Lunghezza del raggio dal lato attuale al prossimo lato X o Y
            float sideDistX, sideDistY;

            // Lunghezza del raggio da un lato X o Y al prossimo lato X o Y
            float deltaDistX = std::abs(1 / rayDirX);
            float deltaDistY = std::abs(1 / rayDirY);
            float perpWallDist;

            // In che direzione andare (step)
            int stepX, stepY;
            int hit = 0; 
            int side;

            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (player.x - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0f - player.x) * deltaDistX;
            }
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (player.y - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0f - player.y) * deltaDistY;
            }

            // DDA (Digital Differential Analyzer)
            while (hit == 0) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                if (map.getTile(mapX, mapY) > 0) hit = 1;
            }

            // Calcola distanza proiettata sulla direzione della camera
            if (side == 0) perpWallDist = (sideDistX - deltaDistX);
            else          perpWallDist = (sideDistY - deltaDistY);

            // Prevenzione divisione per zero
            if (perpWallDist <= 0.0001f) perpWallDist = 0.0001f;

            // Altezza della linea da disegnare sullo schermo
            int lineHeight = (int)(h / perpWallDist);

            // Calcola il pixel di inizio e fine
            int drawStart = -lineHeight / 2 + h / 2;
            if (drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + h / 2;
            if (drawEnd >= h) drawEnd = h - 1;

            // Colore del muro (Più scuro se colpisce lato Y per effetto ombra)
            uint8_t r = 150, g = 0, b = 0; // Rosso sangue horror
            if (side == 1) { r /= 2; }

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);

            // --- Rendering Pavimento e Soffitto (Stile 2.5D) ---
            // Soffitto (Grigio scuro/Atmosfera)
            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_RenderDrawLine(renderer, x, 0, x, drawStart);

            // Pavimento (Grigio ancora più scuro o texture in futuro)
            SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            SDL_RenderDrawLine(renderer, x, drawEnd, x, h);
        }
    }

}
