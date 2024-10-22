#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "include/log.h"
#include "include/event_handler.h"
#include "include/event_checker.h"

#include "include/zenity_selector.h"
#include "include/config.h"
#include "include/notification.h"
#include "include/backup_options.h"
#include "include/compression.h"

// Função para exibir o menu e obter a escolha do usuário
int display_menu()
{
    int choice;
    std::cout << "Select an option:\n";
    std::cout << "1. Start backup and monitoring\n";
    std::cout << "2. Exit\n";
    std::cout << "Your choice: ";
    std::cin >> choice;
    return choice;
}

int main()
{
    const std::string config_file = "config.json";
    Config config = load_config(config_file); // Carrega as configurações

    // Se o caminho do diretório a ser monitorado não estiver configurado, solicitar ao usuário
    if (config.watch_path.empty())
    {
        std::cout << "Select the directory to be monitored:\n";
        config.watch_path = select_directory_with_zenity("Select the directory to monitor");
        std::cout << "You selected the directory to be monitored: " << config.watch_path << "\n";
    }
    else
    {
        std::cout << "Loaded monitored directory from config: " << config.watch_path << "\n";
    }

    // Se o caminho do diretório de backup não estiver configurado, solicitar ao usuário
    if (config.backup_path.empty())
    {
        std::cout << "Select the directory where the backup will be made:\n";
        config.backup_path = select_directory_with_zenity("Select the backup directory");
        std::cout << "You selected the backup directory: " << config.backup_path << "\n";
    }
    else
    {
        std::cout << "Loaded backup directory from config: " << config.backup_path << "\n";
    }

    std::string backup_type = select_backup_type();
    bool compress_files = ask_user_about_compression();

    // Configuração do inotify
    int inotifyFd = inotify_init();
    if (inotifyFd < 0)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(inotifyFd, config.watch_path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);
    if (wd < 0)
    {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    std::cout << "The directory " << config.watch_path << " is being monitored... \n";

    EventHandler handler(inotifyFd, config.backup_path, compress_files);

    EventChecker checker(inotifyFd, handler);

    // Loop principal para verificar eventos
    while (true)
    {
        checker.check();
    }

    // Limpeza
    inotify_rm_watch(inotifyFd, wd);
    close(inotifyFd);

    // Salvar as configurações atuais no arquivo JSON
    save_config(config_file, config);

    return 0;
}
