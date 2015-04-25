#include "temp.h"

#include <iostream>
#include <fstream>

using namespace std;

fstream fs;

/** For the listener to move at 1 me/sec along the y axis, with positions sampled at 240 Hz,
 *  the distance between each sampled position is d = 1/240.
 *  Test: With a source placed at 10 meters distance it should take 10 secs (exactly 441000 samples) for the
 *  listener to reach distance zero from the source (check).
 */
void generatePositions()
{
    unsigned n;
    float x=0., y=0., z=0.;

    n = 24000; // number of positions in file

    fs.open("input/positions.txt", fstream::in | fstream::out | fstream::trunc);

    if (!fs.is_open()) {
        cout << "Error opening file..." << endl;
        return;
    }

    fs << n << " ";

    for (unsigned i = 0; i < n; i++) {
        y = i * 0.00416666666;
        fs << x << " " << y << " " << z << " ";
    }

    fs.close();
}

vector<aserver::Location> readPositions()
{
    fs.open("input/positions.txt", fstream::in );

    if (!fs.is_open()) {
        cout << "Error opening file..." << endl;
    }

    float x, y, z;
    unsigned n;
    fs >> n;

    vector<aserver::Location> pos = vector<aserver::Location>(n);

    for (unsigned i = 0; i < n; i++) {
        fs >> x >> y >> z;
        pos[i] = aserver::Location(x, y, z);
    }
    return pos;
}