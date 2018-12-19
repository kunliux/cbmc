/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/


#ifndef CPROVER_UTIL_MP_ARITH_H
#define CPROVER_UTIL_MP_ARITH_H

#include <string>
#include <iosfwd>

#include "big-int/bigint.hh"

// NOLINTNEXTLINE(readability/identifiers)
typedef BigInt mp_integer;

std::ostream &operator<<(std::ostream &, const mp_integer &);
mp_integer operator>>(const mp_integer &, const mp_integer &);
mp_integer operator<<(const mp_integer &, const mp_integer &);

const std::string integer2string(const mp_integer &, unsigned base=10);
const mp_integer string2integer(const std::string &, unsigned base=10);
const std::string integer2binary(const mp_integer &, std::size_t width);
const mp_integer binary2integer(const std::string &, bool is_signed);
mp_integer::ullong_t integer2ulong(const mp_integer &);
std::size_t integer2size_t(const mp_integer &);
unsigned integer2unsigned(const mp_integer &);

#endif // CPROVER_UTIL_MP_ARITH_H