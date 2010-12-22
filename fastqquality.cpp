#include <iostream>
#include <getopt.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <signal.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <map>

#include "split.h"

using namespace std;

short qualityChar2ShortInt(char c) {
    return static_cast<short>(c) - 33;
}

int main(int argc, char** argv) {

    if (argc > 1) {
        cerr << "usage: [FASTQ stream] | " << argv[0] << endl
             << "prints a histogram of phred33 quality score and count for all bases" << endl
             << "in all reads in the FASTQ reads passed on stdin" << endl;
        exit(1);
    }

    map<int, long unsigned int> qualities;

    string name, bases, step, qualstr;

    while (cin) {
        std::getline(cin, name);
        std::getline(cin, bases);
        std::getline(cin, step);
        std::getline(cin, qualstr);
        for (string::iterator s = qualstr.begin(); s != qualstr.end(); ++s) {
            short q = qualityChar2ShortInt(*s);
            ++qualities[q];
        }
    }

    cout << "phred33\ttotal" << endl;

    for (map<int, long unsigned int>::iterator i = qualities.begin(); i != qualities.end(); ++i) {
        int qual = i->first;
        long unsigned int total = i->second;
        cout << qual << "\t" << total << endl;
    }

    return 0;

}
