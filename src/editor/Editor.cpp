#include "editor/Editor.hpp"
#include "core/LocalizationManager.hpp"
#include "core/AssetManager.hpp"
#include "core/PerformanceMonitor.hpp"

namespace Engine::Editor {

    Editor::Editor(Core::WindowAdapter& window, Graphics::Renderer& renderer, Graphics::Map& map, ECS::EntityManager& entities) 
        : m_window(window), m_renderer(renderer), m_map(map), m_entities(entities) {}

    Editor::~Editor() {
        cleanup();
    }

    void Editor::init() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForSDLRenderer(m_window.getWindow(), m_renderer.getNativeRenderer());
        ImGui_ImplSDLRenderer2_Init(m_renderer.getNativeRenderer());
    }

    void Editor::handleEvent(SDL_Event& event) {
        ImGui_ImplSDL2_ProcessEvent(&event);
    }

    void Editor::render(Core::PerformanceMonitor& perf) {
        auto& loc = Core::LocalizationManager::getInstance();
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // --- BARRA DEI MENU SUPERIORE ---
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu(LANG_GET("menu_file").c_str())) {
                if (ImGui::MenuItem(LANG_GET("menu_exit").c_str())) { 
                    SDL_Event quitEv; quitEv.type = SDL_QUIT; SDL_PushEvent(&quitEv); 
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Mondo")) {
                ImGui::InputText("Nome Pianeta", m_planetName, 64);
                m_map.planetName = m_planetName;
                ImGui::ColorEdit3("Atmosfera", m_atmoColor);
                m_map.atmosphereColor[0] = m_atmoColor[0];
                m_map.atmosphereColor[1] = m_atmoColor[1];
                m_map.atmosphereColor[2] = m_atmoColor[2];
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Entità")) {
                if (ImGui::MenuItem("Aggiungi Mostro (Centro)")) { 
                    m_entities.addEntity(12.5f, 12.5f, (float)m_currentLevel, ECS::EntityType::Monster, "monster_tex"); 
                }
                if (ImGui::MenuItem("Aggiungi Item (Centro)")) { 
                    m_entities.addEntity(10.5f, 10.5f, (float)m_currentLevel, ECS::EntityType::Item, "item_tex"); 
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Svuota Tutto")) { m_entities.clear(); }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Strumenti Mappa")) {
                if (ImGui::MenuItem("Vuoto", "0", m_selectedTool == 0)) m_selectedTool = 0;
                if (ImGui::MenuItem("Muro Rosso", "1", m_selectedTool == 1)) m_selectedTool = 1;
                if (ImGui::MenuItem("Muro Verde", "2", m_selectedTool == 2)) m_selectedTool = 2;
                if (ImGui::MenuItem("Muro Blu", "3", m_selectedTool == 3)) m_selectedTool = 3;
                if (ImGui::MenuItem("Pietra", "4", m_selectedTool == 4)) m_selectedTool = 4;
                ImGui::Separator();
                if (ImGui::MenuItem("Porta", "10", m_selectedTool == 10)) m_selectedTool = 10;
                if (ImGui::MenuItem("Scala SU", "20", m_selectedTool == 20)) m_selectedTool = 20;
                if (ImGui::MenuItem("Scala GIU", "21", m_selectedTool == 21)) m_selectedTool = 21;
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Lingua")) {
                if (ImGui::MenuItem("English")) { loc.setLanguage(Core::Language::English); }
                if (ImGui::MenuItem("Italiano")) { loc.setLanguage(Core::Language::Italian); }
                ImGui::EndMenu();
            }

            // Statistiche veloci nella barra
            ImGui::SameLine(ImGui::GetWindowSize().x - 180);
            ImGui::Text("FPS: %.0f | RAM: %zu MB", perf.getFPS(), perf.getMemoryUsage());

            ImGui::EndMainMenuBar();
        }

        // --- PANNELLO PROGETTAZIONE MAPPA ---
        if (m_showMapEditor) {
            ImGui::Begin(LANG_GET("editor_title").c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);
            
            ImGui::Text("Strumenti Mappa (Piano: %d)", m_currentLevel);
            ImGui::RadioButton("Vuoto", &m_selectedTool, 0); ImGui::SameLine();
            ImGui::RadioButton("Muro R", &m_selectedTool, 1); ImGui::SameLine();
            ImGui::RadioButton("Muro V", &m_selectedTool, 2); ImGui::SameLine();
            ImGui::RadioButton("Muro B", &m_selectedTool, 3);
            
            ImGui::RadioButton("Pietra", &m_selectedTool, 4); ImGui::SameLine();
            ImGui::RadioButton("Porta", &m_selectedTool, 10); ImGui::SameLine();
            ImGui::RadioButton("Scala SU", &m_selectedTool, 20); ImGui::SameLine();
            ImGui::RadioButton("Scala GIU", &m_selectedTool, 21);
            
            ImGui::SliderInt("Seleziona Z", &m_currentLevel, 0, m_map.getDepth() - 1);

            ImGui::Separator();
            ImGui::Text("INFO: Click per Tile | Shift+Click per Mostro");

            // Griglia Mappa
            ImGui::BeginChild("MapGrid", ImVec2(400, 400), true);
            for (int y = 0; y < m_map.getHeight(); y++) {
                for (int x = 0; x < m_map.getWidth(); x++) {
                    int tile = m_map.getTile(x, y, m_currentLevel);
                    ImGui::PushID(y * m_map.getWidth() + x);
                    
                    bool hasEntity = false;
                    for (auto& e : m_entities.getEntities()) {
                        if (int(e.x) == x && int(e.y) == y && int(e.z) == m_currentLevel) hasEntity = true;
                    }

                    ImVec4 color = (tile > 0) ? ImVec4(0.6f, 0.1f, 0.1f, 1.0f) : ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
                    if (hasEntity) color = ImVec4(1.0f, 0.8f, 0.0f, 1.0f); // Oro per entità

                    if (ImGui::ColorButton("##tile", color, 0, ImVec2(14, 14))) {
                        if (ImGui::GetIO().KeyShift) {
                            m_entities.addEntity(x + 0.5f, y + 0.5f, (float)m_currentLevel, ECS::EntityType::Monster, "placeholder");
                        } else {
                            m_map.setTile(x, y, m_currentLevel, m_selectedTool);
                        }
                    }
                    if (x < m_map.getWidth() - 1) ImGui::SameLine();
                    ImGui::PopID();
                }
            }
            ImGui::EndChild();
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
