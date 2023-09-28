#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <ctime>

std::ofstream log_file;

extern "C" int copy_file_to(const char *filename);

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))
#define WATCH_PATH "/home/franklyn/Documentos/Códigos/testanto backup"

//refactoring required
void write_log(const std::string &event_type, const std::string &file_name) {
    std::ofstream log_file;
    log_file.open("log.txt", std::ios_base::app);

    std::time_t current_time = std::time(0);
    char buffer[80];
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));

    log_file << "O arquivo " << file_name << " foi " << event_type << " em " << buffer << ".\n";
    log_file.close();
}

class EventHandler
{
    int fd;
    std::string last_moved_from;

public:
    EventHandler(int fd) : fd(fd) {}

    void handle(struct inotify_event *event)
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

private:
    void print_event_message(const std::string &event_type, const std::string &file_name)
    {
        std::cout << "O arquivo " << file_name << " foi " << event_type << ".\n";
    }

    void handle_create(struct inotify_event *event)
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

    void handle_moved_from(struct inotify_event *event)
    {
        last_moved_from = event->name;
        write_log("modificado", event->name);
    }

    void handle_moved_to(struct inotify_event *event)
    {
        std::cout << "O arquivo " << last_moved_from << " foi movido ou renomeado para " << event->name << ".\n";
        last_moved_from.clear();
    }

    void handle_delete(struct inotify_event *event)
    {
        print_event_message("deletado", event->name);
        write_log("modificado", event->name);
    }

    void handle_modify(struct inotify_event *event)
    {
        print_event_message("modificado", event->name);
    }
};

class EventChecker
{
    int fd;
    EventHandler handler;

public:
    EventChecker(int fd, EventHandler handler) : fd(fd), handler(handler) {}

    void check()
    {
        char buffer[BUF_LEN];
        int length = read(fd, buffer, BUF_LEN);

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
};


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

    EventHandler handler(fd);
    EventChecker checker(fd, handler);

    while (1)
    {
        checker.check();
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}
