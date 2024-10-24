#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <string>
#include <sys/inotify.h>

class EventHandler
{
    int inotifyFd;
    std::string last_moved_from;
    std::string backup_path;
    bool compress_files; // Variável para controlar a compressão

public:
    // Corrigir o construtor para incluir compress_files
    EventHandler(int inotifyFd, const std::string& backup_path, bool compress_files); 
    void handle(struct inotify_event *event);

private:
    void print_event_message(const std::string &event_type, const std::string &file_name);
    void handle_create(struct inotify_event *event);
    void handle_moved_from(struct inotify_event *event);
    void handle_moved_to(struct inotify_event *event);
    void handle_delete(struct inotify_event *event);
    void handle_modify(struct inotify_event *event);
    void handle_file_event(struct inotify_event *event, const std::string &src_path);
    void handle_directory_event(struct inotify_event *event, const std::string &src_path);

    void backup_file(const std::string &file_path);
    void backup_directory(const std::string &dir_path);
};

#endif // EVENT_HANDLER_H

