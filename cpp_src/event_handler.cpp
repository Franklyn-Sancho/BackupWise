#include <iostream>
#include <unistd.h>
#include <cstring>
#include "include/log.h"
#include "include/event_handler.h"
#include "include/notification.h"

extern "C" {
    int copy_file_to(const char *filename, const char *backup_path);
    int copy_directory(const char *src, const char *dst); // Confirme se o tipo de retorno está correto
}


#define WATCH_PATH "/home/franklyn/Documentos/Códigos/testanto backup"

EventHandler::EventHandler(int inotifyFd, const std::string &backup_path)
    : inotifyFd(inotifyFd), backup_path(backup_path) {}

void EventHandler::handle(struct inotify_event *event)
{
    if (event->len > 0 && strncmp(event->name, ".goutputstream", 14) != 0)
    {
        std::string src_path = std::string(WATCH_PATH) + "/" + event->name;

        if (event->mask & IN_ISDIR)
        {
            handle_directory_event(event, src_path);
        }
        else
        {
            handle_file_event(event, src_path);
        }
    }
    else
    {
        std::cout << "Ignored event: file name is empty or matches the ignored pattern." << std::endl;
    }
}

void EventHandler::handle_file_event(struct inotify_event *event, const std::string &src_path)
{
    if (event->mask & (IN_CREATE | IN_MODIFY))
    {
        backup_file(src_path);
    }
    else if (event->mask & IN_DELETE)
    {
        std::cout << "File deleted: " << src_path << std::endl;
    }
    else if (event->mask & IN_MOVED_FROM)
    {
        last_moved_from = src_path;
    }
    else if (event->mask & IN_MOVED_TO)
    {
        std::cout << "File moved from " << last_moved_from << " to " << src_path << std::endl;
        last_moved_from.clear();
        backup_file(src_path);
    }
}

void EventHandler::handle_directory_event(struct inotify_event *event, const std::string &src_path)
{
    if (event->mask & (IN_CREATE | IN_MODIFY))
    {
        backup_directory(src_path);
        int wd = inotify_add_watch(inotifyFd, src_path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);
        if (wd < 0)
        {
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }
    }
    else if (event->mask & IN_DELETE)
    {
        std::cout << "Directory deleted: " << src_path << std::endl;
    }
    else if (event->mask & IN_MOVED_FROM)
    {
        last_moved_from = src_path;
    }
    else if (event->mask & IN_MOVED_TO)
    {
        std::cout << "Directory moved from " << last_moved_from << " to " << src_path << std::endl;
        last_moved_from.clear();
        backup_directory(src_path);
    }
}

void EventHandler::backup_file(const std::string &src_path) {
    int result = copy_file_to(src_path.c_str(), backup_path.c_str());
    if (result != 0) {
        std::cerr << "Error while backing up file: " << src_path << "\n";
    } else {
        std::cout << "Successfully backed up file: " << src_path << " to " << backup_path << std::endl;
        // Notificar o sucesso do backup
        send_notification("Backup Completed", "Successfully backed up file: " + src_path);
    }
}

void EventHandler::backup_directory(const std::string &src_path) {
    int result = copy_directory(src_path.c_str(), backup_path.c_str());
    if (result != 0) {
        std::cerr << "Error while backing up directory: " << result << "\n";
    } else {
        std::cout << "Successfully backed up directory: " << src_path << " to " << backup_path << std::endl;
        // Notificar o sucesso do backup
        send_notification("Backup Completed", "Successfully backed up directory: " + src_path);
    }
}
