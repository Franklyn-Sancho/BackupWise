#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "include/log.h"
#include "include/event_handler.h"
#include "include/event_checker.h"

#define WATCH_PATH "/home/franklyn/Documentos/Códigos/testanto backup"

int main()
{
    int inotifyFd = inotify_init();
    if (inotifyFd < 0)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(inotifyFd, WATCH_PATH, IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);
    if (wd < 0)
    {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    std::cout << "O Diretório está sendo escutado... \n";

    EventHandler handler(inotifyFd);
    EventChecker checker(inotifyFd, handler);

    while (1)
    {
        checker.check();
    }

    inotify_rm_watch(inotifyFd, wd);
    close(inotifyFd);

    return 0;
}