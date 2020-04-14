//
// Created by ekaterina on 14.04.2020.
//

#include "gtest/gtest.h"
#include "../inc/database_manager.h"

#define SUCCESS 0
#define FAILED 1

////добавить фичи!!!

TEST(testMainFunc, test_succesful) {
    std::string login("pkaterinaa");
    std::string password("123456789863");

    DatabaseManager mng = DatabaseManager();
    int reg_res = mng.Register(login, password);
    //проверка на то, что регистрация была пройдена успешно
    ASSERT_EQ(reg_res, SUCCESS);

    int auth_res = mng.Authorize(login, password);
    //проверка на то, что авторизация только что зарегистрированного пользователя прошла успешно
    ASSERT_EQ(auth_res, SUCCESS);

    int del_res = mng.DeleteUser(login, password);
    EXPECT_EQ(del_res, SUCCESS);
}

TEST(testMainFunc, test_failing_authorization) {
    std::string login("pkaterinaa");
    std::string right_password("123456789863");

    std::string wrong_password("lalalala");

    DatabaseManager mng = DatabaseManager();
    mng.Register(login, right_password);

    int auth_res = mng.Authorize(login, wrong_password);
    EXPECT_EQ(auth_res, FAILED);
}

TEST(testMainFunc, test_failing_registration) {
    std::string login("pkaterinaa");
    std::string password("lalala");

    DatabaseManager mng = DatabaseManager();
    int reg_res = mng.Register(login, password);
    EXPECT_EQ(reg_res, FAILED);
}

TEST(testMainFunc, test_failing_deleting) {
    std::string login("pkaterinaa");
    std::string wrong_password("lalala");

    DatabaseManager mng = DatabaseManager();
    int del_res = mng.DeleteUser(login, wrong_password);
    EXPECT_EQ(del_res, FAILED);
}

TEST(testMainFunc, test_succesful_add_files) {
    std::string login("pkaterinaa");
    std::string password("123456789863");

    std::string file_name("test_file.txt");
    std::string dir_name("./test_dir/");
    int hash_sum = 12345;

    DatabaseManager mng = DatabaseManager();
    mng.Authorize(login, password);

    int upload = mng.UploadFile(file_name, dir_name, hash_sum);
    EXPECT_EQ(upload, SUCCESS);
}

TEST(testMainFunc, test_succesful_download_files) {
    std::string login("pkaterinaa");
    std::string password("123456789863");

    std::string file_name("test_file.txt");
    std::string dir_name("./test_dir/");
    int hash_sum = 12345;

    DatabaseManager mng = DatabaseManager();
    mng.Authorize(login, password);

    FILE * download = mng.DownloadFile(file_name, dir_name);
    EXPECT_EQ(download, true);
}


TEST(testMainFunc, test_succesful_delete_files) {
    std::string login("pkaterinaa");
    std::string password("123456789863");

    std::string file_name("test_file.txt");
    std::string dir_name("./test_dir/");
    int hash_sum = 12345;

    DatabaseManager mng = DatabaseManager();
    mng.Authorize(login, password);

    int del_file = mng.DeleteFile(file_name, dir_name);
    EXPECT_EQ(del_file, SUCCESS);
}

TEST(testMainFunc, test_failing_adding_files) {
    std::string login("pkaterinaa");
    std::string password("123456789863");

    std::string file_name("test_file.txt");
    std::string dir_name("./test_dir/");
    int hash_sum = 12345;

    DatabaseManager mng = DatabaseManager();
    mng.Authorize(login, password);

    int upload = mng.UploadFile(file_name, dir_name, hash_sum);
    EXPECT_EQ(upload, FAILED);
}

TEST(testMainFunc, test_failing_download_files) {
    std::string login("pkaterinaa");
    std::string password("123456789863");

    std::string wrong_file_name("test_file_1.txt");
    std::string dir_name("./test_dir/");
    int hash_sum = 12345;

    DatabaseManager mng = DatabaseManager();
    mng.Authorize(login, password);

    FILE * download = mng.DownloadFile(wrong_file_name, dir_name);
    EXPECT_EQ(download, false);
}


TEST(testMainFunc, test_failing_delete_files) {
    std::string login("pkaterinaa");
    std::string password("123456789863");

    std::string wrong_file_name("test_file_1.txt");
    std::string dir_name("./test_dir/");
    int hash_sum = 12345;

    DatabaseManager mng = DatabaseManager();
    mng.Authorize(login, password);

    int del_file = mng.DeleteFile(wrong_file_name, dir_name);
    EXPECT_EQ(del_file, false);
}

