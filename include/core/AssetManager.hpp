#pragma once
#include <SDL.h>
#include <string>
#include <unordered_map>
#include <iostream>

namespace Engine::Core {

    /**
     * @brief Gestore degli Asset (Pattern Service Locator).
     * Carica e memorizza texture, suoni, ecc.
     */
    class AssetManager {
    public:
        static AssetManager& getInstance() {
            static AssetManager instance;
            return instance;
        }

        bool loadTexture(SDL_Renderer* renderer, const std::string& name, const std::string& path) {
            SDL_Surface* surface = SDL_LoadBMP(path.c_str()); // Per ora BMP per semplicità, espandibile a PNG
            if (!surface) {
                std::cerr << "Errore caricamento asset: " << SDL_GetError() << std::endl;
                return false;
            }
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            
            if (texture) {
                m_textures[name] = texture;
                return true;
            }
            return false;
        }

        SDL_Texture* getTexture(const std::string& name) {
            if (m_textures.find(name) != m_textures.end()) {
                return m_textures[name];
            }
            return nullptr;
        }

        void cleanup() {
            for (auto& pair : m_textures) {
                SDL_DestroyTexture(pair.second);
            }
            m_textures.clear();
        }

    private:
        AssetManager() = default;
        std::unordered_map<std::string, SDL_Texture*> m_textures;
    };

}
