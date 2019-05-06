// $Id: bigint.cpp,v 1.78 2019-04-03 16:44:33-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;    

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue_, bool is_negative_):
                uvalue(uvalue_), is_negative(is_negative_) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
   return *this;
}

bigint bigint::operator- () const {
   return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
   ubigint result;
   bool new_is_neg;
   if(is_negative == that.is_negative) {
      result = uvalue + that.uvalue;
      new_is_neg = is_negative;
   } else {
      if(uvalue < that.uvalue) {
         result = that.uvalue - uvalue;
         new_is_neg = that.is_negative;
      } else {
         result = uvalue - that.uvalue;
         new_is_neg = is_negative;
      }
   }
   if(result == 0) {
       new_is_neg = false;
   }
   return {result, new_is_neg};
}

bigint bigint::operator- (const bigint& that) const {
   bigint a = {uvalue, is_negative};
   bigint b = {that.uvalue, not that.is_negative};
   return a+b;
}


bigint bigint::operator* (const bigint& that) const {
   ubigint result = uvalue * that.uvalue;
   bool new_is_neg = is_negative xor that.is_negative;
   return {result, new_is_neg};
}

bigint bigint::operator/ (const bigint& that) const {
   ubigint result = uvalue / that.uvalue;
   bool new_is_neg = is_negative xor that.is_negative;
   return {result, new_is_neg};
}

bigint bigint::operator% (const bigint& that) const {
   ubigint result = uvalue % that.uvalue;
   bool new_is_neg = is_negative;
   return {result, new_is_neg};
}

bool bigint::operator== (const bigint& that) const {
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative) return is_negative;
   return is_negative ? uvalue > that.uvalue
                      : uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
   return out << (that.is_negative ? "-" : "") << that.uvalue;
}
