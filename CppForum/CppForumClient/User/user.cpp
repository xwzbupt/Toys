#include "user.h"
#include "Forum/forum.h"
#include "User/administrator.h"
#include "User/commonuser.h"
#include "User/moderator.h"
#include "User/anonymous.h"
#include <QDebug>

User* User::_user = nullptr;

User::User()
{
}

User::~User() = default;

bool User::LogIn(){
    try {
        Forum::Get().Show();
    } catch (std::runtime_error& e) {
        qDebug()<<e.what();
        return false;
    }
    qDebug()<<"logged in!";
    return true;
}

bool User::Logout(){
    if (!Forum::LogOut()) {
        return false;
    }
    qDebug()<<"logout!";
    delete _user;
    Forum::Get().Hide();
    return true;
}


infrastructure::Error User::TryLogin(QString id, QString password){
    auto response = Forum::Verify(id, password);

    if (response.error == infrastructure::SUCCESS) {
        switch (response.status) {
        case infrastructure::ADMINISTRATOR:
            _user = new Administrator();
            break;
        case infrastructure::COMMON_USER:
            _user = new CommonUser(response.postCount);
            break;
        case infrastructure::MODERATOR:
            _user = new Moderator(response.postCount);
            break;
        case infrastructure::ANONYMOUS:
            _user = new Anonymous();
            break;
        default:
            break;
        }
        _user->id = id;
        _user->name = response.name;
        _user->password = password;
    }

    return response.error;
}

