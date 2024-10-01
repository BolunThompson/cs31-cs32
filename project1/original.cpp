// Code for Project 1
// Report poll results

#include <iostream>
using namespace std;

int main()
{
    int numberSurveyed;
    int forTrarris;
    int forHump;

    cout << "How many registered voters were surveyed? ";
    cin >> numberSurveyed;
    cout << "How many of them say they will probably vote for Trarris? ";
    cin >> forTrarris;
    cout << "How many of them say they will probably vote for Hump? ";
    cin >> forHump;

    double pctTrarris = 100.0 * forTrarris / numberSurveyed;
    double pctHump = 100.0 * forHump / numberSurveyed;
    cout.setf(ios::fixed);
    cout.precision(1);

    cout << endl;
    cout << pctTrarris << "% say they will probably vote for Trarris." << endl;
    cout << pctHump << "% say they will probably vote for Hump." << endl;
    if (forTrarris > forHump)
        cout << "Trarris is predicted to win the election." << endl;
    else
        cout << "Hump is predicted to win the election." << endl;
}
