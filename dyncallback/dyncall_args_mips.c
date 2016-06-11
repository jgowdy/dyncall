/*

 Package: dyncall
 Library: dyncallback
 File: dyncallback/dyncall_args_mips.c
 Description: Callback's Arguments VM - Implementation for non-o32 MIPS
 License:

   Copyright (c) 2013-2015 Daniel Adler <dadler@uni-goettingen.de>,
                           Tassilo Philipp <tphilipp@potion-studios.com>

   Permission to use, copy, modify, and distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/


#include "dyncall_args_mips.h"

DCint dcbArgInt(DCArgs* p)
{
  DCint value;
  if(p->reg_count.i < DCARGS_MIPS_NUM_IREGS)
    value = p->ireg_data[p->reg_count.i++];
  else {
    value = *((DCint*)p->stackptr);
    p->stackptr += sizeof(DCint);
  }
  return value;
}
DCuint dcbArgUInt(DCArgs* p) { return (DCuint)dcbArgInt(p); }

DCulonglong dcbArgULongLong(DCArgs* p)
{
  DCulonglong value;
  p->reg_count.i += (p->reg_count.i & 1); // Skip one reg if not aligned.
#if defined(DC__Endian_LITTLE)
  value  = ((DCulonglong)dcbArgUInt(p)) << 32;
  value |= dcbArgUInt(p);
#else
  value  = dcbArgUInt(p);
  value |= ((DCulonglong)dcbArgUInt(p)) << 32;
#endif
  return value;
}
DClonglong dcbArgLongLong(DCArgs* p) { return (DClonglong)dcbArgULongLong(p); }

DClong      dcbArgLong   (DCArgs* p) { return (DClong)   dcbArgUInt(p); }
DCulong     dcbArgULong  (DCArgs* p) { return (DCulong)  dcbArgUInt(p); }
DCchar      dcbArgChar   (DCArgs* p) { return (DCchar)   dcbArgUInt(p); }
DCuchar     dcbArgUChar  (DCArgs* p) { return (DCuchar)  dcbArgUInt(p); }
DCshort     dcbArgShort  (DCArgs* p) { return (DCshort)  dcbArgUInt(p); }
DCushort    dcbArgUShort (DCArgs* p) { return (DCushort) dcbArgUInt(p); }
DCbool      dcbArgBool   (DCArgs* p) { return (DCbool)   dcbArgUInt(p); }
DCpointer   dcbArgPointer(DCArgs* p) { return (DCpointer)dcbArgUInt(p); }

DCfloat dcbArgFloat(DCArgs* p)
{
  DCfloat result;
  if(p->reg_count.f < DCARGS_MIPS_NUM_FREGS)
    result = (DCfloat)p->freg_data[p->reg_count.f++];
  else {
    result = *((DCfloat*)p->stackptr);
    p->stackptr += sizeof(DCfloat);
  }
  return result;
}
DCdouble dcbArgDouble(DCArgs* p)
{
  union {
    DCdouble result;
    DCfloat  f[2];
  } d;
  p->reg_count.f += (p->reg_count.f & 1); // Skip one reg if not aligned.
#if defined(DC__Endian_LITTLE)
  d.f[0] = dcbArgFloat(p);
  d.f[1] = dcbArgFloat(p);
#else
  d.f[1] = dcbArgFloat(p);
  d.f[0] = dcbArgFloat(p);
#endif
  return d.result;
}

