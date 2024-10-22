#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <string>

// Função para compactar arquivos
void compress_backup(const std::string& backup_path);

// Função para perguntar se o usuário quer compressão
bool ask_user_about_compression();

#endif // COMPRESSION_H

