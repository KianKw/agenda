#include "../include/User.hpp"
#include <string>

using std::string;

User::User(const string &t_userName,
           const string &t_userPassword,
           const string &t_userEmail,
           const string &t_userPhone) :
     m_name(t_userName),
     m_password(t_userPassword),
     m_email(t_userEmail),
     m_phone(t_userPhone) {}

User::User(const User &t_user) {
     this->m_name = t_user.m_name;
     this->m_password = t_user.m_password;
     this->m_email = t_user.m_email;
     this->m_phone = t_user.m_phone;
}

std::string User::getName() const {
     return m_name;
}

void User::setName(const string &t_name) {
     m_name = t_name;
}

std::string User::getPassword() const {
     return m_password;
}

void User::setPassword(const string &t_password) {
     m_password = t_password;
}

std::string User::getEmail() const {
     return m_email;
}

void User::setEmail(const string &t_email) {
     m_email = t_email;
}

std::string User::getPhone() const {
     return m_phone;
}

void User::setPhone(const string &t_phone) {
     m_phone = t_phone;
}
