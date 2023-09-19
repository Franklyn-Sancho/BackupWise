#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include "copy_dir_to.h"

/* extern int copy_dir_to(); // rust function
extern int copy_file_to(const char* filename); */

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + NAME_MAX + 1))
#define WATCH_PATH  "/home/franklyn/Documentos/Códigos/testanto backup"

void handle_event(struct inotify_event *event)
{
    if (event->len > 0 && strncmp(event->name, ".goutputstream", 14) != 0)
    {
        if (event->mask & IN_CREATE)
        {
            printf("The file %s was created.\n", event->name);
            copy_file_to(event->name);
            return;
        }
        else if (event->mask & IN_MOVED_FROM)
        {
            printf("The file %s was moved or renamed from.\n", event->name);
        }
        else if (event->mask & IN_MOVED_TO)
        {
            printf("The file %s was moved or renamed to.\n", event->name);
        }
        else if (event->mask & IN_DELETE)
        {
            printf("The file %s was deleted.\n", event->name);
        }
        else if (event->mask & IN_MODIFY)
        {
            printf("The file %s was modified.\n", event->name);
        }
    }
}

void check_event(int fd)
{
    char buffer[BUF_LEN];
    int length, i = 0;

    length = read(fd, buffer, BUF_LEN);
    if (length < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    while (i < length)
    {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        handle_event(event);
        i += EVENT_SIZE + event->len;
    }
}

int main()
{
    int fd, wd;

    fd = inotify_init();
    if (fd < 0)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    wd = inotify_add_watch(fd, WATCH_PATH, IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);
    if (wd < 0)
    {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("O Diretório está sendo escutado... \n");

    while (1)
    {
        check_event(fd);
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}