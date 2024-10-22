#include "include/config.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Função para carregar configurações do arquivo JSON
Config load_config(const std::string& filename) {
    Config config;
    std::ifstream file(filename);
    if (file.is_open()) {
        json j;
        file >> j;
        config.watch_path = j["watch_path"].get<std::string>();
        config.backup_path = j["backup_path"].get<std::string>();
    }
    return config;
}

// Função para salvar configurações no arquivo JSON
void save_config(const std::string& filename, const Config& config) {
    json j;
    j["watch_path"] = config.watch_path;
    j["backup_path"] = config.backup_path;
    
    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4); // Escreve o JSON formatado
    }
}
