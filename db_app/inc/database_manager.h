//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_DATABASE_MANAGER_H
#define DB_APP_DATABASE_MANAGER_H

#include "files_database_manager.h"
#include "users_database_manager.h"

class DatabaseManager{
private:
    UsersDatabaseManager _usersDatabaseManager;

    FilesDatabaseManager _filesDatabaseManager;

    int _userID;

    std::string _userDirectory;
public:
    DatabaseManager();

    ~DatabaseManager() = default;

    std::string GetUserDir();

    ///Функция проверки свободного места в ФС для записи нового файла, используется сервером перед тем, как начать передавать файл от пользователя
    unsigned long CheckAvailableSpace();

    ///Все функции, возврвщающие bool, работают по логичному принципу "все ОК = true, иначе (ошибка в работе с БД, некорректные входные данные и тд) false"
    ///Можно всем вместе продумать хорошую систему ошибок, которая будет храниться в структуре enum, но нужно это сделать централизованно
    /// и исправить текущую версию
    bool Register(const std::string &login, const std::string &password);

    bool Authorize(const std::string &login, const std::string &password);

    bool DeleteUser(const std::string &login, const std::string &password);

    /// !!!!     ВАЖНЫЙ МОМЕНТ     !!!!
    ///Объясняю систему хранения файлов:
    ///В ФС есть папка (так называемый users_storage), где хранятся все файлы пользователей (можно сделать примонтированный диск или что-то такое)
    ///У каждого пользователя в ней есть своя папка с названием, соответствующим ID пользователя (для удобства)
    ///В этой папке в независимости от подразумеваемой пользователем структуризации файлов хранятся обычные файлы с именами = ID этого файла
    ///Разбиение на пользовательские папки хранится исключительно в БД
    /// !!!!!!!!!!!!!!!!!!!!! МЕГА_ВАЖНО !!!!!!!!!!!!!!!!!!!
    /// Поскольку сервер, принимая файл записывает его в ФС, необходимо, чтобы он записал его в определенную директорию
    /// Я сделала для этого функцию std::string GetUserDir(), которая вернет путь к пользовательской папке в ФС
    /// Соответственно, сервер записывает файл с названием, которое ему дал пользователь в эту папку
    /// (без пути к пользовательской директории, только само имя файла, чтобы не плодить папки!)
    /// А обертка БД после удачной записи данных о файле в БД переименует этот файл соответственно его ID
    bool Upload(const std::string &file_name, const std::string &dir_name, const std::string &hash_sum) ;

    ///Функция скачивания файла возвращает умный указатель на открытый файл
    ///Если файл по какой-либо причине не может быть открыт (он не существует, нет такой записи в БД или проблема в работе с указателями),
    /// то возвращается nullptr
    ///=> перед чтением содержимого файла для передачи данных необходимо проверить на nullptr
    std::shared_ptr<FILE> Download(const std::string &file_name, const std::string &dir_name);

    bool DeleteFile(const std::string &file_name, const std::string &dir_name);

    /// Функция для возврата списка файлов пользователя
    /// На данный момент реализовна для нерекурсивного списка файлов, но можно сделать и рекурсивный
    /// Возвращаемое значение имеет следующий тип <item_name, item_type>,
    /// где item_type может иметь 2 значения: dir или file, для интерпретации этих данных пользователю
    std::map <std::string, std::string> GetFileList(const std::string &dir_name);
};


#endif //DB_APP_DATABASE_MANAGER_H
