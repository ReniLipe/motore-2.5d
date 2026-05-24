#include "graphics/Map.hpp"

namespace Engine::Graphics {

    Map::Map(int width, int height, int depth) : m_width(width), m_height(height), m_depth(depth) {
        m_data.resize(width * height * depth, 0);
        m_textureIds.resize(width * height * depth, 1);
    }

    void Map::setTile(int x, int y, int z, int value, int textureId) {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height && z >= 0 && z < m_depth) {
            int index = (z * m_width * m_height) + (y * m_width) + x;
            m_data[index] = value;
            m_textureIds[index] = textureId;
        }
    }

    int Map::getTile(int x, int y, int z) const {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height && z >= 0 && z < m_depth) {
            return m_data[(z * m_width * m_height) + (y * m_width) + x];
        }
        return (z == 0) ? 1 : 0; // Se fuori mappa al livello 0, metti muro. Altri livelli vuoto.
    }

    TileType Map::getTileType(int x, int y, int z) const {
        return static_cast<TileType>(getTile(x, y, z));
    }

    int Map::getTextureId(int x, int y, int z) const {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height && z >= 0 && z < m_depth) {
            return m_textureIds[(z * m_width * m_height) + (y * m_width) + x];
        }
        return 1;
    }

    void Map::generateDummy() {
        // Genera muri perimetrali per ogni livello
        for (int z = 0; z < m_depth; z++) {
            for (int i = 0; i < m_width; ++i) {
                setTile(i, 0, z, 1);
                setTile(i, m_height - 1, z, 1);
            }
            for (int i = 0; i < m_height; ++i) {
                setTile(0, i, z, 1);
                setTile(m_width - 1, i, z, 1);
            }
        }
        
        // Aggiungi qualche pilastro al livello 0
        setTile(5, 5, 0, 1);
        setTile(5, 6, 0, 1);
        
        // Aggiungi qualcosa al livello 1 se esiste
        if (m_depth > 1) {
            setTile(10, 10, 1, 1);
        }
    }

}
