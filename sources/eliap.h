//
// FILE: eliap.h -- Interface to extensive form Liapunov solution module
//
// $Id$
//

#ifndef ELIAP_H
#define ELIAP_H

#include "gstream.h"
#include "gstatus.h"
#include "glist.h"

#include "efg.h"
#include "behavsol.h"

#include "subsolve.h"

class EFLiapParams  {
  public:
    int trace, nTries, stopAfter, maxits1, maxitsN;
    double tol1, tolN;
    gOutput *tracefile;
    gStatus &status;

    EFLiapParams(gStatus & = gstatus);
};


bool Liap(const Efg &, EFLiapParams &,
	  const BehavProfile<double> &, gList<BehavSolution> &,
	  long &nevals, long &niters);


class EFLiapBySubgame : public SubgameSolver  {
  private:
    int nevals, subgame_number;
    gPVector<int> infoset_subgames;
    EFLiapParams params;
    BehavProfile<gNumber> start;
    
    int SolveSubgame(const Efg &, const EFSupport &,
		     gList<BehavSolution> &);
    EfgAlgType AlgorithmID() const { return EfgAlg_ELIAPSUB; }    

  public:
    EFLiapBySubgame(const Efg &E, const EFLiapParams &,
		    const BehavProfile<gNumber> &, int max = 0);
    virtual ~EFLiapBySubgame();

    int NumEvals(void) const   { return nevals; }
};



#endif    // ELIAP_H









