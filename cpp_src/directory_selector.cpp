#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

std::string select_directory(const std::string& initial_path = "/home") {
    std::vector<std::filesystem::path> entries;
    std::filesystem::path current_path = initial_path;

    while (true) {
        entries.clear();
        std::cout << "\n===== Directory Selection =====\n";
        std::cout << "Current directory: " << current_path << "\n";
        std::cout << "0. Select the current directory\n";
        std::cout << "1. .. (Go back to the previous directory)\n";
        std::cout << "2. Enter the path manually\n";

        int index = 3;
        for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
            if (entry.is_directory()) {
                entries.push_back(entry.path());
                std::cout << index << ". " << entry.path().filename().string() << "\n";
                ++index;
            }
        }

        int choice = 0;
        std::cout << "\nEnter the option number or -1 to cancel: ";
        std::cin >> choice;

        if (choice == -1) {
            std::cout << "Operation canceled.\n";
            return "";
        } else if (choice == 0) {
            return current_path.string();
        } else if (choice == 1) {
            current_path = current_path.parent_path();  // Go up to the parent directory
        } else if (choice == 2) {
            std::string manual_path;
            std::cout << "Enter the directory path: ";
            std::cin.ignore();
            std::getline(std::cin, manual_path);
            std::filesystem::path new_path = manual_path;
            if (std::filesystem::is_directory(new_path)) {
                current_path = new_path;
            } else {
                std::cout << "Invalid path. Please try again.\n";
            }
        } else if (choice >= 3 && choice < index) {
            current_path = entries[choice - 3];  // Navigate to the selected directory
        } else {
            std::cout << "Invalid option. Please try again.\n";
        }
    }
}



