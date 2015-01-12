// $Id: templates.tcc,v 1.2 2013-07-30 17:09:20-07 - - $
/*This program was completed using pair programming.
  Partner:  Andrew Lien (ahlien@ucsc.edu)
  Partner:  Daniel Jou (djou@ucsc.edu)
  CMPS 109
  Assignment 4
*/
// Explicit template instantiations.

#ifdef __LISTMAP_H__
template class listmap <string, string>;
#endif

#ifdef __UTIL_H__
template string to_string <int> (const int &);
#endif
