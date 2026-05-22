#include "graphics/Map.hpp"

namespace Engine::Graphics {

    Map::Map(int width, int height) : m_width(width), m_height(height) {
        m_data.resize(width * height, 0);
        m_textureIds.resize(width * height, 1);
    }

    void Map::setTile(int x, int y, int value, int textureId) {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            m_data[y * m_width + x] = value;
            m_textureIds[y * m_width + x] = textureId;
        }
    }

    int Map::getTile(int x, int y) const {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            return m_data[y * m_width + x];
        }
        return 1;
    }

    int Map::getTextureId(int x, int y) const {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            return m_textureIds[y * m_width + x];
        }
        return 1;
    }

    void Map::generateDummy() {
        for (int i = 0; i < m_width; ++i) {
            setTile(i, 0, 1);
            setTile(i, m_height - 1, 1);
        }
        for (int i = 0; i < m_height; ++i) {
            setTile(0, i, 1);
            setTile(m_width - 1, i, 1);
        }
    }

}
