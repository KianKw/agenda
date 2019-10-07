#include "../include/Date.hpp"

Date::Date()
    :m_year(0), m_month(0)
    ,m_day(0), m_hour(0)
    ,m_minute(0) {}
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute)
    :m_year(t_year)
    ,m_month(t_month)
    ,m_day(t_day)
    ,m_hour(t_hour)
    ,m_minute(t_minute) {}

bool IsNumb(char c) {
    return (('0' <= c) && (c <= '9'));
}
int FourBitToNumb(char c1, char c2, char c3, char c4) {
    return ((c1 - '0') * 1000 + (c2 - '0') * 100
           +(c3 - '0') * 10 + (c4 - '0'));
}
int TwoBitToNumb(char c1, char c2) {
    return ((c1 - '0') * 10 + (c2 - '0'));
}

Date::Date(const std::string &dateString) {
    m_year   = 0;
    m_month  = 0;
    m_day    = 0;
    m_hour   = 0;
    m_minute = 0;
    if ((dateString.size() != 16)
      ||(dateString[4] != '-')
      ||(dateString[7] != '-')
      ||(dateString[10] != '/')
      ||(dateString[13] != ':')) {
        return;
    }
    int arr[] = {0,1,2,3, 5,6, 8,9, 11,12, 14,15};
    for (int i = 0; i < 12; i++) {
        if (!IsNumb(dateString[arr[i]])) {
            return;
        }
    }
    m_year   =FourBitToNumb(dateString[0], dateString[1],
                            dateString[2], dateString[3]);
    m_month  = TwoBitToNumb(dateString[5], dateString[6]);
    m_day    = TwoBitToNumb(dateString[8], dateString[9]);
    m_hour   = TwoBitToNumb(dateString[11],dateString[12]);
    m_minute = TwoBitToNumb(dateString[14],dateString[15]);
}

int Date::getYear(void) const {
    return m_year;
}
void Date::setYear(const int t_year) {
    m_year = t_year;
}
int Date::getMonth(void) const {
    return m_month;
}
void Date::setMonth(const int t_month) {
    m_month = t_month;
}
int Date::getDay(void) const {
    return m_day;
}
void Date::setDay(const int t_day) {
    m_day = t_day;
}
int Date::getHour(void) const {
    return m_hour;
}
void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}
int Date::getMinute(void) const {
    return m_minute;
}
void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}
bool IsLeapYear(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}
bool Date::isValid(const Date &t_date) {
    if(!(0 <= t_date.m_minute && t_date.m_minute <= 59)) {
        return false;
    } else if (!(0 <= t_date.m_hour && t_date.m_hour <= 23)) {
        return false;
    } else if (!(0 <= t_date.m_month && t_date.m_month <= 12)) {
        return false;
    } else if (!(0 <= t_date.m_year && t_date.m_year <= 9999)){
        return false;
    } else {
        if ((t_date.m_month == 1)
          ||(t_date.m_month == 3)
          ||(t_date.m_month == 5)
          ||(t_date.m_month == 7)
          ||(t_date.m_month == 8)
          ||(t_date.m_month == 10)
          ||(t_date.m_month == 12)) {
            if(!(0 <= t_date.m_day && t_date.m_day <= 31))
                return false;
        } else if ((t_date.m_month == 4)
                 ||(t_date.m_month == 6)
                 ||(t_date.m_month == 9)
                 ||(t_date.m_month == 11)) {
            if(!(0 <= t_date.m_day && t_date.m_day <= 30))
                return false;
        } else {
            if(IsLeapYear(t_date.m_year)) {
                if(!(0 <= t_date.m_day && t_date.m_day <= 29))
                    return false;
            } else {
                if(!(0 <= t_date.m_day && t_date.m_day <= 28))
                    return false;
            }
        }
    }
    return true;
}
Date Date::stringToDate(const std::string &t_dateString) {
    Date temp1(t_dateString);
    Date temp2;
    if(isValid(temp1))
        return temp1;
    else
        return temp2;
}
std::string FourBitToString(int n) {
    std::string temp = "";
    if (n < 0) {
        temp = "0000";
    } else if (0 <= n && n <= 9) {
        temp = "000";
        temp += '0' + n;
    } else if (10 <= n && n <= 99) {
        temp = "00";
        temp += '0' + n / 10;
        temp += '0' + n % 10;
    } else if (100 <= n && n <= 999) {
        temp = "0";
        temp += '0' + n / 100;
        temp += '0' + (n / 10) % 10;
        temp += '0' + n % 10;
    } else if (1000 <= n && n <= 9999) {
        temp += '0' + n / 1000;
        temp += '0' + (n / 100) % 10;
        temp += '0' + (n / 10) % 10;
        temp += '0' + n % 10;
    } else {
        temp = "0000";
    }
    return temp;
}
std::string TwoBitToString(int n) {
    std::string temp = "";
    if (n < 0) {
        temp = "00";
    } else if (0 <= n && n <= 9) {
        temp = "0";
        temp += '0' + n;
    } else if (10 <= n && n <= 99) {
        temp += '0' + n / 10;
        temp += '0' + n % 10;
    } else {
        temp = "00";
    }
    return temp;
}
std::string Date::dateToString(const Date &t_date) {
    std::string temp = "";
    if(!isValid(t_date))
        temp = "0000-00-00/00:00";
    else {
        temp = FourBitToString(t_date.m_year);
        temp += '-';
        temp += TwoBitToString(t_date.m_month);
        temp += '-';
        temp += TwoBitToString(t_date.m_day);
        temp += '/';
        temp += TwoBitToString(t_date.m_hour);
        temp += ':';
        temp += TwoBitToString(t_date.m_minute);
    }
    return temp;
}
Date &Date::operator=(const Date &t_date) {
    this->m_year   = t_date.m_year  ;
    this->m_month  = t_date.m_month ;
    this->m_day    = t_date.m_day   ;
    this->m_hour   = t_date.m_hour  ;
    this->m_minute = t_date.m_minute;
}
bool Date::operator==(const Date &t_date) const {
    return ((this->m_year   == t_date.m_year  )
          &&(this->m_month  == t_date.m_month )
          &&(this->m_day    == t_date.m_day   )
          &&(this->m_hour   == t_date.m_hour  )
          &&(this->m_minute == t_date.m_minute));
}
bool Date::operator>(const Date &t_date) const {
    if (this->m_year > t_date.m_year) {
        return true;
    } else if (this->m_year == t_date.m_year) {
        if (this->m_month  > t_date.m_month) {
            return true;
        } else if (this->m_month == t_date.m_month) {
            if (this->m_day > t_date.m_day) {
                return true;
            } else if (this->m_day == t_date.m_day) {
                if (this->m_hour > t_date.m_hour) {
                    return true;
                } else if (this->m_hour == t_date.m_hour) {
                    if (this->m_minute > t_date.m_minute) {
                        return true;
                    } else
                        return false;
                } else
                    return false;
            } else
                return false;
        } else
            return false;
    } else
        return false;
}
bool Date::operator<(const Date &t_date) const {
    return (!(*this > t_date) && !(*this == t_date));
}
bool Date::operator>=(const Date &t_date) const {
    return ((*this > t_date) || (*this == t_date));
}
bool Date::operator<=(const Date &t_date) const {
    return !(*this > t_date);
}
