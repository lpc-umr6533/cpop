// $Id: RandPoissonT.cc,v 1.7 2010/06/16 17:24:53 garren Exp $
// -*- C++ -*-
//
// -----------------------------------------------------------------------
//                             HEP Random
//                         --- RandPoissonT ---
//                      class implementation file
// -----------------------------------------------------------------------

// =======================================================================
// M. Fischler    - Implemented new, much faster table-driven algorithm
//		    applicable for mu < 100 (Nov 1999), using these as
//		    implementation of RandPoisson.
// M. Fischler	  - Moved the table driven algorithms into RandPoissonT (here)
//		    1/26/00.
// M. Fischler	  - Removed mean=100 from the table-driven set, since it
//		    uses a value just off the end of the table. (April 2004)
// M Fischler     - put and get to/from streams 12/15/04
// M Fischler     - fireArray using defaultMean 2/10/05
// M Fischler	      - put/get to/from streams uses pairs of ulongs when
//			+ storing doubles avoid problems with precision 
//			-- appears not to need modification, relying on
//			RandPoisson::put() instead  4/14/05
//
// =======================================================================


#include "RandPoissonT.h"
#include "RandPoissonQ.h"
#include "DoubConv.hh"

//
// Constructors and destructors:
//

namespace CLHEP {

std::string RandPoissonT::name() const {return "RandPoissonT";}
HepRandomEngine & RandPoissonT::engine() {return RandPoisson::engine();}

RandPoissonT::RandPoissonT(HepRandomEngine & anEngine, double m )
: RandPoisson(anEngine, m)
{}

RandPoissonT::RandPoissonT(HepRandomEngine * anEngine, double m )
: RandPoisson(anEngine, m)
{}

RandPoissonT::~RandPoissonT() {
}

//
// fire, operator(), and shoot methods:
//

long RandPoissonT::shoot(double xm) {
  HepRandomEngine* e = HepRandom::getTheEngine();
  return shoot ( e, xm );
} // shoot()

double RandPoissonT::operator()() {
  return double( fire() );
}

double RandPoissonT::operator()( double mean ) {
  return double( fire( mean ) );
}

long RandPoissonT::fire(double mean) {
  return shoot (getLocalEngine(), mean);
}

long RandPoissonT::fire() {

  if ( defaultMean < RandPoissonQ::tableBoundary() ) {
    return RandPoissonQ::shoot ( getLocalEngine(), defaultMean );
  } else {
    return RandPoisson::shoot(getLocalEngine(), defaultMean);
  }

} // fire()

long RandPoissonT::shoot(HepRandomEngine* anEngine, double mean) {

  if ( mean < RandPoissonQ::tableBoundary() ) {
    return RandPoissonQ::shoot ( anEngine, mean );
  } else {
    return RandPoisson::shoot( anEngine, mean );
  }

} // shoot (anEngine, mean)

void RandPoissonT::shootArray(const int size, long* vect, double m) {
   int i;
   for (i=0; i<size; ++i)
     vect[i] = shoot(m);
}

void RandPoissonT::fireArray(const int size, long* vect, double m) {
   int i;
   for (i=0; i<size; ++i) {
     vect[i] = fire( m );
   }
}

void RandPoissonT::fireArray(const int size, long* vect) {
   int i;
   for (i=0; i<size; ++i) {
     vect[i] = fire( defaultMean );
   }
}


std::ostream & RandPoissonT::put ( std::ostream & os ) const {
  int pr=os.precision(20);
  os << " " << name() << "\n";
  RandPoisson::put(os);
  os.precision(pr);
  return os;
}

std::istream & RandPoissonT::get ( std::istream & is ) {
  std::string inName;
  is >> inName;
  if (inName != name()) {
    is.clear(std::ios::badbit | is.rdstate());
    std::cerr << "Mismatch when expecting to read state of a "
    	      << name() << " distribution\n"
	      << "Name found was " << inName
	      << "\nistream is left in the badbit state\n";
    return is;
  }
  RandPoisson::get(is);
  return is;
}


}  // namespace CLHEP
