//
// FILE: gsignal.h -- Declaration of abstract signal-handling class
//
// $Id$
//

#ifndef GSIGNAL_H
#define GSIGNAL_H

#include "gmisc.h"
#include "gtext.h"

class gSignalBreak : public gException {
 public:
  gText Description(void) const { return "User interrupt occurred."; }
};

class gSignal   {
  public:
    virtual ~gSignal()  { }
    virtual bool Get(void) const = 0;
    virtual void Reset(void) = 0;
};

extern gSignal &gbreak;

#endif   // GSIGNAL_H
