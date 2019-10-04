#include "Agenda/include/Storage.hpp"
#include <fstream>
#include "Agenda/include/Path.hpp"

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
    std::string participator_str;  // 每一个参与者str
    std::vector<std::string> participators_temp;  // 参与者str形成的向量
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
        if(buf.at(j) == '&') {
            participators_temp.push_back(participator_str);
            participator_str = buf.at(++j);
        }
        else {
            participator_str += buf.at(j);
        }
    }
    participators_temp.push_back(participator_str);
    temp.setParticipator(participators_temp);
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
    std::ifstream user_read(Path::userPath,std::ofstream::in);
    if(!user_read)
        return false;
    std::ifstream meeting_read(Path::meetingPath,std::ofstream::in);
    if(!meeting_read) {
        user_read.close();
        return false;
    }
    std::string buf;
    while (user_read.peek() != EOF) {
        user_read >> buf;
        m_userList.push_front(StringToUser(buf));
    }
    while (meeting_read.peek() != EOF) {
        meeting_read >> buf;
        m_meetingList.push_front(StringToMeeting(buf));
    }
    user_read.close();
    meeting_read.close();
    return true;
}
bool Storage::writeToFile(void) {
    if(!m_dirty)
        return true;
    std::ofstream user_write(Path::userPath,std::ofstream::trunc);
    if(!user_write)
        return false;
    std::ofstream meeting_write(Path::meetingPath,std::ofstream::trunc);
    if(!meeting_write) {
        user_write.close();
        return false;
    }
    std::string buf;
    for (std::list<User>::iterator it = m_userList.begin(); it != m_userList.end(); it++) {
        buf = "\"";
        buf += it->getName();buf += "\",\"";
        buf += it->getPassword();buf += "\",\"";
        buf += it->getEmail();buf += "\",\"";
        buf += it->getPhone();buf += "\"\n";
        user_write << buf;
    }
    for (std::list<Meeting>::iterator it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
        buf = "\"";
        buf += it->getSponsor();buf += "\",\"";
        buf+= it->getParticipator().at(0);
        for (int i = 1; i < it->getParticipator().size();i++) {
            buf += "&";
            buf += it->getParticipator().at(i);
        } buf += "\",\"";
        buf += it->getStartDate().dateToString(it->getStartDate());buf += "\",\"";
        buf += it->getEndDate().dateToString(it->getEndDate());buf += "\"\n";
        buf += it->getTitle();buf += "\"\n";
        user_write << buf;
    }
    user_write.close();
    meeting_write.close();
    return true;
}
std::shared_ptr<Storage> Storage::getInstance(void) {
    if(nullptr == m_instance) {
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
        if(filter(*it))
            temp.push_front(*it);
    }
    return temp;
}
int Storage::updateUser(std::function<bool(const User &)> filter,
                        std::function<void(User &)> switcher) {
    m_dirty = true;
    int num = 0;
    for (std::list<User>::iterator it = m_userList.begin(); it != m_userList.end(); it++) {
        if(filter(*it)) {
            switcher(*it);
            ++num;
        }
    }
    return num;
}
int Storage::deleteUser(std::function<bool(const User &)> filter) {
    m_dirty = true;
    int num = 0;
    for (std::list<User>::iterator it = m_userList.begin(); it != m_userList.end(); it++) {
        if(filter(*it)) {
            it = m_userList.erase(it);
            ++num;
            continue;
        }
    }
    return num;
}
void Storage::createMeeting(const Meeting &t_meeting) {
    m_dirty = true;
    m_meetingList.push_front(t_meeting);
}
std::list<Meeting> Storage::queryMeeting(
        std::function<bool(const Meeting &)> filter) const {
    std::list<Meeting> temp;
    for (std::list<Meeting>::const_iterator it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
        if(filter(*it))
            temp.push_front(*it);
    }
    return temp;
}
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                           std::function<void(Meeting &)> switcher) {
    m_dirty = true;
    int num = 0;
    for (std::list<Meeting>::iterator it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
        if(filter(*it)) {
            switcher(*it);
            ++num;
        }
    }
    return num;
}
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
    m_dirty = true;
    int num = 0;
    for (std::list<Meeting>::iterator it = m_meetingList.begin(); it != m_meetingList.end(); it++) {
        if(filter(*it)) {
            it = m_meetingList.erase(it);
            ++num;
            continue;
        }
    }
    return num;
}
bool Storage::sync(void) {
    return writeToFile();
}
