#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "include/log.h"
#include "include/event_handler.h"
#include "include/event_checker.h"

#include "include/zenity_selector.h"



int main() {
    // Select the directory to be monitored using Zenity
    std::cout << "Select the directory to be monitored:\n";
    std::string watch_path = select_directory_with_zenity("Select the directory to monitor");
    std::cout << "You selected the directory to be monitored: " << watch_path << "\n";

    // Select the backup directory using Zenity
    std::cout << "Select the directory where the backup will be made:\n";
    std::string backup_path = select_directory_with_zenity("Select the backup directory");
    std::cout << "You selected the backup directory: " << backup_path << "\n";

    // Inotify configuration
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

    EventHandler handler(inotifyFd, backup_path);
    EventChecker checker(inotifyFd, handler);

    while (true) {
        checker.check();
    }

    inotify_rm_watch(inotifyFd, wd);
    close(inotifyFd);

    return 0;
}

