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

#include "BamAlignment.h"
#include "BamReader.h"

using namespace std;
using namespace BamTools;

short qualityChar2ShortInt(char c) {
    return static_cast<short>(c) - 33;
}

int main(int argc, char** argv) {

    if (argc > 1) {
        cerr << "usage: [BAM data stream] | " << argv[0] << endl
             << "prints a histogram of phred33 quality score and count for all bases" << endl
             << "in all reads in the BAM alignments passed on stdin, by read group." << endl;
        exit(1);
    }

    map<int, long unsigned int> qualities;
    map<string, map<int, long unsigned int> > qualitiesRG;

    BamReader reader;
    if (!reader.Open("stdin")) {
        cerr << "could not open stdin for reading" << endl << endl;
        return 1;
    }

    map<string, string> readGroupToSampleName;

    string bamHeader = reader.GetHeaderText();

    vector<string> headerLines = split(bamHeader, '\n');

    for (vector<string>::const_iterator it = headerLines.begin(); it != headerLines.end(); ++it) {

        // get next line from header, skip if empty
        string headerLine = *it;
        if ( headerLine.empty() ) { continue; }

        // lines of the header look like:
        // "@RG     ID:-    SM:NA11832      CN:BCM  PL:454"
        //                     ^^^^^^^\ is our sample name
        if ( headerLine.find("@RG") == 0 ) {
            vector<string> readGroupParts = split(headerLine, "\t ");
            string name = "";
            string readGroupID = "";
            for (vector<string>::const_iterator r = readGroupParts.begin(); r != readGroupParts.end(); ++r) {
                vector<string> nameParts = split(*r, ":");
                if (nameParts.at(0) == "SM") {
                   name = nameParts.at(1);
                } else if (nameParts.at(0) == "ID") {
                   readGroupID = nameParts.at(1);
                }
            }
            if (name == "") {
                cerr << " could not find SM: in @RG tag " << endl << headerLine << endl;
                exit(1);
            }
            if (readGroupID == "") {
                cerr << " could not find ID: in @RG tag " << endl << headerLine << endl;
                exit(1);
            }
            readGroupToSampleName[readGroupID] = name;
        }
    }

    BamAlignment alignment;

    string readGroup;
    while (reader.GetNextAlignment(alignment)) {
        if (alignment.GetTag("RG", readGroup)) {
            for (string::iterator s = alignment.Qualities.begin(); s != alignment.Qualities.end(); ++s) {
                short q = qualityChar2ShortInt(*s);
                ++qualitiesRG[readGroup][q];
                ++qualities[q];
            }
        } else {
            for (string::iterator s = alignment.Qualities.begin(); s != alignment.Qualities.end(); ++s) {
                short q = qualityChar2ShortInt(*s);
                ++qualities[q];
            }
        }
    }

    cout << "# phred33\ttotal";
    for (map<string, string>::iterator r = readGroupToSampleName.begin(); r != readGroupToSampleName.end(); ++r) {
        cout << "\t" << r->first;
    }
    cout << endl;

    for (map<int, long unsigned int>::iterator i = qualities.begin(); i != qualities.end(); ++i) {
        int qual = i->first;
        long unsigned int total = i->second;
        cout << qual << "\t" << total;
        for (map<string, string>::iterator r = readGroupToSampleName.begin(); r != readGroupToSampleName.end(); ++r) {
            cout << "\t" << qualitiesRG[r->first][qual];
        }
        cout << endl;
    }

    reader.Close();

    return 0;

}
