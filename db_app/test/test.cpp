//
// Created by ekaterina on 14.04.2020.
//


//////////сделать проверку перед запуском тестов на существование в БД такого пользователя и файла

#include "../inc/database_manager.h"
#include "gtest/gtest.h"

//#include "../inc/interfaces.h"
#include "../inc/users_database_manager.h"

#define SUCCESS 0
#define FAILED 1

struct UserConnectionData {
    string login = "pkaterina";
    string password = "123456789863";
    string wrong_password = "lalalala";

    string file_name = "test_file.txt";
    string dir_name = "./test_dir/";
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
        _mng.Authorize(userConnectionData.login, userConnectionData.password);

        _file_name = userConnectionData.file_name;
        _hash_sum = userConnectionData.hash_sum;
        _dir_name = userConnectionData.dir_name;
        _wrong_file_name = userConnectionData.wrong_file_name;
    }

    void TearDown()  override {}
};

TEST(MainFuncTest, test1) {
    DatabaseManager _mng = DatabaseManager();

    //UsersDatabaseManager userdb = UsersDatabaseManager();
    //_mng.Register("lala", "lala");
   // UserSession session = UserSession();
}

/** Тест на успешную регистрацию и авторизацию **/
TEST_F(TestUserData, test_succesful) {
    int reg_res = _mng.Register(_login, _password);
    //проверка на то, что регистрация была пройдена успешно
    ASSERT_EQ(reg_res, SUCCESS);

    int auth_res = _mng.Authorize(_login, _password);
    //проверка на то, что авторизация только что зарегистрированного пользователя прошла успешно
    ASSERT_EQ(auth_res, SUCCESS);
}

/** Тест на неудачную регистрацию (логин уже занят) и авторизацию (неправильный пароль) **/
TEST_F(TestUserData, test_failing_registration_authorization) {
    int reg_res = _mng.Register(_login, _password);
    ASSERT_EQ(reg_res, FAILED);

    int auth_res = _mng.Authorize(_login, _wrong_password);
    EXPECT_EQ(auth_res, FAILED);
}

/** Тест удачное опубликование файла **/
TEST_F(TestFileWork, test_succesful_add_files) {
    int upload = _mng.Upload(_file_name, _dir_name, _hash_sum);
    EXPECT_EQ(upload, SUCCESS);
}

/** Тест на удачное скачивание файла **/
TEST_F(TestFileWork, test_succesful_download_files) {
    FILE * download = _mng.Download(_file_name, _dir_name);
    //EXPECT_EQ(download, true);
    EXPECT_TRUE(download);
}

/** Тест на неуспешное добавление файла (уже существует) **/
TEST_F(TestFileWork, test_failing_adding_files) {
    int upload = _mng.Upload(_file_name, _dir_name, _hash_sum);
    EXPECT_EQ(upload, FAILED);
}

/** Тест на неудачное скачивание файла (неверное имя файла) **/
TEST_F(TestFileWork, test_failing_download_files) {
    FILE * download = _mng.Download(_wrong_file_name, _dir_name);
    //EXPECT_EQ(download, false);
    EXPECT_FALSE(download);
}

/** Тест на неудачное удаление файла (неверное имя файла) **/
TEST_F(TestFileWork, test_failing_delete_files) {
    int del_file = _mng.DeleteFile(_wrong_file_name, _dir_name);
    EXPECT_EQ(del_file, FAILED);
}


/** Тест на удачное удаление файла **/
TEST_F(TestFileWork, test_succesful_delete_files) {
    int del_file = _mng.DeleteFile(_file_name, _dir_name);
    EXPECT_EQ(del_file, SUCCESS);
}

/** Тест на неудачное (неправильный пароль) удаление пользователя и удачное**/
TEST_F(TestUserData, test_failing_succesful_deleting) {
    int del_res = _mng.DeleteUser(_login, _wrong_password);
    ASSERT_EQ(del_res, FAILED);

    del_res = _mng.DeleteUser(_login, _password);
    ASSERT_EQ(del_res, SUCCESS);
}
