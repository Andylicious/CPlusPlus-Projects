// $Id: xless.h,v 1.2 2013-07-30 17:09:20-07 - - $

#ifndef __XLESS_H__
#define __XLESS_H__

//
// We assume that the type type_t has an operator< function.
//

template <typename Type>
struct xless {
   bool operator() (const Type &left, const Type &right) const {
      return left < right;
   }
};

#endif

