#pragma once
#include <string>
#include <unordered_map>
#include <memory>

namespace Engine::Core {

    enum class Language {
        English,
        Italian
    };

    /**
     * @brief Manager per la localizzazione (Singleton).
     * Permette di cambiare lingua a runtime.
     */
    class LocalizationManager {
    public:
        static LocalizationManager& getInstance() {
            static LocalizationManager instance;
            return instance;
        }

        void setLanguage(Language lang) {
            m_currentLang = lang;
            loadStrings();
        }

        std::string getString(const std::string& key) {
            if (m_strings.find(key) != m_strings.end()) {
                return m_strings[key];
            }
            return key; // Ritorna la chiave se non trova la traduzione
        }

    private:
        LocalizationManager() { setLanguage(Language::Italian); } // Italiano predefinito
        
        void loadStrings() {
            m_strings.clear();
            if (m_currentLang == Language::Italian) {
                m_strings["editor_title"] = "Progettista Planetario";
                m_strings["welcome_msg"] = "Configura il tuo pianeta horror e i suoi segreti.";
                m_strings["wall_height"] = "Altezza Muri";
                m_strings["add_entity"] = "Aggiungi Entità";
                m_strings["menu_file"] = "File";
                m_strings["menu_edit"] = "Modifica";
                m_strings["menu_save"] = "Salva Pianeta";
                m_strings["menu_exit"] = "Esci";
                m_strings["lang_switch"] = "Lingua";
                m_strings["planet_name"] = "Nome del Pianeta";
                m_strings["atmo_color"] = "Colore Atmosfera";
                m_strings["map_edit_tools"] = "Strumenti Mappa";
                m_strings["tool_wall"] = "Muro";
                m_strings["tool_empty"] = "Vuoto";
            } else {
                m_strings["editor_title"] = "Planet Designer";
                m_strings["welcome_msg"] = "Configure your horror planet and its secrets.";
                m_strings["wall_height"] = "Wall Height";
                m_strings["add_entity"] = "Add Entity";
                m_strings["menu_file"] = "File";
                m_strings["menu_edit"] = "Edit";
                m_strings["menu_save"] = "Save Planet";
                m_strings["menu_exit"] = "Exit";
                m_strings["lang_switch"] = "Language";
                m_strings["planet_name"] = "Planet Name";
                m_strings["atmo_color"] = "Atmosphere Color";
                m_strings["map_edit_tools"] = "Map Tools";
                m_strings["tool_wall"] = "Wall";
                m_strings["tool_empty"] = "Empty";
            }
        }

        Language m_currentLang;
        std::unordered_map<std::string, std::string> m_strings;
    };

    // Macro comoda per accedere alle stringhe
    #define LANG_GET(key) Engine::Core::LocalizationManager::getInstance().getString(key)

}
