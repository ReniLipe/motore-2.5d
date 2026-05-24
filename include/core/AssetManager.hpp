#pragma once
#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cstring>

namespace Engine::Core {

    /**
     * @brief Gestore degli Asset (Pattern Service Locator).
     * Carica e memorizza texture, suoni, ecc.
     */
    class AssetManager {
    public:
        struct TextureData {
            SDL_Texture* texture;
            std::vector<uint32_t> pixels;
            int width, height;
        };

        static AssetManager& getInstance() {
            static AssetManager instance;
            return instance;
        }

        bool loadTexture(SDL_Renderer* renderer, const std::string& name, const std::string& path) {
            SDL_Surface* surface = SDL_LoadBMP(path.c_str());
            if (!surface) {
                return createPlaceholderTexture(renderer, name);
            }
            
            // Converti in formato RGBA8888 per coerenza
            SDL_Surface* optimized = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
            SDL_FreeSurface(surface);

            if (!optimized) return false;

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, optimized);
            
            TextureData data;
            data.texture = texture;
            data.width = optimized->w;
            data.height = optimized->h;
            data.pixels.resize(data.width * data.height);
            memcpy(data.pixels.data(), optimized->pixels, data.width * data.height * sizeof(uint32_t));

            SDL_FreeSurface(optimized);
            
            if (texture) {
                m_textures[name] = data;
                return true;
            }
            return false;
        }

        bool createPlaceholderTexture(SDL_Renderer* renderer, const std::string& name) {
            int size = 64;
            TextureData data;
            data.width = size;
            data.height = size;
            data.pixels.resize(size * size);

            for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                    data.pixels[y * size + x] = ((x / 8 + y / 8) % 2 == 0) ? 0xFFFFFFFF : 0xFF0000FF;
                }
            }

            data.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, size, size);
            SDL_UpdateTexture(data.texture, NULL, data.pixels.data(), size * sizeof(uint32_t));
            
            m_textures[name] = data;
            return true;
        }

        TextureData* getTextureData(const std::string& name) {
            if (m_textures.find(name) != m_textures.end()) {
                return &m_textures[name];
            }
            return nullptr;
        }

        void cleanup() {
            for (auto& pair : m_textures) {
                SDL_DestroyTexture(pair.second.texture);
            }
            m_textures.clear();
        }

    private:
        AssetManager() = default;
        std::unordered_map<std::string, TextureData> m_textures;
    };

}
