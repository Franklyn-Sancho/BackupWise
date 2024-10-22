#include "include/zenity_selector.h"
#include <iostream> // Para std::cerr
#include <array>   // Para std::array

std::string select_directory_with_zenity(const std::string& title) {
    // Comando Zenity para abrir um diálogo de seleção de diretórios
    std::string command = "zenity --file-selection --directory --title=\"" + title + "\"";
    std::array<char, 256> buffer; // Usando std::array para segurança
    std::string result;

    // Abrir o pipe para executar o comando
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error executing Zenity!\n";
        return result;
    }

    // Capturar o resultado da seleção do diretório
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    // Remover a quebra de linha do final da string
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    pclose(pipe);
    return result;
}
