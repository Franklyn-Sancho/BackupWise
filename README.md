# BackupWise - A File Backup Application CLI

## Overview
This application in C++ and RUST is designed to monitor a specified directory for file changes and automatically back up modified files to a designated backup location. It utilizes the Linux inotify API to listen for file system events such as creation, modification, movement, and deletion of files.

## Features
- **Real-time Monitoring:** Continuously watches a specified directory for any file changes.
- **Automatic Backup:** Automatically copies modified files to a backup directory.
- **Directory Support:** Capable of monitoring entire directories and backing up their contents.
- **Logging:** Records file modifications to a log file for tracking changes.

## Prerequisites
- **Linux Operating System:** This application is designed to run on Linux, as it uses the inotify API.
- **C++ Compiler:** Ensure you have a compatible C++ compiler installed (e.g., g++).

## Installation
Clone the repository:
```bash
git clone <repository-url>
cd <repository-directory>
cd cpp_src
make
```

## Usage
run the application
```bash
./main 
```

When prompted, enter the path of the directory to be monitored (default: /home/franklyn/Documentos/Códigos/testanto backup), followed by the path of the backup directory.

The application will begin monitoring the specified directory and will automatically back up files as they are modified.

## Contributing
Contributions are welcome! Feel free to open issues or submit pull requests.

![BackupWise Logo](https://github.com/Franklyn-Sancho/BackupWise/blob/main/backup.png)
