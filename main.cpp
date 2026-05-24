#include "core/Engine.hpp"
#include "states/PlayState.hpp"
#include <iostream>

/**
 * @file main.cpp
 * @brief Punto di ingresso principale del Motore Grafico Horror 2.5D.
 * 
 * Il motore segue un'architettura modulare:
 * - EngineInstance: Coordina i sistemi core (Rendering, Input, Stati).
 * - StateManager: Gestisce il ciclo di vita delle diverse fasi di gioco.
 * - Renderer: Astrae le chiamate grafiche per la modularità.
 */
int main(int argc, char* argv[]) {
    using namespace Engine::Core;
    using namespace Engine::States;

    // 1. Inizializzazione dell'istanza del motore
    EngineInstance engine("Horror 2.5D Engine - Master Tool", 1280, 720);
    
    if (!engine.init()) {
        std::cerr << "Errore critico: Impossibile inizializzare il motore." << std::endl;
        return -1;
    }

    // 2. Creazione e attivazione dello stato iniziale (PlayState)
    // Usiamo unique_ptr per una gestione sicura della memoria (RAII).
    auto playState = std::make_unique<PlayState>(engine);
    engine.getStates().pushState(std::move(playState));

    // 3. Avvio del Game Loop principale
    // Il controllo passa al motore, che gestirà update e render in modo modulare.
    std::cout << "Motore avviato correttamente. Benvenuto nel vuoto." << std::endl;
    engine.run();

    return 0;
}
