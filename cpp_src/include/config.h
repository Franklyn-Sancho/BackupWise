#ifndef CONFIG_H
#define CONFIG_H

#include <string>

// Estrutura para armazenar configurações
struct Config {
    std::string watch_path;
    std::string backup_path;
};

// Declaração das funções para carregar e salvar configurações
Config load_config(const std::string& filename);
void save_config(const std::string& filename, const Config& config);

#endif // CONFIG_H
