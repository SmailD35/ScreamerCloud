#include <iostream>
#include "./inc/database_manager.h"
#include "./inc/interfaces.h"


///TO DO:
///1) продумать системы кодов ошибок (и их обработку)
///2) туду-шка в тестах

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto session = UserSession();
    return 0;
}
