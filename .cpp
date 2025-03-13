#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream> // Для работы с файловыми потоками ввода/вывода

namespace fs = std::filesystem;
//ls <путь> — показать содержимое указанной папки(файлы и каталоги).
//touch <путь> — создать новый пустой файл по указанному пути.
//mkdir <путь> — создать новую папку по указанному пути.
//rm <путь> — удалить файл или папку по указанному пути(для папок — рекурсивно).
//mv <старый путь> <новый путь> — переименовать или переместить файл / папку.
//cp <источник> <назначение> — скопировать файл / папку в указанное место.
//size <путь> — вычислить общий размер файла или всех файлов в папке(рекурсивно).
//find <путь> <маска> — выполнить поиск файлов в указанной папке по имени(с учётом подкаталогов).
//exit — завершить работу программы.

class FileManager {
public:
    // Показать содержимое каталога
    void showContents(const std::string& path) {
        for (const auto& entry : fs::directory_iterator(path)) {
            std::cout << (entry.is_directory() ? "[DIR]  " : "[FILE] ") << entry.path().filename().string() << '\n';
        }
    }

    // Создать файл
    void createFile(const std::string& path) {
        std::ofstream file(path);
        if (file) std::cout << "Файл создан: " << path << '\n';
        else std::cout << "Не удалось создать файл: " << path << '\n';
    }

    // Создать каталог
    void createDirectory(const std::string& path) {
        if (fs::create_directory(path))
            std::cout << "Каталог создан: " << path << '\n';
        else
            std::cout << "Не удалось создать каталог: " << path << '\n';
    }

    // Удалить файл/каталог
    void deleteEntry(const std::string& path) {
        if (fs::remove_all(path))
            std::cout << "Удалено: " << path << '\n';
        else
            std::cout << "Не удалось удалить: " << path << '\n';
    }

    // Переименовать или переместить файл/каталог
    void renameEntry(const std::string& oldPath, const std::string& newPath) {
        fs::rename(oldPath, newPath);
        std::cout << "Переименовано: " << oldPath << " -> " << newPath << '\n';
    }

    // Копировать файл/каталог
    void copyEntry(const std::string& source, const std::string& destination) {
        fs::copy(source, destination, fs::copy_options::recursive);
        std::cout << "Скопировано: " << source << " -> " << destination << '\n';
    }

    // Вычислить размер каталога/файла
    uintmax_t calculateSize(const std::string& path) {
        uintmax_t size = 0;
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (fs::is_regular_file(entry)) {
                size += fs::file_size(entry);
            }
        }
        return size;
    }

    // Поиск файлов по маске
    void search(const std::string& path, const std::string& mask) {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (entry.path().filename().string().find(mask) != std::string::npos) {
                std::cout << entry.path().string() << '\n';
            }
        }
    }
};

int main() {
    FileManager fm;
    std::string command, path, newPath;

    setlocale(LC_ALL, "Rus");
    while (true) {
        std::cout << "Введите команду и путь (ls, touch, mkdir, rm, mv, cp, size, find, exit): ";
        std::cin >> command;

        if (command == "ls") {
            std::cin >> path;
            fm.showContents(path);
        }
        else if (command == "touch") {
            std::cin >> path;
            fm.createFile(path);
        }
        else if (command == "mkdir") {
            std::cin >> path;
            fm.createDirectory(path);
        }
        else if (command == "rm") {
            std::cin >> path;
            fm.deleteEntry(path);
        }
        else if (command == "mv") {
            std::cin >> path >> newPath;
            fm.renameEntry(path, newPath);
        }
        else if (command == "cp") {
            std::cin >> path >> newPath;
            fm.copyEntry(path, newPath);
        }
        else if (command == "size") {
            std::cin >> path;
            std::cout << "Размер: " << fm.calculateSize(path) << " байт" << '\n';
        }
        else if (command == "find") {
            std::cin >> path >> newPath;
            fm.search(path, newPath);
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "Неизвестная команда!" << '\n';
        }
    }
    return 0;
}
