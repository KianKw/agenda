#include "../include/AgendaService.hpp"

AgendaService::AgendaService() {
    startAgenda();
}
AgendaService::~AgendaService() {
    quitAgenda();
}
bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    auto UserMatchPassword = [=](const User &t_user) -> bool {
        return (t_user.getName() == userName)
            && (t_user.getPassword() == password);
    };
    std::list<User> temp = m_storage->queryUser(UserMatchPassword);
    return temp.size();
}
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                const std::string &email, const std::string &phone) {
    User temp(userName, password, email, phone);
    m_storage->createUser(temp);
    auto UserAllMatch = [=](const User &t_user) -> bool {
        return ((t_user.getName() == userName)
            &&  (t_user.getPassword() == password)
            &&  (t_user.getEmail() == email)
            &&  (t_user.getPhone() == phone));
    };
    return m_storage->queryUser(UserAllMatch).size();
}
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    auto UserMatchPassword = [=](const User &t_user) -> bool {
        return (t_user.getName() == userName)
            && (t_user.getPassword() == password);
    };
    return m_storage->deleteUser(UserMatchPassword);
}
std::list<User> AgendaService::listAllUsers(void) const {
    auto TrueMatch = [](const User &) -> bool {return true;};
    return m_storage->queryUser(TrueMatch);
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                                  const std::string &startDate, const std::string &endDate,
                                  const std::vector<std::string> &participator) {
    Date start_date(startDate);
    Date end_date(endDate);
    if(start_date >= end_date) {
        return false;
    } else {
        Meeting temp(userName, participator, start_date, end_date, title);
        m_storage->createMeeting(temp);
        auto MeetingAllMatch = [=](const Meeting &t_meeting) -> bool {
            return ((t_meeting.getSponsor() == userName)
                &&  (t_meeting.getTitle() == title)
                &&  (t_meeting.getStartDate() == start_date)
                &&  (t_meeting.getEndDate() == end_date)
                &&  (t_meeting.getParticipator() == participator));
        };
        return m_storage->queryMeeting(MeetingAllMatch).size();
    }
}
bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator) {
    auto SponsorMatchTitle = [=](const Meeting &t_meeting) -> bool {
        return (t_meeting.getTitle() == title)
            && (t_meeting.getSponsor() == userName);
    };
    auto AddParticipator = [=](Meeting &temp_meeting) -> void {
        temp_meeting.addParticipator(participator);
    };
    return m_storage->updateMeeting(SponsorMatchTitle, AddParticipator);
}
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator) {
    auto SponsorMatchTitle = [=](const Meeting &t_meeting) -> bool {
        return (t_meeting.getTitle() == title)
            && (t_meeting.getSponsor() == userName)
            && (t_meeting.isParticipator(participator));
    };
    auto RemoveParticipator = [=](Meeting &temp_meeting) -> void {
        temp_meeting.removeParticipator(participator);
    };
    return m_storage->updateMeeting(SponsorMatchTitle, RemoveParticipator);
}
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    auto ParticipatorMatchTitle = [=](const Meeting &t_meeting) -> bool {
        return (t_meeting.getTitle() == title)
            && (t_meeting.isParticipator(userName));
    };
    auto RemoveParticipator = [=](Meeting &temp_meeting) -> void {
        temp_meeting.removeParticipator(userName);
    };
    return m_storage->updateMeeting(ParticipatorMatchTitle, RemoveParticipator);
}

/**
* @param userName as a sponsor OR a participator
*/
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                              const std::string &title) const {
    auto AllNameMatchTitle = [=](const Meeting &t_meeting) -> bool {
        return ((t_meeting.getTitle() == title)
            &&  ((t_meeting.getSponsor() == userName)
                ||(t_meeting.isParticipator(userName))));
        // relate question No.1, which is in meeting.cpp/////////////////////////////////////////////////////////////
    };
    return m_storage->queryMeeting(AllNameMatchTitle);
}
/**
* @param startDate time interval's start date
* @param endDate time interval's end date
*/
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &startDate,
                                               const std::string &endDate) const {
    Date start_date(startDate);
    Date end_date(endDate);
    if(start_date >= end_date) {
        std::list<Meeting> empty_list;
        return empty_list;
    } else {
        auto NameMatchTime = [=](const Meeting &t_meeting) -> bool {
        return ((t_meeting.isParticipator(userName)||(t_meeting.getSponsor() == userName))
#if 0
            &&  (t_meeting.getStartDate() > start_date && t_meeting.getEndDate() < end_date));/////////////////////////////whether the time match is correct !important. I have a deep suspicion of it.
#else
            &&   (((t_meeting.getStartDate() < start_date) && (t_meeting.getEndDate() > end_date))
                ||((t_meeting.getStartDate() >= start_date) && (t_meeting.getStartDate() < end_date))));
#endif
        };
        return m_storage->queryMeeting(NameMatchTime);
    }
}
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    auto MatchName = [=](const Meeting &t_meeting) -> bool {
        return ((t_meeting.isParticipator(userName))
            ||  (t_meeting.getSponsor() == userName));
    };
    return m_storage->queryMeeting(MatchName);
}
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    auto MatchSponsor = [=](const Meeting &t_meeting) -> bool {
        return t_meeting.getSponsor() == userName;
    };
    return m_storage->queryMeeting(MatchSponsor);
}
std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const {
    auto MatchParticipator = [=](const Meeting &t_meeting) -> bool {
        return t_meeting.isParticipator(userName);
    };
    return m_storage->queryMeeting(MatchParticipator);
}
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    auto SponsorMatchTitle = [=](const Meeting &t_meeting) -> bool {
        return ((t_meeting.getTitle() == title)
            &&  (t_meeting.getSponsor() == userName));
    };
    return m_storage->deleteMeeting(SponsorMatchTitle);
}
bool AgendaService::deleteAllMeetings(const std::string &userName) {
    auto MatchSponsor = [=](const Meeting &t_meeting) -> bool {
        return t_meeting.getSponsor() == userName;
    };
    return m_storage->deleteMeeting(MatchSponsor);
}
void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}
void AgendaService::quitAgenda(void) {
    m_storage->sync();
}
