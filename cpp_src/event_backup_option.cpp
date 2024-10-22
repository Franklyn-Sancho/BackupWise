#include "include/backup_options.h"
#include <iostream>

std::string select_backup_type() {
    std::cout << "Choose the type of backup:\n";
    std::cout << "1. Full Backup\n";
    std::cout << "2. Incremental Backup\n";
    std::cout << "3. Differential Backup\n";
    std::cout << "Enter your choice (1-3): ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            return "Full";
        case 2:
            return "Incremental";
        case 3:
            return "Differential";
        default:
            std::cout << "Invalid choice, defaulting to Full Backup.\n";
            return "Full";
    }
}
