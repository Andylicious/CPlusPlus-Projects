// $Id: numbers.cpp,v 1.1 2013-08-04 15:26:21-07 - - $
/*This program was completed using pair programming.
Partner:  Andrew Lien (ahlien@ucsc.edu)
Partner:  Daniel Jou (djou@ucsc.edu)
CMPS 109
Assignment 3
*/

#include <cstdlib>

using namespace std;

#include "numbers.h"
#include "util.h"

ostream &operator<< (ostream &out, const degrees &that) {
   out << that._degree_value << "deg";
   return out;
}

ostream &operator<< (ostream &out, const points &that) {
   out << that._point_value << "pt";
   return out;
}

ostream &operator<< (ostream &out, const inches &that) {
   out << that._inch_value << "in";
   return out;
}

ostream &operator<< (ostream &out, const xycoords &coords) {
   out << "(" << coords.first << "," << coords.second << ")";
   return out;
}
