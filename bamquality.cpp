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

#include "BamAlignment.h"
#include "BamReader.h"

using namespace std;
using namespace BamTools;

short qualityChar2ShortInt(char c) {
    return static_cast<short>(c) - 33;
}

int main(int argc, char** argv) {

    if (argc > 1) {
        cerr << "usage: [BAM data stream] | " << argv[0] << endl;
        cerr << "prints a histogram of phred33 quality score and count for all" << endl;
        cerr << "bases in all reads in the BAM alignments passed on stdin" << endl;
        exit(1);
    }

    map<int, long unsigned int> qualities;

    BamReader reader;
    if (!reader.Open("stdin")) {
        cerr << "could not open stdin for reading" << endl << endl;
        return 1;
    }

    BamAlignment alignment;

    while (reader.GetNextAlignment(alignment)) {
        for (string::iterator s = alignment.Qualities.begin(); s != alignment.Qualities.end(); ++s) {
            ++qualities[qualityChar2ShortInt(*s)];
        }
    }

    cout << "phred33\tcount" << endl;
    for (map<int, long unsigned int>::iterator i = qualities.begin(); i != qualities.end(); ++i) {
        cout << i->first << "\t" << i->second << endl;
    }

    reader.Close();

    return 0;

}
