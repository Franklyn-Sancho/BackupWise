#include <iostream>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include "copy_dir_to.h"

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + NAME_MAX + 1))
#define WATCH_PATH "/home/franklyn/Documentos/Códigos/testanto backup"

class EventHandler
{
public:
    
   virtual void handle(struct inotify_event *event) = 0;
};

class CreateEventHandler : public EventHandler
{
public:
    void handle(struct inotify_event *event) override
    {
        std::cout << "The file " << event->name << " was created.\n";
        copy_file_to(event->name);
    }
};

class MovedFromEventHandler : public EventHandler
{
public:
    void handle(struct inotify_event *event) override
    {
        std::cout << "The file " << event->name << " was moved from.\n";
    }
};

class MovedToEventHandler : public EventHandler
{
public:
    void handle(struct inotify_event *event) override
    {
        std::cout << "The file " << event->name << " was moved to.\n";
    }
};

class DeleteEventHandler : public EventHandler
{
public:
    void handle(struct inotify_event *event) override
    {
        std::cout << "the file " << event->name << "was deleted.\n";
        copy_file_to(event->name);
    }
};

class ModifyEventHandler : public EventHandler
{
public:
    void handle(struct inotify_event *event) override
    {
        std::cout << "The file " << event->name << " was modified.\n";
    }
};

void check_events(int fd, EventHandler *handlers[])
{
    char buffer[BUF_LEN];
    int length = read(fd, buffer, BUF_LEN);

    if (length < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < length;)
    {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        if (event->mask & IN_CREATE)
        {
            handlers[0]->handle(event);
        }
        i += EVENT_SIZE + event->len;
    }
}

int main()
{
    int fd = inotify_init();

    if (fd < 0)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(fd, WATCH_PATH, IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);

    if (wd < 0)
    {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    std::cout << "O Diretório está sendo escutado... \n";

    EventHandler *handlers[] = {new CreateEventHandler() /* , new MovedFromEventHandler(), ... */};

    while (1)
    {
        check_events(fd, handlers);
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}
