#include "include/notification.h"
#include <cstdlib>

void send_notification(const std::string& title, const std::string& message) {
    std::string command = "notify-send \"" + title + "\" \"" + message + "\"";
    system(command.c_str());
}
