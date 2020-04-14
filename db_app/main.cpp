#include <iostream>
#include "./inc/database_manager.h"
#include "./inc/interfaces.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    auto session = UserSession();
    return 0;
}
