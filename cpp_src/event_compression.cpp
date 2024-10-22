#include "include/compression.h"
#include <cstdlib>

#include "include/compression.h"
#include <iostream>
#include <cstdlib> // Para a função system()

// Função para compactar arquivos
void compress_backup(const std::string& backup_path) {
    std::string command = "tar -czf " + backup_path + ".tar.gz " + backup_path;
    int result = system(command.c_str());
    if (result == 0) {
        std::cout << "Backup successfully compressed to " << backup_path << ".tar.gz" << std::endl;
    } else {
        std::cerr << "Error occurred while compressing the backup." << std::endl;
    }
}

// Função para perguntar se o usuário quer compressão
bool ask_user_about_compression() {
    std::cout << "Do you want to create a compressed version of the backup? (y/n): ";
    char compress_choice;
    std::cin >> compress_choice;
    return compress_choice == 'y' || compress_choice == 'Y';
}

