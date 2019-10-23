#include "../include/Storage.hpp"
#include "../include/Path.hpp"
#include <fstream>
#include <iostream>

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
    m_dirty = false;
    readFromFile();
}

User StringToUser(std::string user_str) {
    User temp;
    std::string buf;
    int i = 1;
    buf = user_str.at(i++);
    while (user_str.at(i) != '\"') {
        buf += user_str.at(i++);
    }
    temp.setName(buf);
    i += 3;
    buf = user_str.at(i++);
    while (user_str.at(i) != '\"') {
        buf += user_str.at(i++);
    }
    temp.setPassword(buf);
    i += 3;
    buf = user_str.at(i++);
    while (user_str.at(i) != '\"') {
        buf += user_str.at(i++);
    }
    temp.setEmail(buf);
    i += 3;
    buf = user_str.at(i++);
    while (user_str.at(i) != '\"') {
        buf += user_str.at(i++);
    }
    temp.setPhone(buf);
    return temp;
}
Meeting StringToMeeting(std::string meeting_str) {
    Meeting temp;
    std::string buf;
    std::string participator_str;
    std::vector<std::string> participator_temp;
    Date date_temp;
    int i = 1;
    buf = meeting_str.at(i++);
    while (meeting_str.at(i) != '\"') {
        buf += meeting_str.at(i++);
    }
    temp.setSponsor(buf);
    i += 3;
    buf = meeting_str.at(i++);
    while (meeting_str.at(i) != '\"') {
        buf += meeting_str.at(i++);
    }
    participator_str = buf.at(0);
    for (int j = 1; j < buf.size(); ++j) {
        if (buf.at(j) == '&') {
            participator_temp.push_back(participator_str);
            participator_str = buf.at(++j);
        } else {
            participator_str += buf.at(j);
        }
    }
    participator_temp.push_back(participator_str);
    temp.setParticipator(participator_temp);
    i += 3;
    buf = meeting_str.at(i++);
    while (meeting_str.at(i) != '\"') {
        buf += meeting_str.at(i++);
    }
    temp.setStartDate(date_temp.stringToDate(buf));
    i += 3;
    buf = meeting_str.at(i++);
    while (meeting_str.at(i) != '\"') {
        buf += meeting_str.at(i++);
    }
    temp.setEndDate(date_temp.stringToDate(buf));
    i += 3;
    buf = meeting_str.at(i++);
    while (meeting_str.at(i) != '\"') {
        buf += meeting_str.at(i++);
    }
    temp.setTitle(buf);
    return temp;
}
bool Storage::readFromFile(void) {
    std::ifstream user_read(Path::userPath, std::ofstream::in);
    if (!user_read)
        return false;
    std::ifstream meeting_read(Path::meetingPath, std::ofstream::in);
    if (!meeting_read) {
        user_read.close();
        return false;
    }
    std::string buf;
    while (user_read.peek() != EOF) {
        getline(user_read, buf);
        m_userList.push_front(StringToUser(buf));
    }
    while (meeting_read.peek() != EOF) {
        getline(meeting_read, buf);
        m_meetingList.push_front(StringToMeeting(buf));
    }
    user_read.close();
    meeting_read.close();
    return true;
}
bool IsStringLegal(std::string str) {
    for (std::string::size_type i = 0; i < str.size(); ++i) {
        if (str.at(i) == '\"' || str.at(i) == '&' || str.at(i) == ',')
            return false;
    }
    return true;
}
bool Storage::writeToFile(void) {
    if (!m_dirty) {
        // std::cout << "(!dirty)" << std::endl;
        return true;
    }
    bool flag = true;
    std::ofstream user_write(Path::userPath, std::ios::out|std::ios::trunc);
    if (!user_write.is_open()) {
        // std::cout << "user_write.is_open() error" << std::endl;
        return false;
    }
    for (std::list<User>::iterator it = m_userList.begin(); it != m_userList.end(); ++it) {
        if (IsStringLegal(it->getName()))
            user_write << "\"" << it->getName() << "\",";
        else
            flag = false;
        if (IsStringLegal(it->getPassword()))
            user_write << "\"" << it->getPassword() << "\",";
        else
            flag = false;
        if (IsStringLegal(it->getEmail()))
            user_write << "\"" << it->getEmail() << "\",";
        else
            flag = false;
        if (IsStringLegal(it->getPhone()))
            user_write << "\"" << it->getPhone() << "\"\n";
        else
            flag = false;
    }
    user_write.close();
    std::ofstream meeting_write(Path::meetingPath);
    if (!meeting_write.is_open()) {
        // std::cout << "meeting_write.is_open() error" << std::endl;
        return false;
    }
    for (std::list<Meeting>::iterator it = m_meetingList.begin(); it != m_meetingList.end(); ++it) {
        if (IsStringLegal(it->getSponsor()))
            meeting_write << "\"" << it->getSponsor() << "\",\"";
        else
            flag = false;
        std::vector<std::string> temp = it->getParticipator();
        for (std::vector<std::string>::size_type i = 0; i < temp.size(); ++i) {
            if (IsStringLegal(temp[i]))
                meeting_write << temp[i];
            else
                flag = false;
            if (i != temp.size() - 1)
                meeting_write << '&';
            else
                meeting_write << "\",";
        }
        if (IsStringLegal(it->getStartDate().dateToString(it->getStartDate())))
            meeting_write << "\"" << it->getStartDate().dateToString(it->getStartDate()) << "\",";
        else
            flag = false;
        if (IsStringLegal(it->getEndDate().dateToString(it->getEndDate())))
            meeting_write << "\"" << it->getEndDate().dateToString(it->getEndDate()) << "\",";
        else
            flag = false;
        if (IsStringLegal(it->getTitle()))
            meeting_write << "\"" << it->getTitle() << "\"\n";
        else
            flag = false;
    }
    meeting_write.close();
    m_dirty = false;
    return flag;
}
std::shared_ptr<Storage> Storage::getInstance(void) {
    if (nullptr == m_instance) {
        m_instance = std::shared_ptr<Storage>(new Storage());
    }
    return m_instance;
}
Storage::~Storage() {
    sync();
    m_instance = nullptr;
}
void Storage::createUser(const User &t_user) {
    m_dirty = true;
    m_userList.push_front(t_user);
}
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
    std::list<User> temp;
    for (std::list<User>::const_iterator it = m_userList.begin(); it != m_userList.end(); it++) {
        if (filter(*it))
            temp.push_front(*it);
    }
    return temp;
}
int Storage::updateUser(std::function<bool(const User &)> filter,
                        std::function<void(User &)> switcher) {
    int num = 0;
    for (std::list<User>::iterator it = m_userList.begin(); it != m_userList.end(); ++it) {
        if (filter(*it)) {
            switcher(*it);
            ++num;
        }
    }
    if (num)
        m_dirty = true;
    return num;
}
int Storage::deleteUser(std::function<bool(const User &)> filter) {
    int num = 0;
    for (std::list<User>::iterator it = m_userList.begin(); it != m_userList.end(); ++it) {
        if (filter(*it)) {
            auto temp = it;
            it = m_userList.erase(temp);
            ++num;
            continue;
        }
    }
    if (num)
        m_dirty = true;
    return num;
}
void Storage::createMeeting(const Meeting &t_meeting) {
    m_dirty = true;
    m_meetingList.push_front(t_meeting);
}
std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
    std::list<Meeting> temp;
    for (std::list<Meeting>::const_iterator it = m_meetingList.begin(); it != m_meetingList.end(); ++it) {
        if (filter(*it))
            temp.push_front(*it);
    }
    return temp;
}
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                           std::function<void(Meeting &)> switcher) {
    int num = 0;
    for (std::list<Meeting>::iterator it = m_meetingList.begin(); it != m_meetingList.end(); ++it) {
        if (filter(*it)) {
            switcher(*it);
            ++num;
        }
    }
    if (num)
        m_dirty = true;
    return num;
}
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
    int num = 0;
    for (std::list<Meeting>::iterator it = m_meetingList.begin(); it != m_meetingList.end(); ++it) {
        if (filter(*it)) {
            auto temp = it;
            it = m_meetingList.erase(temp);
            ++num;
            continue;
        }
    }
    if (num)
        m_dirty = true;
    return num;
}
bool Storage::sync(void) {
    if (m_dirty) {
        return writeToFile();
    }
    return true;
}
