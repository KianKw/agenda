#include "AgendaUI.hpp"
//#include "../include/AgendaUI.hpp"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;

using std::string;

AgendaUI::AgendaUI() {
    startAgenda();
}
[[noreturn]] void AgendaUI::OperationLoop(void) {
    while (1) {
        system("cls");
        string operation = getOperation();
        putchar(10);
        executeOperation(operation);
        putchar(10);
        system("pause");
    }
}
void AgendaUI::startAgenda(void) {
    m_agendaService.startAgenda();
}
void PrintBeforLoginOperations(void) {
    cout << "Action :" << endl;
    cout << "l    - log in Agenda by user name and password" << endl;
    cout << "r    - register an Agenda account" << endl;
    cout << "q    - quit Agenda" << endl;
}
void PrintAfterLoginOperations(void) {
    cout << "Action :" << endl;
    cout << "o    - log out Agenda" << endl;
    cout << "dc   - delete Agenda account" << endl;
    cout << "lu   - list all Agenda user" << endl;
    cout << "cm   - create a meeting" << endl;
    cout << "amp  - add meeting participator" << endl;
    cout << "rmp  - remove meeting participator" << endl;
    cout << "rqm  - request to quit meeting" << endl;
    cout << "la   - list all meetings" << endl;
    cout << "las  - list all sponsor meetings" << endl;
    cout << "lap  - list all participator meetings" << endl;
    cout << "qm   - query meeting by title" << endl;
    cout << "qt   - query meeting by time interval" << endl;
    cout << "dm   - delete meeting by title" << endl;
    cout << "da   - delete all meetings" << endl;
}
std::string AgendaUI::getOperation() {
    putchar(10);
    for (int i = 0; i < 47; i++)
        cout << '-';
    cout << "Agenda";
    for (int i = 0; i < 47; i++)
        cout << '-';
    putchar(10);
    if (m_userName == "") {
        PrintBeforLoginOperations();
        for (int i = 0; i < 100; i++)
            cout << '-';
        cout << "\n\nAgenda :~$ ";
    } else {
        PrintAfterLoginOperations();
        for (int i = 0; i < 100; i++)
            cout << '-';
        cout << "\n\nAgenda@" << m_userName << ":~# ";
    }
    string operation;
    cin >> operation;
    return operation;
}
bool AgendaUI::executeOperation(std::string t_operation) {
    if (t_operation == "l")
        userLogIn();
    else if (t_operation == "r")
        userRegister();
    else if (t_operation == "q")
        quitAgenda();
    else if (t_operation == "o")
        userLogOut();
    else if (t_operation == "dc")
        deleteUser();
    else if (t_operation == "lu")
        listAllUsers();
    else if (t_operation == "cm")
        createMeeting();
    else if (t_operation == "amp")
        addMeetingParticipator();
    else if (t_operation == "rmp")
        removeMeetingParticipator();
    else if (t_operation == "rqm")
        requestQuitMeeting();
    else if (t_operation == "la")
        listAllMeetings();
    else if (t_operation == "las")
        listAllSponsorMeetings();
    else if (t_operation == "lap")
        listAllParticipateMeetings();
    else if (t_operation == "qm")
        queryMeetingByTitle();
    else if (t_operation == "qt")
        queryMeetingByTimeInterval();
    else if (t_operation == "dm")
        deleteMeetingByTitle();
    else if (t_operation == "da")
        deleteAllMeetings();
    else {
        cout << "error: The operation does not exit!" << endl << endl;
        system("pause");
        OperationLoop();
    }
    return true;
}
void AgendaUI::userLogIn(void) {
    cout << "[log in] [username] [password]" << endl;
    cout << "[log in] ";
    cin >> m_userName >> m_userPassword;
    if (m_agendaService.userLogIn(m_userName, m_userPassword))
        cout << "[log in] succeed!" << endl;
    else {
        cout << "[log in] Password error or user doesn't exist!" << endl;
        m_userName = "";
        m_userPassword = "";
    }
}
void AgendaUI::userRegister(void) {
    string name, password, email, phone;
    cout << "[register] [user name] [password] [email] [phone]" << endl;
    cout << "[register] ";
    cin >> name >> password >> email >> phone;
    if (m_agendaService.userRegister(name, password, email, phone))
        cout << "[register] succeed!" << endl;
    else
        cout << "[register] This username has been registered!" << endl;
}
void AgendaUI::quitAgenda(void) {
    m_agendaService.quitAgenda();
}
void AgendaUI::userLogOut(void) {
    m_userName = "";
    m_userPassword = "";
}
void AgendaUI::deleteUser(void) {
    m_agendaService.deleteUser(m_userName, m_userPassword);
    m_userName = "";
    m_userPassword = "";
}
void AgendaUI::listAllUsers(void) {
    cout << "[list all users]" << endl;
    std::list<User> temp = m_agendaService.listAllUsers();
    printf ("\n%-30s %-30s %-30s\n", "name", "email", "phone");
    for (std::list<User>::const_iterator it = temp.begin(); it != temp.end(); ++it) {
        printf ("%-30s %-30s %-30s\n", it->getName().c_str(), it->getEmail().c_str(), it->getPhone().c_str());
    }
}
void AgendaUI::createMeeting(void) {
    cout << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    int num = 0;
    cin >> num;
    while (num <= 0) {
        cout << "[create meeting] error! The number of participators is illegal!" << endl;
        cout << "[create meeting] Please enter a positive integer!" << endl;
        cin >> num;
    }
    std::vector<string> participators;
    string temp;
    for (int i = 1; i <= num; ++i) {
        cout << "[create meeting] [please enter the participator " << i << "]" << endl;
        cout << "[create meeting] ";
        cin >> temp;
        participators.push_back(temp);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mmdd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    string title, start_date, end_date;
    cin >> title >> start_date >> end_date;
    if (m_agendaService.createMeeting(m_userName, title, start_date, end_date, participators))
        cout << "[create meeting] succeed!" << endl;
    else
        cout << "[create meeting] error!" << endl;
}
void AgendaUI::addMeetingParticipator(void) {
    cout << "[add meeting participator] [title] [participator]" << endl;
    cout << "[add meeting participator] ";
    string title, participator;
    cin >> title >> participator;
    if (m_agendaService.addMeetingParticipator(m_userName, title, participator))
        cout << "[add meeting participator] succeed!" << endl;
    else
        cout << "[add meeting participator] error!" << endl;
}
void AgendaUI::removeMeetingParticipator(void) {
    cout << "[remove meeting participator] [title] [participator]" << endl;
    cout << "[remove meeting participator] ";
    string title, participator;
    cin >> title >> participator;
    if (m_agendaService.removeMeetingParticipator(m_userName, title, participator))
        cout << "[remove meeting participator] succeed!" << endl;
    else
        cout << "[remove meeting participator] error!" << endl;
}
void AgendaUI::requestQuitMeeting(void) {
    cout << "[request to quit meeting] [title]" << endl;
    cout << "[request to quit meeting] ";
    string title, participator;
    cin >> title >> participator;
    if (m_agendaService.quitMeeting(m_userName, title))
        cout << "[request to quit meeting] succeed!" << endl;
    else
        cout << "[request to quit meeting] error!" <<  endl;
}
void AgendaUI::listAllMeetings(void) {
    cout << "[list all meetings]" << endl;
    std::list<Meeting> temp = m_agendaService.listAllMeetings(m_userName);
    printMeetings(temp);
}
void AgendaUI::listAllSponsorMeetings(void) {
    cout << "[list all sponsor meetings]" << endl;
    std::list<Meeting> temp = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(temp);
}
void AgendaUI::listAllParticipateMeetings(void) {
    cout << "[list all participator meetings]" << endl;
    std::list<Meeting> temp = m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(temp);
}
void AgendaUI::queryMeetingByTitle(void) {
    cout << "[query meeting] [title]" << endl;
    cout << "[query meeting] ";
    string title;
    cin >> title;
    std::list<Meeting> temp = m_agendaService.meetingQuery(m_userName, title);
    printMeetings(temp);
}
void AgendaUI::queryMeetingByTimeInterval(void) {
    cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] ";
    string start_date, end_date;
    cin >> start_date >> end_date;
        std::list<Meeting> temp = m_agendaService.meetingQuery(m_userName, start_date, end_date);
    printMeetings(temp);
}
void AgendaUI::deleteMeetingByTitle(void) {
    cout << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title))
        cout << "[delete meeting] succeed!" << endl;
    else
        cout << "[delete meeting] error!" << endl;
}
void AgendaUI::deleteAllMeetings(void) {
    if (m_agendaService.deleteAllMeetings(m_userName))
        cout << "[delete all meeting] succeed!" << endl;
    else
        cout << "[delete all meeting] error!" << endl;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    printf ("%-20s %-20s %-20s %-20s %-20s\n", "title", "sponsor", "start time", "end time", "participators");
    std::vector<std::string> participator;
    for (std::list<Meeting>::const_iterator it = t_meetings.begin(); it != t_meetings.end(); ++it) {
        printf ("%-20s %-20s %-20s %-20s ", it->getTitle().c_str(),
                                                it->getSponsor().c_str(),
                                                it->getStartDate().dateToString(it->getStartDate()).c_str(),
                                                it->getEndDate().dateToString(it->getEndDate()).c_str());
        participator = it->getParticipator();
        for (std::vector<string>::size_type i = 0; i < participator.size(); ++i) {
            cout << participator.at(i);
            if (i == participator.size() - 1)
                putchar(10);
            else
                cout << ",";
        }
    }
}
