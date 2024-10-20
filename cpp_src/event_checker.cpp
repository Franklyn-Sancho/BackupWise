#include "include/event_checker.h"
#include <unistd.h>
#include <iostream>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

EventChecker::EventChecker(int inotifyFd, EventHandler &handler)
    : inotifyFd(inotifyFd), handler(handler) {}

void EventChecker::check() {
    char buffer[BUF_LEN];
    int length = read(inotifyFd, buffer, BUF_LEN);

    if (length < 0) {
        if (errno == EINTR) {
            // Interrupção do sistema, tentar novamente
            return;
        }
        perror("read");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (i < length) {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];

        // Verifica se o evento possui um comprimento válido
        if (event->len > 0) {
            // Tenta lidar com o evento usando try-catch para capturar possíveis falhas
            try {
                handler.handle(event);
            } catch (const std::exception &e) {
                std::cerr << "Error processing event: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Event length is zero, skipping." << std::endl;
        }

        i += EVENT_SIZE + event->len;
    }
}




