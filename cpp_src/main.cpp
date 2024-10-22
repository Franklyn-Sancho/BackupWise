#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "include/log.h"
#include "include/event_handler.h"
#include "include/event_checker.h"

#include "include/directory_selector.h"

int main() {
    // Seleção do diretório a ser monitorado
    std::cout << "Selecione o diretório que será monitorado:\n";
    std::string watch_path = select_directory(); 
    std::cout << "Você selecionou o diretório a ser monitorado: " << watch_path << "\n";

    // Seleção do diretório de backup
    std::cout << "Selecione o diretório onde será feito o backup:\n";
    std::string backup_path = select_directory(); 
    std::cout << "Você selecionou o diretório de backup: " << backup_path << "\n";

    // Configuração do inotify
    int inotifyFd = inotify_init();
    if (inotifyFd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(inotifyFd, watch_path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);
    if (wd < 0) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    std::cout << "O diretório " << watch_path << " está sendo monitorado... \n";

    EventHandler handler(inotifyFd, backup_path);
    EventChecker checker(inotifyFd, handler);

    while (true) {
        checker.check();
    }

    inotify_rm_watch(inotifyFd, wd);
    close(inotifyFd);

    return 0;
}

