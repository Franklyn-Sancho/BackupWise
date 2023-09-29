/* #include <fstream>
#include <string>

void backupFile(const std::string& originalFile, const std::string& backupFile) {

    std::ifstream src(originalFile, std::ios::binary);
    std::ofstream dst(backupFile, std::ios::binary | std::ios::app);

    dst << src.rdbuf();
}

int main() {
    const std::string originalFileName = "original.txt";
    const std::string backupFileName = "salvo.txt";

    backupFile(originalFileName, backupFileName);

    return 0;
} */