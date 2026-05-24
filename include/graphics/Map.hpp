#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace Engine::Graphics {

    /**
     * @brief Tipi di tile supportati dal motore.
     */
    enum class TileType {
        Empty = 0,
        WallRed = 1,
        WallGreen = 2,
        WallBlue = 3,
        WallStone = 4, // Tile con texture
        Door = 10,
        StairsUp = 20,
        StairsDown = 21
    };

    /**
     * @brief Gestisce la mappa del pianeta con supporto a tipi di tile avanzati.
     */
    class Map {
    public:
        Map(int width, int height, int depth = 1);

        void setTile(int x, int y, int z, int value, int textureId = 1);
        int getTile(int x, int y, int z) const;
        TileType getTileType(int x, int y, int z) const;
        int getTextureId(int x, int y, int z) const;

        // Metadati del pianeta
        std::string planetName = "Pianeta Ignoto";
        float atmosphereColor[3] = {0.1f, 0.0f, 0.0f};

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }
        int getDepth() const { return m_depth; }

        void generateDummy();

    private:
        int m_width;
        int m_height;
        int m_depth;
        std::vector<int> m_data;
        std::vector<int> m_textureIds;
    };

}
