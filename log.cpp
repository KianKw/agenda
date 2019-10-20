#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <time.h>


using namespace std;

void writeToFile(std::string s) {
  time_t t = time(0);
  char str[100];
  std::ofstream out("./Log.data", std::ofstream::app);
  strftime(str, sizeof(str), "%Y-%m-%d/%I:%M:%S", localtime(&t));
  std::string ns(str);
  out << " * " << ns << " * "<< "---"  << s << std::endl;
  out.close();
}

int main()
{
    string s = "aaaaaa";
    writeToFile(s);
    cout << "Hello World!" << endl;
    return 0;
}
