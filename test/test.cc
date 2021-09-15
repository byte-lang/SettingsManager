#include "SettingsManager.h"
using namespace std;
using namespace SettingsManager;
int main()
{
    Parse parse;
    Data data;
    while(true) {
        string value;
        string change;
        cout << "Name: ";
        getline(cin, value);
        parse.Open("test.settings");
        data = parse.Read();
        cout << data[value] << endl;
    }
    return 1;
}