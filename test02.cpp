#include <iostream>
#include <algorithm>
#include <iterator>

using std::cout; using std::endl;
using std::string; using std::reverse;

string ReverseString(string &s){
    string rev(s.rbegin(), s.rend());
    return rev;
}

int main() {
    string str = "This string shall be reversed";

    cout << str << endl;
    cout << ReverseString(str) << endl;

    return EXIT_SUCCESS;
}


// int main() {
//     string str = "This string shall be reversed";

//     cout << str << endl;

//     return EXIT_SUCCESS;
// }