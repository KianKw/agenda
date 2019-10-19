#include "../include/AgendaService.hpp"

#include <iostream>

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
bool AgendaService::userRegister(const std::string &userName, 
                                 const std::string &password,
                                 const std::string &email, 
                                 const std::string &phone) {
    auto MatchUserName = [=](const User &t_user) ->bool {
        return t_user.getName() == userName;
    };
    std::list<User> temp_user_list = m_storage->queryUser(MatchUserName);
    if (temp_user_list.size() == 0) {
        User temp(userName, password, email, phone);
        m_storage->createUser(temp);
        auto UserAllMatch = [=](const User &t_user) -> bool {
            return ((t_user.getName() == userName)
                &&  (t_user.getPassword() == password)
                &&  (t_user.getEmail() == email)
                &&  (t_user.getPhone() == phone));
        };
        return m_storage->queryUser(UserAllMatch).size();
    } else {
        // std::cout << "error: The name has exited" << std::endl;
        return false;
    }
}
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    auto UserMatchPassword = [=](const User &t_user) -> bool {
        return (t_user.getName() == userName)
            && (t_user.getPassword() == password);
    };
    auto MatchSponsor = [=](const Meeting &t_meeting) -> bool {
        return (t_meeting.getSponsor() == userName);
    };
    m_storage->deleteMeeting(MatchSponsor);

    auto MatchParticipator = [=](const Meeting &t_meeting) -> bool {
        return t_meeting.isParticipator(userName);
    };
    auto RemoveParticipator = [=](Meeting &temp_meeting) -> void {
        temp_meeting.removeParticipator(userName);
    };
    m_storage->updateMeeting(MatchParticipator, RemoveParticipator);
    auto MatchNullMeeting = [](const Meeting &t_meeting) -> bool {
        return (t_meeting.getParticipator().size() == 0);
    };
    m_storage->deleteMeeting(MatchNullMeeting);

    return m_storage->deleteUser(UserMatchPassword);
}
std::list<User> AgendaService::listAllUsers(void) const {
    auto TrueMatch = [](const User &) -> bool {return true;};
    return m_storage->queryUser(TrueMatch);
}
bool AgendaService::createMeeting(const std::string &userName, 
                                  const std::string &title,
                                  const std::string &startDate, 
                                  const std::string &endDate,
                                  const std::vector<std::string> &participator) {
    if (participator.size() == 0) {
        // std::cout << "error: There is no participator" << std::endl;
        return false;
    }
    // Make sure that all attendees have registered
    auto TrueMatch = [](const User &) -> bool {return true;};
    std::list<User> user_all_list = m_storage->queryUser(TrueMatch);
    bool isUser = false;
    for (std::list<User>::iterator it = user_all_list.begin(); it != user_all_list.end(); ++it) {
        if (userName == it->getName())
            isUser = true;
    }
    if (!isUser) {
        // std::cout << "Sponsor have not registered" << std::endl;
        return false;
    }
    for (std::vector<User>::size_type i = 0;i < participator.size(); ++i) {
        isUser = false;
        for (std::list<User>::iterator it = user_all_list.begin(); it != user_all_list.end(); ++it) {
            if (participator[i] == it->getName())
                isUser = true;
        }
        if (!isUser) {
            // std::cout << "Participator have not registered" << std::endl;
            return false;
        }
    }
    // Make sure the name of attendees are not repeated
    for (std::vector<User>::size_type i = 0; i < participator.size(); ++i) {
        if (userName == participator[i]) {
            // std::cout << "Sponsor is in participator list" << std::endl;
            return false;
        }
    }
    for (std::vector<User>::size_type i = 0; i < participator.size(); ++i) {
        for (std::vector<User>::size_type j = i + 1; j < participator.size(); ++j)
            if (participator[j] == participator[i]) {
                // std::cout << "There are two participators have the same name" << std::endl;
                return false;
            }
    }
    // Make sure the title is unique
    auto MatchTitle = [=](const Meeting &t_meeting) -> bool {
        return (t_meeting.getTitle() == title);
    };
    std::list<Meeting> Retitle_meeting = m_storage->queryMeeting(MatchTitle);
    if (Retitle_meeting.size()) {
        // std::cout << "The title has exited" << std::endl;
        return false;
    }
    // Check whether the date is corect
    Date start_date(startDate);
    Date end_date(endDate);
    if ((start_date.getYear() == 0)
        ||(end_date.getYear() == 0)
        ||(!start_date.isValid(start_date))
        ||(!end_date.isValid(end_date))
        ||(start_date >= end_date)) {
        return false;
    }
    std::list<Meeting> sponsor_busy = meetingQuery(userName, startDate, endDate);
    for (std::list<Meeting>::iterator it = sponsor_busy.begin(); it != sponsor_busy.end(); ++it) {
        if (it->getStartDate() > start_date && it->getStartDate() < end_date)
            return false;
        if (it->getEndDate() > start_date && it->getEndDate() < end_date)
            return false;
        if (it->getStartDate() <= start_date && it->getEndDate() >= end_date)
            return false;
    }
    for (std::vector<std::string>::size_type i = 0; i < participator.size(); ++i) {
        std::list<Meeting> participator_busy = meetingQuery(participator[i], startDate, endDate);
        for (std::list<Meeting>::iterator it = participator_busy.begin(); it != participator_busy.end(); ++it) {
            if (it->getStartDate() > start_date && it->getStartDate() < end_date)
                return false;
            if (it->getEndDate() > start_date && it->getEndDate() < end_date)
                return false;
            if (it->getStartDate() <= start_date && it->getEndDate() >= end_date)
                return false;
        }
    }
#if 0
    // check whether the sponsor is busy
    auto SponsorBusy = [=](const Meeting &t_meeting) -> bool {
        if ((t_meeting.getSponsor() == userName || t_meeting.isParticipator(userName))
         && ((t_meeting.getStartDate() < start_date && t_meeting.getEndDate() > end_date)
            ||(t_meeting.getStartDate() >= start_date && t_meeting.getStartDate() < end_date)))
            return true;
        return false;
    };
    std::list<Meeting> sponsor_busy_meeting = m_storage->queryMeeting(SponsorBusy);
    if (sponsor_busy_meeting.size()) {
        return false;
    }
    // check whether the participator is busy
    for (std::vector<std::string>::size_type i = 0; i < participator.size(); ++i) {
        auto ParticipatorBusy = [=](const Meeting &t_meeting) -> bool {
            if ((t_meeting.getSponsor() == participator[i] || t_meeting.isParticipator(participator[i]))
             && ((t_meeting.getStartDate() < start_date && t_meeting.getEndDate() > end_date)
                ||(t_meeting.getStartDate() >= start_date && t_meeting.getStartDate() < end_date)))
                return true;
            return false;
        };
        std::list<Meeting> participator_busy_meeting = m_storage->queryMeeting(ParticipatorBusy);
        if (participator_busy_meeting.size()) {
            return false;
        }
    }
#endif
    // create meeting
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
bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator) {
    auto TrueMatch = [](const User &) -> bool {return true;};
    std::list<User> user_all_list = m_storage->queryUser(TrueMatch);
    bool isUser = false;
    for (std::list<User>::iterator it = user_all_list.begin(); it != user_all_list.end(); ++it) {
        if (participator == it->getName())
            isUser = true;
    }
    if (!isUser) {
        // std::cout << "Participator have not registered" << std::endl;
        return false;
    }
    // Make sure the names of attendees are not repeated
    if (userName == participator) {
        // std::cout << "Sponsor is in participator list" << std::endl;
        return false;
    }
#if 0
    
    
    if (t_meeting.size() == 1) {
        std::list<Meeting> t_meeting_list = meetingQuery(participator,
                                                        t_meeting.front().getStartDate().dateToString(t_meeting.front().getStartDate()),
                                                        t_meeting.front().getEndDate().dateToString(t_meeting.front().getEndDate()));
        if (t_meeting_list.size() != 0) {
            // std::cout << "The particitor is busy" << std::endl;
            return false;
        }
    }
#endif
    auto SponsorMatchTitle = [=](const Meeting &t_meeting) -> bool {
        return ((t_meeting.getTitle() == title)
            &&  (t_meeting.getSponsor() == userName)
            &&  (!t_meeting.isParticipator(participator)));
    };
    std::list<Meeting> t_meeting = m_storage->queryMeeting(SponsorMatchTitle);
    Date start_date = t_meeting.front().getStartDate();
    Date end_date = t_meeting.front().getEndDate();
    std::list<Meeting> participator_busy = meetingQuery(participator, start_date.dateToString(start_date), end_date.dateToString(end_date));
    for (std::list<Meeting>::iterator it = participator_busy.begin(); it != participator_busy.end(); ++it) {
        if (it->getStartDate() > start_date && it->getStartDate() < end_date)
            return false;
        if (it->getEndDate() > start_date && it->getEndDate() < end_date)
            return false;
        if (it->getStartDate() <= start_date && it->getEndDate() >= end_date)
            return false;
    }
    auto AddParticipator = [=](Meeting &temp_meeting) -> void {
        temp_meeting.addParticipator(participator);
    };
    return m_storage->updateMeeting(SponsorMatchTitle, AddParticipator);
}
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator) {
    auto SponsorMatchTitle2 = [=](const Meeting &t_meeting) -> bool {
        return (t_meeting.getTitle() == title)
            && (t_meeting.getSponsor() == userName)
            && (t_meeting.isParticipator(participator));
    };
    auto RemoveParticipator = [=](Meeting &temp_meeting) -> void {
        temp_meeting.removeParticipator(participator);
    };
    int count = m_storage->updateMeeting(SponsorMatchTitle2, RemoveParticipator);
    auto MatchNullMeeting = [](const Meeting &t_meeting) -> bool {
        return (t_meeting.getParticipator().size() == 0);
    };
    m_storage->deleteMeeting(MatchNullMeeting);
    return count;
}
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    auto ParticipatorMatchTitle = [=](const Meeting &t_meeting) -> bool {
        if (t_meeting.getTitle() == title) {
            if (t_meeting.getSponsor() == userName)
                return false;
            for (std::vector<Meeting>::size_type i = 0; i < t_meeting.getParticipator().size(); ++i) {
                if (t_meeting.getParticipator()[i] == userName)
                    return true;
            }
        }
        return false;
    };
    auto RemoveParticipator = [=](Meeting &t_meeting) -> void {
        t_meeting.removeParticipator(userName);
    };
    int count = m_storage->updateMeeting(ParticipatorMatchTitle, RemoveParticipator);
    auto MatchNullMeeting = [](const Meeting &t_meeting) -> bool {
        return (t_meeting.getParticipator().size() == 0);
    };
    m_storage->deleteMeeting(MatchNullMeeting);
    return count;
#if 0
    auto ParticipatorMatchTitle = [=](const Meeting &t_meeting) -> bool {
        if (t_meeting.getSponsor() == userName) {
            return false;
        } else {
            return (t_meeting.getTitle() == title)
                && (t_meeting.isParticipator(userName))
                && (t_meeting.getSponsor() != userName);
        }
    };
    auto RemoveParticipator = [=](Meeting &temp_meeting) -> void {
        temp_meeting.removeParticipator(userName);
    };
    int count = m_storage->updateMeeting(ParticipatorMatchTitle, RemoveParticipator);
    auto MatchNullMeeting = [](const Meeting &t_meeting) -> bool {
        return (t_meeting.getParticipator().size() == 0);
    };
    m_storage->deleteMeeting(MatchNullMeeting);
    return count;
#endif
}
std::list<Meeting> AgendaService::meetingQuery (const std::string &userName,
                                                const std::string &title) const {
    auto AllNameMatchTitle = [=](const Meeting &t_meeting) -> bool {
        return ((t_meeting.getTitle() == title)
            &&  ((t_meeting.getSponsor() == userName)
                ||(t_meeting.isParticipator(userName))));
    };
    return m_storage->queryMeeting(AllNameMatchTitle);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &startDate,
                                               const std::string &endDate) const {
    std::list<Meeting> empty_list;
    Date start_date(startDate);
    Date end_date(endDate);
    if ((start_date.getYear() == 0)
        ||(end_date.getYear() == 0)
        ||(!start_date.isValid(start_date))
        ||(!end_date.isValid(end_date))
        ||(start_date >= end_date)) {
        return empty_list;
    } else {
        auto NameMatchTime = [=](const Meeting &t_meeting) -> bool {
            return ((t_meeting.isParticipator(userName)||(t_meeting.getSponsor() == userName)))
                && (((t_meeting.getStartDate() < start_date) && (t_meeting.getEndDate() >= start_date))
                    ||(t_meeting.getStartDate() >= start_date) && (t_meeting.getStartDate() <= end_date));
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
        return (t_meeting.getSponsor() == userName);
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
        return (t_meeting.getSponsor() == userName);
    };
    return m_storage->deleteMeeting(MatchSponsor);
}
void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}
void AgendaService::quitAgenda(void) {
    m_storage->sync();
}
