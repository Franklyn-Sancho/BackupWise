#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>

extern int copy_dir_to();

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void handle_event(struct inotify_event *event)
{
    if (event->len)
    {
        if (event->mask & IN_CREATE)
        {
            printf("The file %s was created.\n", event->name);
            copy_dir_to();
            return;
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

int check_event()
{
    int fd, wd, length;
    char buffer[BUF_LEN];

    fd = inotify_init();
    if (fd < 0)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    wd = inotify_add_watch(fd, "/home/franklyn/Documentos/Códigos/testanto backup", IN_MODIFY | IN_CREATE | IN_DELETE);
    if (wd < 0)
    {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("Diretório está sendo escutado... \n");

    while (1)
    {
        length = read(fd, buffer, BUF_LEN);
        if (length < 0)
        {
            perror("read");
            break;
        }

        for (int i = 0; i < length;)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            handle_event(event);
            i += sizeof(struct inotify_event) + event->len;
        }
    }

    (void)inotify_rm_watch(fd, wd);
    (void)close(fd);

    return 0;
}

int main() {
    /* int result = copy_dir_to();
    printf("Called Rust function, result: %d\n", result);
    return 0; */

    check_event();
    return 0;
}