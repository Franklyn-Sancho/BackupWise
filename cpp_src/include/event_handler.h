#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <string>
#include <sys/inotify.h>

class EventHandler
{
    int fd;
    std::string last_moved_from;

public:
    EventHandler(int fd);
    void handle(struct inotify_event *event);

private:
    void print_event_message(const std::string &event_type, const std::string &file_name);
    void handle_create(struct inotify_event *event);
    void handle_moved_from(struct inotify_event *event);
    void handle_moved_to(struct inotify_event *event);
    void handle_delete(struct inotify_event *event);
    void handle_modify(struct inotify_event *event);
};

#endif // EVENT_HANDLER_H
