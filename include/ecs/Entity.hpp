#pragma once
#include <string>
#include <vector>

namespace Engine::ECS {

    enum class EntityType {
        StaticObject,
        Monster,
        Secret,
        Item
    };

    /**
     * @brief Entità del gioco (Pattern Prototype/Factory).
     */
    struct Entity {
        float x, y, z;
        EntityType type;
        std::string textureName;
        bool active = true;
        std::string interactionText; 
    };

    class EntityManager {
    public:
        void addEntity(float x, float y, float z, EntityType type, const std::string& tex) {
            m_entities.push_back({x, y, z, type, tex, true, ""});
        }

        std::vector<Entity>& getEntities() { return m_entities; }

        void clear() { m_entities.clear(); }

    private:
        std::vector<Entity> m_entities;
    };

}
