// $Id: iterstack.h,v 1.8 2013-01-23 19:47:39-08 - - $
/*This program was completed using pair programming.
Partner:  Andrew Lien (ahlien@ucsc.edu)
Partner:  Daniel Jou (djou@ucsc.edu)
CMPS 109
Assignment 2
*/


#ifndef __ITERSTACK_H__
#define __ITERSTACK_H__

#include <vector>

using namespace std;

template <typename value_type>
class iterstack: private vector<value_type> {
   public:
      using vector<value_type>::const_reference;
      using vector<value_type>::const_reverse_iterator;
      using vector<value_type>::push_back;
      using vector<value_type>::pop_back;
      using vector<value_type>::clear;
      using vector<value_type>::back;
      using vector<value_type>::size;
      using vector<value_type>::empty;
      using vector<value_type>::rbegin;
      using vector<value_type>::rend;
      inline void push (const value_type &value) { push_back (value); }
      inline void pop() { pop_back(); }
      inline const value_type &top() const { return back(); }
};

#endif

