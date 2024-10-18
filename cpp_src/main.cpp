#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "include/log.h"
#include "include/event_handler.h"
#include "include/event_checker.h"

#define WATCH_PATH_DEFAULT "/home/franklyn/Documentos/Códigos/testanto backup"

int main()
{
    std::string watch_path;
    std::string backup_path;

    // Prompts the user for the directories
    std::cout << "Enter the path of the directory to be monitored (default: " << WATCH_PATH_DEFAULT << "): ";
    std::getline(std::cin, watch_path);
    if (watch_path.empty()) {
        watch_path = WATCH_PATH_DEFAULT; // Uses the default if not provided
    }

    std::cout << "Enter the path of the backup directory: ";
    std::getline(std::cin, backup_path);

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

    std::cout << "The directory " << watch_path << " is being monitored... \n";

    EventHandler handler(inotifyFd, backup_path); // Passes the backup directory
    EventChecker checker(inotifyFd, handler);

    while (true) {
        checker.check();
    }

    inotify_rm_watch(inotifyFd, wd);
    close(inotifyFd);

    return 0;
}
