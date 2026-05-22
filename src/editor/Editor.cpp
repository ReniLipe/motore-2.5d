#include "editor/Editor.hpp"
#include "core/LocalizationManager.hpp"
#include "core/AssetManager.hpp"

namespace Engine::Editor {

    Editor::Editor(Core::WindowAdapter& window, Graphics::Map& map, ECS::EntityManager& entities) 
        : m_window(window), m_map(map), m_entities(entities) {}

    Editor::~Editor() {
        cleanup();
    }

    void Editor::init() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForSDLRenderer(m_window.getWindow(), m_window.getRenderer());
        ImGui_ImplSDLRenderer2_Init(m_window.getRenderer());
    }

    void Editor::handleEvent(SDL_Event& event) {
        ImGui_ImplSDL2_ProcessEvent(&event);
    }

    void Editor::render() {
        auto& loc = Core::LocalizationManager::getInstance();
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu(LANG_GET("menu_file").c_str())) {
                if (ImGui::MenuItem(LANG_GET("menu_save").c_str())) {
                    // Qui implementeremo la serializzazione JSON
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(LANG_GET("lang_switch").c_str())) {
                if (ImGui::MenuItem("English")) { loc.setLanguage(Core::Language::English); }
                if (ImGui::MenuItem("Italiano")) { loc.setLanguage(Core::Language::Italian); }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // --- PANNELLO PROGETTAZIONE ---
        if (m_showMapEditor) {
            // Rimosso il flag di chiusura (NULL invece di &m_showMapEditor) per evitare che sparisca cliccando la X
            ImGui::Begin(LANG_GET("editor_title").c_str(), NULL, ImGuiWindowFlags_MenuBar);
            
            ImGui::InputText(LANG_GET("planet_name").c_str(), m_planetName, 64);
            m_map.planetName = m_planetName;

            ImGui::ColorEdit3(LANG_GET("atmo_color").c_str(), m_atmoColor);
            m_map.atmosphereColor[0] = m_atmoColor[0];
            m_map.atmosphereColor[1] = m_atmoColor[1];
            m_map.atmosphereColor[2] = m_atmoColor[2];
            
            ImGui::Separator();
            ImGui::Text("%s", LANG_GET("map_edit_tools").c_str());
            ImGui::RadioButton(LANG_GET("tool_wall").c_str(), &m_selectedTool, 1); ImGui::SameLine();
            ImGui::RadioButton(LANG_GET("tool_empty").c_str(), &m_selectedTool, 0);

            // Griglia Mappa
            ImGui::BeginChild("MapGrid", ImVec2(0, 300), true);
            for (int y = 0; y < m_map.getHeight(); y++) {
                for (int x = 0; x < m_map.getWidth(); x++) {
                    int tile = m_map.getTile(x, y);
                    ImGui::PushID(y * m_map.getWidth() + x);
                    ImVec4 color = (tile > 0) ? ImVec4(0.6f, 0.1f, 0.1f, 1.0f) : ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
                    if (ImGui::ColorButton("##tile", color, 0, ImVec2(15, 15))) {
                        m_map.setTile(x, y, m_selectedTool);
                    }
                    if (x < m_map.getWidth() - 1) ImGui::SameLine();
                    ImGui::PopID();
                }
            }
            ImGui::EndChild();

            ImGui::Separator();
            if (ImGui::Button(LANG_GET("add_entity").c_str())) {
                m_entities.addEntity(12.0f, 12.0f, ECS::EntityType::Monster, "monster_tex");
            }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    }

    void Editor::cleanup() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

}
