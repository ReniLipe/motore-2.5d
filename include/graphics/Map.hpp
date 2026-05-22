#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace Engine::Graphics {

    /**
     * @brief Gestisce la mappa del pianeta.
     * Modularità: Può essere caricata da file o generata proceduralmente.
     */
    class Map {
    public:
        Map(int width, int height);
        
        void setTile(int x, int y, int value, int textureId = 1);
        int getTile(int x, int y) const;
        int getTextureId(int x, int y) const;

        // Metadati del pianeta
        std::string planetName = "Pianeta Ignoto";
        float atmosphereColor[3] = {0.1f, 0.0f, 0.0f};

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }

        void generateDummy();

    private:
        int m_width;
        int m_height;
        std::vector<int> m_data;
        std::vector<int> m_textureIds;
    };

}
