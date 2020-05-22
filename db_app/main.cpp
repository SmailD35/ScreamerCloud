#include <iostream>
#include "./inc/database_manager.h"
#include "./inc/interfaces.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto session = UserSession();
    return 0;
}


///создание директории в ФС при регистрации пользователя
///проверка наличия файлов не только в БД, но и в ФС (выкидыаем false или код ошибки)