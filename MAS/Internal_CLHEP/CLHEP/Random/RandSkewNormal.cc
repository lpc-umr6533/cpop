// $Id: RandSkewNormal.cc,v 1.1 2011/05/27 20:36:57 garren Exp $
// -*- C++ -*-
//
// -----------------------------------------------------------------------
//                             HEP Random
//                       --- RandSkewNormal ---
//                      class implementation file
// -----------------------------------------------------------------------

// =======================================================================
// M Fischler and L Garren - Created: 26 May 2011
// =======================================================================


#include "RandSkewNormal.h"
#include "RandGaussT.h"
#include "DoubConv.hh"

namespace CLHEP {

std::string RandSkewNormal::name() const {return "RandSkewNormal";}
HepRandomEngine & RandSkewNormal::engine() {return *localEngine;}

RandSkewNormal::~RandSkewNormal() {}

double RandSkewNormal::operator()() 
{
  return fire( shapeParameter );
}

double RandSkewNormal::operator()( double shape ) 
{
  return fire( shape );
}

//-------------

double RandSkewNormal::shoot(HepRandomEngine* anEngine)
{
  // really dumb use of RandSkewNormal
  double k = 1;
  return gaussianSkewNormal( anEngine, k );
}

double RandSkewNormal::shoot(HepRandomEngine* anEngine, double shape) 
{
  return gaussianSkewNormal( anEngine, shape );
}

double RandSkewNormal::shoot() 
{
  // really dumb use of RandSkewNormal
  HepRandomEngine* anEngine = HepRandom::getTheEngine();
  double k = 1;
  return gaussianSkewNormal( anEngine, k );
}

double RandSkewNormal::shoot(double shape) 
{
  HepRandomEngine* anEngine = HepRandom::getTheEngine();
  return gaussianSkewNormal( anEngine, shape );
}

void RandSkewNormal::shootArray( const int size, double* vect,
                                  double shape )
{
  for( double* v = vect; v != vect+size; ++v )
    *v = shoot(shape);
}

void RandSkewNormal::shootArray(HepRandomEngine* anEngine, const int size,
                                   double* vect, double shape )
{
  for( double* v = vect; v != vect+size; ++v )
    *v = shoot(anEngine, shape);
}

//-------------

double RandSkewNormal::fire() {
  return gaussianSkewNormal( getLocalEngine(), getShapeParameter() );
}

double RandSkewNormal::fire(double shape) {
  return gaussianSkewNormal( getLocalEngine(), shape );
}

void RandSkewNormal::fireArray( const int size, double* vect)
{
  for( double* v = vect; v != vect+size; ++v )
    *v = fire( shapeParameter );
}

void RandSkewNormal::fireArray( const int size, double* vect,
                                 double shape )
{
  for( double* v = vect; v != vect+size; ++v )
    *v = fire( shape );
}

//-------------

double RandSkewNormal::gaussianSkewNormal ( HepRandomEngine* e, double k) 
{ 
  // RandSkewNormal is an implementation of Azzalini's SN generator
  // http://azzalini.stat.unipd.it/SN/
  // K. McFarlane, June 2010.
  // For location parameter m = 0., scale = 1.
  // To get a distribution with scale parameter b and location m:
  //    r = m + b * RandSkewNormal.fire(k);
  double u[2] = {0.};
  RandGaussT::shootArray(e, 2, u, 0, 1);
  double delta = k/std::sqrt(1. + k*k);
  double u1 = delta*u[0] + std::sqrt(1 - delta*delta)*u[1];
  double r = u[0] >= 0 ? u1 : -u1;
  return r;
}

//-------------

std::ostream & RandSkewNormal::put ( std::ostream & os ) const {
  int pr=os.precision(20);
  std::vector<unsigned long> t(2);
  os << " " << name() << "\n";
  os << "Uvec" << "\n";
  t = DoubConv::dto2longs(shapeParameter);
  os << shapeParameter << " " << t[0] << " " << t[1] << "\n";
  os.precision(pr);
  return os;
}

std::istream & RandSkewNormal::get ( std::istream & is ) {
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
  if (possibleKeywordInput(is, "Uvec", shapeParameter)) {
    std::vector<unsigned long> t(2);
    is >> shapeParameter >> t[0] >> t[1]; shapeParameter = DoubConv::longs2double(t); 
    return is;
  }
  // is >> shapeParameter encompassed by possibleKeywordInput
  return is;
}


}  // namespace CLHEP
