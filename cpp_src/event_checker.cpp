#include "include/event_checker.h"
#include <unistd.h>

#define BUF_LEN (1024 * (EVENT_SIZE + 16))
#define EVENT_SIZE (sizeof(struct inotify_event))

EventChecker::EventChecker(int inotifyFd, EventHandler handler) : inotifyFd(inotifyFd), handler(handler) {}

void EventChecker::check()
{
    char buffer[BUF_LEN];
    int length = read(inotifyFd, buffer, BUF_LEN);

    if (length < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (i < length)
    {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        handler.handle(event);
        i += EVENT_SIZE + event->len;
    }
}
