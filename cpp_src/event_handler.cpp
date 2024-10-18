#include <iostream>
#include <unistd.h>
#include <cstring>
#include "include/log.h"
#include "include/event_handler.h"

extern "C" int copy_file_to(const char *filename, const char *backup_path); // Add the backup_path

#define WATCH_PATH "/home/franklyn/Documentos/Códigos/testanto backup"

// Modifies the constructor to initialize inotifyFd and backup_path
EventHandler::EventHandler(int inotifyFd, const std::string& backup_path)
    : inotifyFd(inotifyFd), backup_path(backup_path) {}

void EventHandler::handle(struct inotify_event *event)
{
    if (event->len > 0 && strncmp(event->name, ".goutputstream", 14) != 0)
    {
        if (event->mask & IN_CREATE || event->mask & IN_MODIFY)
        {
            handle_create(event);
        }
        else if (event->mask & IN_MOVED_FROM)
        {
            handle_moved_from(event);
        }
        else if (event->mask & IN_MOVED_TO)
        {
            handle_moved_to(event);
        }
        else if (event->mask & IN_DELETE)
        {
            handle_delete(event);
        }

        // Build the full path of the file
        std::string src_path = std::string(WATCH_PATH) + "/" + event->name;

        // Call the copy_file_to function with the full path and the backup path
        int result = copy_file_to(src_path.c_str(), backup_path.c_str());
        if (result != 0)
        {
            std::cerr << "Error while backing up: " << result << "\n";
        }
    }
    else
    {
        std::cout << "Ignored event: file name is empty or matches the ignored pattern." << std::endl;
    }
}

void EventHandler::print_event_message(const std::string &event_type, const std::string &file_name)
{
    std::cout << "The file " << file_name << " was " << event_type << ".\n";
}

void EventHandler::handle_create(struct inotify_event *event)
{
    print_event_message("created", event->name);
    if (event->mask & IN_ISDIR)
    {
        std::cout << "New directory created: " << event->name << std::endl;
        usleep(1000);
        std::string new_dir = std::string(WATCH_PATH) + "/" + event->name;
        int wd = inotify_add_watch(inotifyFd, new_dir.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);
        if (wd < 0)
        {
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }
    }
    // Removed unnecessary logging
}

void EventHandler::handle_moved_from(struct inotify_event *event)
{
    last_moved_from = event->name;
    // Removed unnecessary logging
}

void EventHandler::handle_moved_to(struct inotify_event *event)
{
    std::cout << "The file " << last_moved_from << " was moved or renamed to " << event->name << ".\n";
    last_moved_from.clear();
    // Removed unnecessary logging
}

void EventHandler::handle_delete(struct inotify_event *event)
{
    print_event_message("deleted", event->name);
    // Removed unnecessary logging
}

void EventHandler::handle_modify(struct inotify_event *event)
{
    print_event_message("modified", event->name);
    // Removed unnecessary logging
}

