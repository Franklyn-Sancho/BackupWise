#include <iostream>
#include <unistd.h>
#include <cstring>
#include "include/log.h"
#include "include/event_handler.h"

extern "C" int copy_file_to(const char *filename);

#define WATCH_PATH "/home/franklyn/Documentos/Códigos/testanto backup"

EventHandler::EventHandler(int fd) : fd(fd) {}

void EventHandler::handle(struct inotify_event *event)
{
    if (event->len > 0 && strncmp(event->name, ".goutputstream", 14) != 0)
    {
        if (event->mask & IN_CREATE)
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
        else if (event->mask & IN_MODIFY)
        {
            handle_modify(event);
        }
        int result = copy_file_to(event->name);
        if (result != 0)
        {
            std::cerr << "Erro ao fazer backup.\n";
        }
    }
}

void EventHandler::print_event_message(const std::string &event_type, const std::string &file_name)
{
    std::cout << "O arquivo " << file_name << " foi " << event_type << ".\n";
}

void EventHandler::handle_create(struct inotify_event *event)
{
    print_event_message("criado", event->name);
    if (event->mask & IN_ISDIR)
    {
        usleep(1000);
        std::string new_dir = std::string(WATCH_PATH) + "/" + event->name;
        int wd = inotify_add_watch(fd, new_dir.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);
        if (wd < 0)
        {
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }
    }
    write_log("modificado", event->name);
}

void EventHandler::handle_moved_from(struct inotify_event *event)
{
    last_moved_from = event->name;
    write_log("modificado", event->name);
}

void EventHandler::handle_moved_to(struct inotify_event *event)
{
    std::cout << "O arquivo " << last_moved_from << " foi movido ou renomeado para " << event->name << ".\n";
    last_moved_from.clear();
}

void EventHandler::handle_delete(struct inotify_event *event)
{
    print_event_message("deletado", event->name);
    write_log("modificado", event->name);
}

void EventHandler::handle_modify(struct inotify_event *event)
{
    print_event_message("modificado", event->name);
}
