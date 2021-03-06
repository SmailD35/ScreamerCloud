//
// Created by ekaterina on 14.04.2020.
//


//////////сделать проверку перед запуском тестов на существование в БД такого пользователя и файла

#include "../inc/database_manager.h"
#include "gtest/gtest.h"

using namespace std;
namespace fs = boost::filesystem;

#define SUCCESS 0
#define FAILED 1

struct UserConnectionData {
    string login = "pkaterina";
    string password = "123456789863";
    string wrong_password = "lalalala";

    string file_name = "test_user_file.txt";
    string dir_name = "../test_user_dir/";
    string hash_sum = "123445";

    string wrong_file_name = "test_file_1.txt";
};

class TestUserData : public ::testing::Test {
protected:
    DatabaseManager _mng;

    string _login;
    string _password;
    string _wrong_password;

    void SetUp() override {
        UserConnectionData userConnectionData = UserConnectionData();
        _mng = DatabaseManager();

        _login = userConnectionData.login;
        _password = userConnectionData.password;
        _wrong_password = userConnectionData.wrong_password;
    }
};

class TestFileWork : public ::testing::Test {
protected:
    string _file_name;
    string _dir_name;
    string _hash_sum;
    string _wrong_file_name;
    DatabaseManager _mng;

    void SetUp() override {
        UserConnectionData userConnectionData = UserConnectionData();
        _mng = DatabaseManager();
        DbErrorCodes error;
        _mng.Authorize(userConnectionData.login, userConnectionData.password, error);

        _file_name = userConnectionData.file_name;
        _hash_sum = userConnectionData.hash_sum;
        _dir_name = userConnectionData.dir_name;
        _wrong_file_name = userConnectionData.wrong_file_name;
    }

    void TearDown()  override {}
};


class TestDeletings : public ::testing::Test {
protected:
    DatabaseManager _mng;

    string _login;
    string _password;

    string _file_name;
    string _dir_name;

    string _wrong_file_name;
    string _wrong_password;

    void SetUp() override {
        UserConnectionData userConnectionData = UserConnectionData();
        _mng = DatabaseManager();

        _login = userConnectionData.login;
        _password = userConnectionData.password;

        _file_name = userConnectionData.file_name;
        _dir_name = userConnectionData.dir_name;
        _wrong_file_name = userConnectionData.wrong_file_name;
        _wrong_password = userConnectionData.wrong_password;
    }
};


/** Тест на успешную регистрацию и авторизацию **/
TEST_F(TestUserData, test_succesful) {
    DbErrorCodes error;
    bool reg_res = _mng.Register(_login, _password, error);
    //проверка на то, что регистрация была пройдена успешно
    ASSERT_TRUE(reg_res);

    bool auth_res = _mng.Authorize(_login, _password, error);
    //проверка на то, что авторизация только что зарегистрированного пользователя прошла успешно
    ASSERT_TRUE(auth_res);
}

/** Тест на неудачную регистрацию (логин уже занят) и авторизацию (неправильный пароль) **/
TEST_F(TestUserData, test_failing_registration_authorization) {
    DbErrorCodes error;
    bool reg_res = _mng.Register(_login, _password, error);
    ASSERT_FALSE(reg_res);

    bool auth_res = _mng.Authorize(_login, _wrong_password, error);
    EXPECT_FALSE(auth_res);
}

/** Тест удачное опубликование файла **//*
TEST_F(TestFileWork, test_succesful_add_files) {
    DbErrorCodes error;
    string user_path = _mng.GetUserDir();
    fs::path p("../test_dir/" + user_path + "/test_user_file");
    fs::copy_file("../../test_user_file", p);

    bool upload = _mng.Upload(_file_name, _dir_name, _hash_sum, error);
    EXPECT_TRUE(upload);
}*/
/** Тест на удачное скачивание файла **//*
TEST_F(TestFileWork, test_succesful_download_files) {
    shared_ptr<FILE> download = _mng.Download(_file_name, _dir_name);
    //EXPECT_EQ(download, true);
    EXPECT_TRUE(download);
}*/

/** Тест на неуспешное добавление файла (уже существует) **/
TEST_F(TestFileWork, test_failing_adding_files) {
    DbErrorCodes error;
    bool upload = _mng.Upload(_file_name, _dir_name, _hash_sum, error);
    EXPECT_FALSE(upload);
}

/** Тест на неудачное скачивание файла (неверное имя файла) **/
/*TEST_F(TestFileWork, test_failing_download_files) {
    shared_ptr<FILE> download = _mng.Download(_wrong_file_name, _dir_name);
    //EXPECT_EQ(download, false);
    EXPECT_FALSE(download);
}*/

/** Тест на неудачное удаление файла (неверное имя файла) **/
TEST_F(TestDeletings, test_failing_delete_files) {
    DbErrorCodes error;
    _mng.Authorize(_login, _password, error);
    bool del_file = _mng.DeleteFile(_wrong_file_name, _dir_name, error);
    EXPECT_FALSE(del_file);
}


/** Тест на удачное удаление файла **//*
TEST_F(TestDeletings, test_succesful_delete_files) {
    DbErrorCodes error;
    _mng.Authorize(_login, _password, error);
    bool del_file = _mng.DeleteFile(_file_name, _dir_name, error);
    EXPECT_TRUE(del_file);
}*/

/** Тест на неудачное (неправильный пароль) удаление пользователя и удачное**/
TEST_F(TestDeletings, test_failing_succesful_deleting) {
    DbErrorCodes error;
    bool del_res = _mng.DeleteUser(_login, _wrong_password, error);
    ASSERT_FALSE(del_res);

    del_res = _mng.DeleteUser(_login, _password, error);
    ASSERT_TRUE(del_res);
}

