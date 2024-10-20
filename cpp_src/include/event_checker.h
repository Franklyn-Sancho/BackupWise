#ifndef EVENT_CHECKER_H
#define EVENT_CHECKER_H

#include "event_handler.h"

class EventChecker
{
    int inotifyFd;
    EventHandler &handler; // Usa referência para o handler

public:
    EventChecker(int inotifyFd, EventHandler &handler);
    void check();
};

#endif // EVENT_CHECKER_H

