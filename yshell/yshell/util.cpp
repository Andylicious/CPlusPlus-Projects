// $Id: util.cpp,v 1.4 2013-07-08 01:24:30-07 - - $
/*This program was completed using pair programming.
Partner:  Andrew Lien (ahlien@ucsc.edu)
Partner:  Daniel Jou (djou@ucsc.edu)
CMPS 109
Assignment 1
*/
#include <cstdlib>
#include <unistd.h>

using namespace std;

#include "util.h"
//throw a yshell exception, lots of places where error can occur, 
//to simply escape from an arbitrarily large # of levels
//we'll call it a yshell_exn, takes a string as an argument
//we'll initialize runtime error of what, 
//we declared the yshell exception
//in the header file, so here we have to provide w/ an implementation
yshell_exn::yshell_exn (const string &what): runtime_error (what) {
}
/*if i have a constructor that says foo:foo{ a=6; }
 *foo::foo():a(6){} do exactly the same thing. 
 */

int exit_status::status = EXIT_SUCCESS;
static string execname_string;

void exit_status::set (int new_status) {
    status = new_status;
}

int exit_status::get() {
    return status;
}

void setexecname (const string &name) {
    execname_string =  name.substr (name.rfind ('/') + 1);
    TRACE ('u', execname_string);
}

string &execname() {
    return execname_string;
}

bool want_echo() {
    const int CIN_FD = 0;
    const int COUT_FD = 1;
    bool cin_isatty = isatty (CIN_FD);
    bool cout_isatty = isatty (COUT_FD);
    TRACE ('u', "cin_isatty = " << cin_isatty
           << ", cout_isatty = " << cout_isatty);
    return ! cin_isatty || ! cout_isatty;
}


wordvec split (const string &line, const string &delimiters) {
    wordvec words;
    size_t end = 0;
    
    // Loop over the string, splitting out words, and for each word
    // thus found, append it to the output wordvec.
    for (;;) {
        size_t start = line.find_first_not_of (delimiters, end);
        if (start == string::npos) break;
        end = line.find_first_of (delimiters, start);
        words.push_back (line.substr (start, end - start));
    }
    TRACE ('u', words);
    return words;
}

ostream &complain() {
    exit_status::set (EXIT_FAILURE);
    cerr << execname() << ": ";
    return cerr;
}

template <typename item_t>
ostream &operator<< (ostream &out, const vector<item_t> &vec) {
    bool want_space = false;
    for (auto itor = vec.cbegin(); itor != vec.cend(); ++itor) {
        if (want_space) out << " ";
        else want_space = true;
        out << *itor;
    }
    return out;
}
