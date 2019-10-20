#include <iostream>
#include <csignal>

using namespace std;

static volatile int keepRunning = 1;

void sig_handler( int sig )
{
    if ( sig == SIGINT)
    {
        keepRunning = 0;
    }
}

int main( )
{
    // ��Ҫ���������߳���ע������źţ�����
    signal( SIGINT, sig_handler );

    while( keepRunning )
    {
      cout << "Running" << endl;
    }

    cout << "Terminated by Ctrl+C signal." << endl;

    cout << "Finishes data saving or some other work, and then exits.";

    return 0;
}
