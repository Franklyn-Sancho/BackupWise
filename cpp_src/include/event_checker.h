#ifndef EVENT_CHECKER_H
#define EVENT_CHECKER_H

#include "event_handler.h"

class EventChecker
{
    int fd;
    EventHandler handler;

public:
    EventChecker(int fd, EventHandler handler);
    void check();
};

#endif // EVENT_CHECKER_H
