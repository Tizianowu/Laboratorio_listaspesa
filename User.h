//
// Created by Tiziano Wu on 05/09/24.
//

#ifndef LABORATORIO_LISTASPESA_USER_H
#define LABORATORIO_LISTASPESA_USER_H
#include <string>

class User {
public:
    User(std::string& Name);

    void AddUser();
private:
    std::string UserName;
};


#endif //LABORATORIO_LISTASPESA_USER_H
