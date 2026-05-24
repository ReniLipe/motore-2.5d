#include "graphics/Raycaster.hpp"
#include "core/AssetManager.hpp"
#include <algorithm>

namespace Engine::Graphics {

    void Raycaster::render(Renderer& renderer, const Map& map, const Player& player, ECS::EntityManager& entities) {
        int w = renderer.getWidth();
        int h = renderer.getHeight();
        
        if (m_zBuffer.size() != (size_t)w) m_zBuffer.resize(w);

        auto& assetMgr = Core::AssetManager::getInstance();
        auto* wallTex = assetMgr.getTextureData("stone_wall");

        // --- 1. Rendering Muri ---
        for (int x = 0; x < w; x++) {
            float cameraX = 2 * x / (float)w - 1;
            float rayDirX = player.dirX + player.planeX * cameraX;
            float rayDirY = player.dirY + player.planeY * cameraX;

            int mapX = int(player.x);
            int mapY = int(player.y);

            float sideDistX, sideDistY;
            float deltaDistX = std::abs(1 / rayDirX);
            float deltaDistY = std::abs(1 / rayDirY);
            float perpWallDist;

            int stepX, stepY;
            int hit = 0; 
            int side;

            if (rayDirX < 0) { stepX = -1; sideDistX = (player.x - mapX) * deltaDistX; }
            else { stepX = 1; sideDistX = (mapX + 1.0f - player.x) * deltaDistX; }
            if (rayDirY < 0) { stepY = -1; sideDistY = (player.y - mapY) * deltaDistY; }
            else { stepY = 1; sideDistY = (mapY + 1.0f - player.y) * deltaDistY; }

            int mapZ = (int)player.z;
            while (hit == 0) {
                if (sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
                else { sideDistY += deltaDistY; mapY += stepY; side = 1; }
                if (map.getTile(mapX, mapY, mapZ) > 0) hit = 1;
            }

            if (side == 0) perpWallDist = (sideDistX - deltaDistX);
            else          perpWallDist = (sideDistY - deltaDistY);

            if (perpWallDist <= 0.0001f) perpWallDist = 0.0001f;
            m_zBuffer[x] = perpWallDist;

            int lineHeight = (int)(h / perpWallDist);
            float verticalShift = player.pitch + (0.5f - (player.z - mapZ)) * lineHeight;
            int drawStart = -lineHeight / 2 + h / 2 + (int)verticalShift;
            int drawEnd = lineHeight / 2 + h / 2 + (int)verticalShift;

            int actualDrawStart = drawStart; if (actualDrawStart < 0) actualDrawStart = 0;
            int actualDrawEnd = drawEnd; if (actualDrawEnd >= h) actualDrawEnd = h - 1;

            TileType type = map.getTileType(mapX, mapY, mapZ);
            bool useTexture = (type == TileType::WallStone) && wallTex;
            
            uint8_t r = 150, g = 0, b = 0;

            if (useTexture) {
                float wallX; 
                if (side == 0) wallX = player.y + perpWallDist * rayDirY;
                else          wallX = player.x + perpWallDist * rayDirX;
                wallX -= floor(wallX);

                int texX = int(wallX * (float)wallTex->width);
                if(side == 0 && rayDirX > 0) texX = wallTex->width - 1 - texX;
                if(side == 1 && rayDirY < 0) texX = wallTex->width - 1 - texX;

                for(int y_pix = actualDrawStart; y_pix < actualDrawEnd; y_pix++) {
                    int d = y_pix * 256 - h * 128 + lineHeight * 128 - (int)verticalShift * 256;
                    int texY = ((d * wallTex->height) / lineHeight) / 256;
                    if (texY < 0) texY = 0; if (texY >= wallTex->height) texY = wallTex->height - 1;
                    uint32_t color = wallTex->pixels[texY * wallTex->width + texX];
                    r = (color >> 24) & 0xFF; g = (color >> 16) & 0xFF; b = (color >> 8) & 0xFF;
                    if (side == 1) { r /= 2; g /= 2; b /= 2; }
                    renderer.drawPixel(x, y_pix, r, g, b);
                }
            } else {
                switch (type) {
                    case TileType::WallGreen: r = 0; g = 150; b = 0; break;
                    case TileType::WallBlue:  r = 0; g = 0; b = 150; break;
                    case TileType::Door:      r = 200; g = 150; b = 50; break; 
                    default: break; 
                }
                if (side == 1) { r /= 2; g /= 2; b /= 2; }
                renderer.drawVerticalLine(x, actualDrawStart, actualDrawEnd, r, g, b);
            }

            if (actualDrawStart > 0) renderer.drawVerticalLine(x, 0, actualDrawStart, 20, 20, 20);
            if (actualDrawEnd < h) renderer.drawVerticalLine(x, actualDrawEnd, h, 40, 40, 40);
        }

        // --- 2. Rendering Sprites (Entità) ---
        auto& entityList = entities.getEntities();
        if (entityList.empty()) return;

        std::vector<int> spriteOrder(entityList.size());
        std::vector<float> spriteDistance(entityList.size());
        for (int i = 0; i < (int)entityList.size(); i++) {
            spriteOrder[i] = i;
            spriteDistance[i] = ((player.x - entityList[i].x) * (player.x - entityList[i].x) + 
                                 (player.y - entityList[i].y) * (player.y - entityList[i].y)); 
        }
        
        for (int i = 0; i < (int)entityList.size() - 1; i++) {
            for (int j = i + 1; j < (int)entityList.size(); j++) {
                if (spriteDistance[i] < spriteDistance[j]) {
                    std::swap(spriteDistance[i], spriteDistance[j]);
                    std::swap(spriteOrder[i], spriteOrder[j]);
                }
            }
        }

        for (int i = 0; i < (int)entityList.size(); i++) {
            auto& sprite = entityList[spriteOrder[i]];
            if (!sprite.active) continue;
            if (std::abs(sprite.z - player.z) > 1.0f) continue;

            float spriteX = sprite.x - player.x;
            float spriteY = sprite.y - player.y;
            float invDet = 1.0f / (player.planeX * player.dirY - player.dirX * player.planeY);
            float transformX = invDet * (player.dirY * spriteX - player.dirX * spriteY);
            float transformY = invDet * (-player.planeY * spriteX + player.planeX * spriteY);

            if (transformY <= 0.1f) continue;

            int spriteScreenX = int((w / 2) * (1 + transformX / transformY));
            int vMoveScreen = int(player.pitch / transformY);
            int spriteHeight = std::abs(int(h / transformY));
            int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
            if (drawStartY < 0) drawStartY = 0;
            int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
            if (drawEndY >= h) drawEndY = h - 1;

            int spriteWidth = std::abs(int(h / transformY));
            int drawStartX = -spriteWidth / 2 + spriteScreenX;
            if (drawStartX < 0) drawStartX = 0;
            int drawEndX = spriteWidth / 2 + spriteScreenX;
            if (drawEndX >= w) drawEndX = w - 1;

            for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
                if (transformY < m_zBuffer[stripe]) {
                    uint8_t r = 255, g = 255, b = 0; 
                    if (sprite.type == ECS::EntityType::Monster) { r = 255; g = 0; b = 255; }
                    renderer.drawVerticalLine(stripe, drawStartY, drawEndY, r, g, b);
                }
            }
        }
    }

}
