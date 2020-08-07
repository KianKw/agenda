#include "../include/AgendaUI.hpp"
#include "../include/MyException.hpp"
#include <iostream>
#include <string>
#include <csignal>

static volatile int keepRunning = 1;

using std::cout;
using std::cin;
using std::endl;
using std::string;

AgendaUI::AgendaUI() {
    startAgenda();
}
void sig_handler(int sig) {
    if (sig == SIGINT) {
        keepRunning = 0;
    }
}
void AgendaUI::OperationLoop(void) {
    signal(SIGINT, sig_handler);
    while (keepRunning) {
        string operation = getOperation();
        putchar(10);
        executeOperation(operation);
        putchar(10);
    }
    // m_log.LogToFile(" ------ Ctrl C to End ------");
    quitAgenda();
}
void AgendaUI::startAgenda(void) {
    m_agendaService.startAgenda();
    // m_log.LogToFile(" ------ StartAgenda ------");
}
void PrintBeforeLoginOperations(void) {
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
    cout << "da   - delete all meeting" << endl;
}
std::string AgendaUI::getOperation() {
    for (int i = 0; i < 47; ++i)
        cout << '-';
    cout << "Agenda";
    for (int i = 0; i < 47; ++i)
        cout << '-';
    putchar(10);
    if (m_userName == "") {
        PrintBeforeLoginOperations();
        for (int i = 0; i < 100; ++i)
            cout << '-';
        cout << "\n\nAgenda :~$";
    } else {
        PrintAfterLoginOperations();
        for (int i = 0; i < 100; ++i)
            cout << '-';
        cout << "\n\nAgenda@" << m_userName << " :~#";
    }
    string operation;
    cin >> operation;
    return operation;
}
bool AgendaUI::executeOperation(std::string t_operation) {
    if (m_userName == "") {
        if (t_operation == "l")
            userLogIn();
        else if (t_operation == "r")
            userRegister();
        else if (t_operation == "q")
            quitAgenda();
        else {
            cout << "error: the operation dose not exit!" << endl;
            // m_log.LogToFile("BeforeLogIn - [error] " + t_operation + " operation dose not exit!");
            return false;
        }
    } else {
        if (t_operation == "o")
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
            cout << "error: the operation dose not exit!" << endl;
            // m_log.LogToFile(m_userName + " - [error] " + t_operation + " operation dose not exit!");
            return false;
        }
    }
    return true;
}
void AgendaUI::userLogIn(void) {
    cout << "[log in] [username] [password]" << endl;
    cout << "[log in] ";
    cin >> m_userName >> m_userPassword;
    try {
        m_agendaService.userLogIn(m_userName, m_userPassword);
        cout << "[log in] succeed!" << endl;
        // m_log.LogToFile(m_userName + " - [log in] succeed!");
    } catch (NotRegistered_User) {
        cout << "[log in] [error] User does not exist!" << endl;
        // m_log.LogToFile("BeforeLogIn - [log in] User does not exist!");
        m_userName = "";
        m_userPassword = "";
    } catch (PasswordError) {
        cout << "[log in] [error] Password error!" << endl;
        // m_log.LogToFile("BeforeLogIn - [log in] Password error!");
        m_userName = "";
        m_userPassword = "";
    }
}
void AgendaUI::userRegister(void) {
    string name, password, email, phone;
    cout << "[register] [user name] [password] [email] [phone]" << endl;
    cout << "[register] ";
    cin >> name >> password >> email >> phone;
    try {
        m_agendaService.userRegister(name, password, email, phone);
        cout << "[register] succeed!" << endl;
        // m_log.LogToFile(name + " - [register] succeed!");
    } catch (NameRepeat_user) {
        cout << "[register] [error] This name has been registered!" << endl;
        // m_log.LogToFile(name + " - [register] error! This name has been registered!");
    }
}
void AgendaUI::quitAgenda(void) {
    m_agendaService.quitAgenda();
    // m_log.LogToFile(" ------ QuitAgenda ------ ");
}
void AgendaUI::userLogOut(void) {
    // m_log.LogToFile(m_userName + " - [user log out]");
    m_userName = "";
    m_userPassword = "";
}
void AgendaUI::deleteUser(void) {
    m_agendaService.deleteUser(m_userName, m_userPassword);
    // m_log.LogToFile(m_userName + " - [delete user]");
    m_userName = "";
    m_userPassword = "";
}
void AgendaUI::listAllUsers(void) {
    std::list<User> temp = m_agendaService.listAllUsers();
    cout << "[list all users]" << endl;
    // m_log.LogToFile(m_userName + " - [list all user]");
    printf("\n%-30s %-30s %-30s\n", "name", "email", "phone");
    for (std::list<User>::const_iterator it = temp.begin(); it != temp.end(); ++it) {
        printf("\n%-30s %-30s %-30s\n", it->getName().c_str(), it->getEmail().c_str(), it->getPhone().c_str());
    }
}
void AgendaUI::createMeeting(void) {
    int num = 0;
    std::vector<string> participators;
    string temp, title, start_date, end_date;
    cout << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    cin >> num;
    while (num <= 0) {
        cout << "[create meeting] error: the number of participators is illegal!" << endl;
        cout << "[create meeting] please enter a positive integer!" << endl;
        cin >> num;
    }
    for (int i = 1; i <= num; ++i) {
        cout << "[create meeting] [please enter the participator " << i << "]" << endl;
        cout << "[create meeting] ";
        cin >> temp;
        participators.push_back(temp);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    cin >> title >> start_date >> end_date;
    try {
        m_agendaService.createMeeting(m_userName, title, start_date, end_date, participators);
        cout << "[create meeting] succeed!" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " succeed!");
    } catch (MeetingNoParticipator) {
        cout << "[create meeting] [error] There is no participator" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] There is no participator!");
    } catch (NotRegistered_Sponsor) {
        cout << "[create meeting] [error] Sponsor has not registered" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] Sponsor has not registered!");
    } catch (NotRegistered_Participator) {
        cout << "[create meeting] [error] Participator have not registered" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] Participator have not registered!");
    } catch (NameRepeat_SponsorParticipator) {
        cout << "[create meeting] [error] The sponsor is in participators list" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] The sponsor is in participators list!");
    } catch (NameRepeat_TWoParticipator) {
        cout << "[create meeting] [error] There are two participators have the same name" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] There are two participators have the same name!");
    } catch (TitleRepeat) {
        cout << "[create meeting] [error] The title has exited" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] The title has exited!");
    } catch (DateIllegal) {
        cout << "[create meeting] [error] The date is error" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] The date is error!");
    } catch (SponsorBusy) {
        cout << "[create meeting] [error] The sponsor is busy" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] The sponsor is busy!");
    } catch (ParticipatorBusy) {
        cout << "[create meeting] [error] The participator is busy" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] The participator is busy!");
    } catch (ErrorFinishNone) {
        cout << "[create meeting] [error] None is finished" << endl;
        // m_log.LogToFile(m_userName + " - [create meeting] " + title + " [error] None is finish!");
    }
}
void AgendaUI::addMeetingParticipator(void) {
    string title, participator;
    cout << "[add meeting participator] [title] [participator]" << endl;
    cout << "[add meeting participator] ";
    cin >> title >> participator;
    try {
        m_agendaService.addMeetingParticipator(m_userName, title, participator);
        cout << "[add meeting participator] succeed!" << endl;
        // m_log.LogToFile(m_userName + " - [add meeting participator] " + participator + " -> " + title);
    } catch (NotRegistered_Participator) {
        cout << "[add meeting participator] [error] The participator has not registed" << endl;
        // m_log.LogToFile(m_userName + " - [add meeting participator] [error] The participator has not registed!");
    } catch (NameRepeat_SponsorParticipator) {
        cout << "[add meeting participator] [error] Sponsor is the participator" << endl;
        // m_log.LogToFile(m_userName + " - [add meeting participator] [error] Sponsor is the participator!");
    } catch (ParticipatorBusy) {
        cout << "[add meeting participator] [error] The participator is busy" << endl;
        // m_log.LogToFile(m_userName + " - [add meeting participator] [error] The participator is busy!");
    } catch (ErrorFinishNone) {
        cout << "[add meeting participator] [error] None is finished" << endl;
        // m_log.LogToFile(m_userName + " - [add meeting participator] " + title + " [error] None is finish!");
    }
}
void AgendaUI::removeMeetingParticipator(void) {
    string title, participator;
    cout << "[remove meeting participator] [title] [participator]" << endl;
    cout << "[remove meeting participator] ";
    cin >> title >> participator;
    try {
        m_agendaService.removeMeetingParticipator(m_userName, title, participator);
        cout << "[remove meeting participator] succeed!" << endl;
        // m_log.LogToFile(m_userName + " - [remove meeting participator] remove " + participator + " from " + title);
    } catch (NoneMeetingName) {
        cout << "[remove meeting participator] [error] " << m_userName << " sponsor no meeting named " << title << endl;
        // m_log.LogToFile(m_userName + " - [remove meeting participator] [error] " + m_userName + " sponsor no meeting named " + title);
    } catch (NoneParticipatorName) {
        cout << "[remove meeting participator] [error] " << title << " has no participator named " << participator << endl;
        // m_log.LogToFile(m_userName + " - [remove meeting participator] [error] " + title + " has no participator named " + participator);
    } catch (ErrorFinishNone) {
        cout << "[remove meeting participator] [error] None is finished!" << endl;
        // m_log.LogToFile(m_userName + " - [remove meeting participator] [error] None is finished!");
    }
}
void AgendaUI::requestQuitMeeting(void) {
    string title;
    cout << "[request to quit meeting] [title]" << endl;
    cout << "[request to quit meeting] ";
    cin >> title;
    try {
        m_agendaService.quitMeeting(m_userName, title);
        cout << "[request to quit meeting] succeed!" << endl;
        // m_log.LogToFile(m_userName + " - [request to quit meeting] quit " + title);
    } catch (NoneMeetingName) {
        cout << "[request to quit meeting] [error] No meeting named " << title << endl;
        // m_log.LogToFile(m_userName + " - [request to quit meeting] [error] No meeting named " + title);
    } catch (SponsorQuitMeeting) {
        cout << "[request to quit meeting] [error] Sponsor can't quit meeting" << endl;
        // m_log.LogToFile(m_userName + " - [request to quit meeting] [error]  Sponsor can't quit meeting");
    } catch (NoneParticipatorName) {
        cout << "[request to quit meeting] [error] " << m_userName << " is not a participator of " << title << endl;
        // m_log.LogToFile(m_userName + " - [request to quit meeting] [error] " + m_userName + " is not a participator of " + title);
    } catch (ErrorFinishNone) {
        cout << "[request to quit meeting] [error] None is finished!" << endl;
        // m_log.LogToFile(m_userName + " - [request to quit meeting] [error] None is finished!");
    }
}
void AgendaUI::listAllMeetings(void) {
    cout << "[list all meetings]" << endl;
    // m_log.LogToFile(m_userName + " - [list all meetings]");
    std::list<Meeting> temp = m_agendaService.listAllMeetings(m_userName);
    printMeetings(temp);
}
void AgendaUI::listAllSponsorMeetings(void) {
    cout << "[list all sponsor meetings]" << endl;
    // m_log.LogToFile(m_userName + " - [list all sponsor meetings]");
    std::list<Meeting> temp = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(temp);
}
void AgendaUI::listAllParticipateMeetings(void) {
    cout << "[list all participator meetings]" << endl;
    // m_log.LogToFile(m_userName + " - [list all participator meetings]");
    std::list<Meeting> temp = m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(temp);
}
void AgendaUI::queryMeetingByTitle(void) {
    string title;
    cout << "[querry meeting] [title]" << endl;
    cout << "[querry meeting] ";
    cin >> title;
    // m_log.LogToFile(m_userName + " - [query meeting] " + title);
    std::list<Meeting> temp = m_agendaService.meetingQuery(m_userName, title);
    printMeetings(temp);
}
void AgendaUI::queryMeetingByTimeInterval(void) {
    string start_date, end_date;
    cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] ";
    cin >> start_date >> end_date;
    // m_log.LogToFile(m_userName + " - [query meeting] " + start_date + " to " + end_date);
    std::list<Meeting> temp = m_agendaService.meetingQuery(m_userName, start_date, end_date);
    printMeetings(temp);
}
void AgendaUI::deleteMeetingByTitle(void) {
    string title;
    cout << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    cin >> title;
    try {
        m_agendaService.deleteMeeting(m_userName, title);
        cout << "[delete meeting] succeed!" << endl;
        // m_log.LogToFile(m_userName + " - [delete meeting] " + title);
    } catch (ErrorFinishNone) {
        cout << "[delete meeting] [error] " << m_userName << " sponsor no meeting named " << title << endl;
        // m_log.LogToFile(m_userName + " - [delete meeting] [error] " + m_userName + " sponsor no meeting named " + title);
    }
}
void AgendaUI::deleteAllMeetings(void) {
    try {
        m_agendaService.deleteAllMeetings(m_userName);
        cout << "[delete all meeting] succeed!" << endl;
        // m_log.LogToFile(m_userName + " - [delete all meeting] succeed!");
    } catch (ErrorFinishNone) {
        cout << "[delete all meeting] error! There is no meeting!" << endl;
        // m_log.LogToFile(m_userName + " - [delete all meeting] error! There is no meeting!");
    }
}
void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    std::vector<string> participator;
    printf("\n%-20s %-20s %-20s %-20s %-20s\n", "title", "sponsor", "start time", "end time", "participators");
    for (std::list<Meeting>::const_iterator it = t_meetings.begin(); it != t_meetings.end(); ++it) {
        printf("%-20s %-20s %-20s %-20s ", it->getTitle().c_str(),
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
