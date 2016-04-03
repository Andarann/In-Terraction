
#include "main.hpp"
#include <ctime>
#include <cmath>

using namespace std;

int main(int argc, char* argv[])
{
    srand(time(NULL));
    //mainLoop();
    vector<float> meh;
    meh.push_back(5);
    meh.push_back(5);
    meh.push_back(5);

    cout << sizeof(meh.data());
    return 0;
}
