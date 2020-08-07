#include "../include/Meeting.hpp"

using std::vector;
using std::string;

Meeting::Meeting(const string &t_sponsor,
        const vector<string> &t_participator,
        const Date &t_startTime,
        const Date &t_endTime,
        const string &t_title) :
        m_sponsor(t_sponsor),
        m_participators(t_participator),
        m_startDate(t_startTime),
        m_endDate(t_endTime),
        m_title(t_title) {}

Meeting::Meeting(const Meeting &t_meeting) {
    this->m_sponsor = t_meeting.m_sponsor;
    this->m_participators = t_meeting.m_participators;
    this->m_startDate = t_meeting.m_startDate;
    this->m_endDate = t_meeting.m_endDate;
    this->m_title = t_meeting.m_title;
}

string Meeting::getSponsor(void) const {
    return m_sponsor;
}

void Meeting::setSponsor(const string &t_sponsor) {
    m_sponsor = t_sponsor;
}

vector<string> Meeting::getParticipator(void) const {
    return m_participators;
}

void Meeting::setParticipator(const vector<string> &t_participators) {
    m_participators = t_participators;
}

void Meeting::addParticipator(const string &t_participator) {
    m_participators.insert(m_participators.begin(), t_participator);
}

void Meeting::removeParticipator(const string &t_participator) {
    for (int i = 0; i < m_participators.size(); ++i) {
        if(m_participators.at(i) == t_participator) {
            m_participators.erase(std::begin(m_participators) + i);
        }
    }
}

Date Meeting::getStartDate(void) const {
    return m_startDate;
}

void Meeting::setStartDate(const Date &t_startTime) {
    m_startDate = t_startTime;
}

Date Meeting::getEndDate(void) const {
    return m_endDate;
}

void Meeting::setEndDate(const Date &t_endTime) {
    m_endDate = t_endTime;
}

string Meeting::getTitle(void) const {
    return m_title;
}

void Meeting::setTitle(const string &t_title) {
    m_title = t_title;
}

bool Meeting::isParticipator(const string &t_username) const {
    for (int i = 0; i < m_participators.size(); ++i) {
        if(m_participators.at(i) == t_username) {
            return true;
        }
    }
    return false;
}
