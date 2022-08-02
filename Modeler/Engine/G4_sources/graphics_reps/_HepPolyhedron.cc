//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: HepPolyhedron.cc,v 1.34 2009/10/28 13:36:32 allison Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
// 
//
// G4 Polyhedron library
//
// History:
// 23.07.96 E.Chernyaev <Evgueni.Tcherniaev@cern.ch> - initial version
//
// 30.09.96 E.Chernyaev
// - added GetNextVertexIndex, GetVertex by Yasuhide Sawada
// - added GetNextUnitNormal, GetNextEdgeIndeces, GetNextEdge
//
// 15.12.96 E.Chernyaev
// - added GetNumberOfRotationSteps, RotateEdge, RotateAroundZ, SetReferences
// - rewritten G4PolyhedronCons;
// - added G4PolyhedronPara, ...Trap, ...Pgon, ...Pcon, ...Sphere, ...Torus
//
// 01.06.97 E.Chernyaev
// - modified RotateAroundZ, added SetSideFacets
//
// 19.03.00 E.Chernyaev
// - implemented boolean operations (add, subtract, intersect) on polyhedra;
//
// 25.05.01 E.Chernyaev
// - added GetSurfaceArea() and GetVolume();
//
// 05.11.02 E.Chernyaev
// - added createTwistedTrap() and createPolyhedron();
//
// 20.06.05 G.Cosmo
// - added HepPolyhedronEllipsoid;
//
// 18.07.07 T.Nikitin
// - added HepParaboloid;
  
#include "_HepPolyhedron.h"
///#include <CLHEP/Units/PhysicalConstants.h>
///#include <CLHEP/Geometry/Vector3D.h>

#include <cstdlib>  // Required on some compilers for std::abs(int) ...
#include <cmath>
#include <iostream>

/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron operator <<                   Date:    09.05.96 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised:          *
 *                                                                     *
 * Function: Print contents of G4 polyhedron                           *
 *                                                                     *
 ***********************************************************************/
std::ostream & operator<<(std::ostream & ostr, const G4Facet & facet) {
  // for (int k=0; k<4; k++) {
  //   ostr << " " << facet.edge[k].v << "/" << facet.edge[k].f;
  // }
  return ostr;
}

std::ostream & operator<<(std::ostream & ostr, const HepPolyhedron & ph) {
  // ostr << std::endl;
  // ostr << "Nverteces=" << ph.nvert << ", Nfacets=" << ph.nface << std::endl;
  // int i;
  // for (i=1; i<=ph.nvert; i++) {
  //    ostr << "xyz(" << i << ")="
  //         << ph.pV[i].x() << ' ' << ph.pV[i].y() << ' ' << ph.pV[i].z()
  //         << std::endl;
  // }
  // for (i=1; i<=ph.nface; i++) {
  //   ostr << "face(" << i << ")=" << ph.pF[i] << std::endl;
  // }
  return ostr;
}

HepPolyhedron::HepPolyhedron(const HepPolyhedron &from)
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron copy constructor             Date:    23.07.96  *
 * Author: E.Chernyaev (IHEP/Protvino)              Revised:           *
 *                                                                     *
 ***********************************************************************/
: nvert(0), nface(0), pF(0)
{
  AllocateMemory(from.nvert, from.nface);
  for (int k=1; k<=nface; k++) pF[k] = from.pF[k];
}

HepPolyhedron & HepPolyhedron::operator=(const HepPolyhedron &from)
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron operator =                   Date:    23.07.96  *
 * Author: E.Chernyaev (IHEP/Protvino)              Revised:           *
 *                                                                     *
 * Function: Copy contents of one polyhedron to another                *
 *                                                                     *
 ***********************************************************************/
{
  if (this != &from) {
    AllocateMemory(from.nvert, from.nface);
    for (int k=1; k<=nface; k++) pF[k] = from.pF[k];
  }
  return *this;
}

int
HepPolyhedron::FindNeighbour(int iFace, int iNode, int iOrder) const
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron::FindNeighbour                Date:    22.11.99 *
 * Author: E.Chernyaev                               Revised:          *
 *                                                                     *
 * Function: Find neighbouring face                                    *
 *                                                                     *
 ***********************************************************************/
{
  int i;
  for (i=0; i<4; i++) {
    if (iNode == std::abs(pF[iFace].edge[i].v)) break;
  }
  if (i == 4) {
    std::cerr
      << "HepPolyhedron::FindNeighbour: face " << iFace
      << " has no node " << iNode
      << std::endl; 
    return 0;
  }
  if (iOrder < 0) {
    if ( --i < 0) i = 3;
    if (pF[iFace].edge[i].v == 0) i = 2;
  }
  return (pF[iFace].edge[i].v > 0) ? 0 : pF[iFace].edge[i].f;
}

void HepPolyhedron::AllocateMemory(int Nvert, int Nface)
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron::AllocateMemory               Date:    19.06.96 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised: 05.11.02 *
 *                                                                     *
 * Function: Allocate memory for GEANT4 polyhedron                     *
 *                                                                     *
 * Input: Nvert - number of nodes                                      *
 *        Nface - number of faces                                      *
 *                                                                     *
 ***********************************************************************/
{
  if (nvert == Nvert && nface == Nface) return;
  if (pF != 0) delete [] pF;
  if (Nvert > 0 && Nface > 0) {
    nvert = Nvert;
    nface = Nface;
    pF    = new G4Facet[nface+1];
  }else{
    nvert = 0; nface = 0; pF = 0;
  }
}

void HepPolyhedron::CreatePrism()
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron::CreatePrism                  Date:    15.07.96 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised:          *
 *                                                                     *
 * Function: Set facets for a prism                                    *
 *                                                                     *
 ***********************************************************************/
{
  enum {DUMMY, BOTTOM, LEFT, BACK, RIGHT, FRONT, TOP};

  pF[1] = G4Facet(1,LEFT,  4,BACK,  3,RIGHT,  2,FRONT);
  pF[2] = G4Facet(5,TOP,   8,BACK,  4,BOTTOM, 1,FRONT);
  pF[3] = G4Facet(8,TOP,   7,RIGHT, 3,BOTTOM, 4,LEFT);
  pF[4] = G4Facet(7,TOP,   6,FRONT, 2,BOTTOM, 3,BACK);
  pF[5] = G4Facet(6,TOP,   5,LEFT,  1,BOTTOM, 2,RIGHT);
  pF[6] = G4Facet(5,FRONT, 6,RIGHT, 7,BACK,   8,LEFT);
}

void HepPolyhedron::RotateEdge(int k1, int k2, double r1, double r2,
                              int v1, int v2, int vEdge,
                              bool ifWholeCircle, int ns, int &kface)
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron::RotateEdge                   Date:    05.12.96 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised:          *
 *                                                                     *
 * Function: Create set of facets by rotation of an edge around Z-axis *
 *                                                                     *
 * Input: k1, k2 - end vertices of the edge                            *
 *        r1, r2 - radiuses of the end vertices                        *
 *        v1, v2 - visibility of edges produced by rotation of the end *
 *                 vertices                                            *
 *        vEdge  - visibility of the edge                              *
 *        ifWholeCircle - is true in case of whole circle rotation     *
 *        ns     - number of discrete steps                            *
 *        r[]    - r-coordinates                                       *
 *        kface  - current free cell in the pF array                   *
 *                                                                     *
 ***********************************************************************/
{
  if (r1 == 0. && r2 == 0) return;

  int i;
  int i1  = k1;
  int i2  = k2;
  int ii1 = ifWholeCircle ? i1 : i1+ns;
  int ii2 = ifWholeCircle ? i2 : i2+ns;
  int vv  = ifWholeCircle ? vEdge : 1;

  if (ns == 1) {
    if (r1 == 0.) {
      pF[kface++]   = G4Facet(i1,0,    v2*i2,0, (i2+1),0);
    }else if (r2 == 0.) {
      pF[kface++]   = G4Facet(i1,0,    i2,0,    v1*(i1+1),0);
    }else{
      pF[kface++]   = G4Facet(i1,0,    v2*i2,0, (i2+1),0, v1*(i1+1),0);
    }
  }else{
    if (r1 == 0.) {
      pF[kface++]   = G4Facet(vv*i1,0,    v2*i2,0, vEdge*(i2+1),0);
      for (i2++,i=1; i<ns-1; i2++,i++) {
        pF[kface++] = G4Facet(vEdge*i1,0, v2*i2,0, vEdge*(i2+1),0);
      }
      pF[kface++]   = G4Facet(vEdge*i1,0, v2*i2,0, vv*ii2,0);
    }else if (r2 == 0.) {
      pF[kface++]   = G4Facet(vv*i1,0,    vEdge*i2,0, v1*(i1+1),0);
      for (i1++,i=1; i<ns-1; i1++,i++) {
        pF[kface++] = G4Facet(vEdge*i1,0, vEdge*i2,0, v1*(i1+1),0);
      }
      pF[kface++]   = G4Facet(vEdge*i1,0, vv*i2,0,    v1*ii1,0);
    }else{
      pF[kface++]   = G4Facet(vv*i1,0,    v2*i2,0, vEdge*(i2+1),0,v1*(i1+1),0);
      for (i1++,i2++,i=1; i<ns-1; i1++,i2++,i++) {
        pF[kface++] = G4Facet(vEdge*i1,0, v2*i2,0, vEdge*(i2+1),0,v1*(i1+1),0);
      }  
      pF[kface++]   = G4Facet(vEdge*i1,0, v2*i2,0, vv*ii2,0,      v1*ii1,0);
    }
  }
}

void HepPolyhedron::SetSideFacets(int ii[4], int vv[4],
                                 int *kk, double *r,
                                 double dphi, int ns, int &kface)
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron::SetSideFacets                Date:    20.05.97 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised:          *
 *                                                                     *
 * Function: Set side facets for the case of incomplete rotation       *
 *                                                                     *
 * Input: ii[4] - indeces of original verteces                         *
 *        vv[4] - visibility of edges                                  *
 *        kk[]  - indeces of nodes                                     *
 *        r[]   - radiuses                                             *
 *        dphi  - delta phi                                            *
 *        ns     - number of discrete steps                            *
 *        kface  - current free cell in the pF array                   *
 *                                                                     *
 ***********************************************************************/
{
  // int k1, k2, k3, k4;
  
  // if (std::abs((double)(dphi-pi)) < perMillion) {          // half a circle
  //   for (int i=0; i<4; i++) {
  //     k1 = ii[i];
  //     k2 = (i == 3) ? ii[0] : ii[i+1];
  //     if (r[k1] == 0. && r[k2] == 0.) vv[i] = -1;      
  //   }
  // }

  // if (ii[1] == ii[2]) {
  //   k1 = kk[ii[0]];
  //   k2 = kk[ii[2]];
  //   k3 = kk[ii[3]];
  //   pF[kface++] = G4Facet(vv[0]*k1,0, vv[2]*k2,0, vv[3]*k3,0);
  //   if (r[ii[0]] != 0.) k1 += ns;
  //   if (r[ii[2]] != 0.) k2 += ns;
  //   if (r[ii[3]] != 0.) k3 += ns;
  //   pF[kface++] = G4Facet(vv[2]*k3,0, vv[0]*k2,0, vv[3]*k1,0);
  // }else if (kk[ii[0]] == kk[ii[1]]) {
  //   k1 = kk[ii[0]];
  //   k2 = kk[ii[2]];
  //   k3 = kk[ii[3]];
  //   pF[kface++] = G4Facet(vv[1]*k1,0, vv[2]*k2,0, vv[3]*k3,0);
  //   if (r[ii[0]] != 0.) k1 += ns;
  //   if (r[ii[2]] != 0.) k2 += ns;
  //   if (r[ii[3]] != 0.) k3 += ns;
  //   pF[kface++] = G4Facet(vv[2]*k3,0, vv[1]*k2,0, vv[3]*k1,0);
  // }else if (kk[ii[2]] == kk[ii[3]]) {
  //   k1 = kk[ii[0]];
  //   k2 = kk[ii[1]];
  //   k3 = kk[ii[2]];
  //   pF[kface++] = G4Facet(vv[0]*k1,0, vv[1]*k2,0, vv[3]*k3,0);
  //   if (r[ii[0]] != 0.) k1 += ns;
  //   if (r[ii[1]] != 0.) k2 += ns;
  //   if (r[ii[2]] != 0.) k3 += ns;
  //   pF[kface++] = G4Facet(vv[1]*k3,0, vv[0]*k2,0, vv[3]*k1,0);
  // }else{
  //   k1 = kk[ii[0]];
  //   k2 = kk[ii[1]];
  //   k3 = kk[ii[2]];
  //   k4 = kk[ii[3]];
  //   pF[kface++] = G4Facet(vv[0]*k1,0, vv[1]*k2,0, vv[2]*k3,0, vv[3]*k4,0);
  //   if (r[ii[0]] != 0.) k1 += ns;
  //   if (r[ii[1]] != 0.) k2 += ns;
  //   if (r[ii[2]] != 0.) k3 += ns;
  //   if (r[ii[3]] != 0.) k4 += ns;
  //   pF[kface++] = G4Facet(vv[2]*k4,0, vv[1]*k3,0, vv[0]*k2,0, vv[3]*k1,0);
  // }
}

void HepPolyhedron::RotateAroundZ(int nstep, double phi, double dphi,
                                 int np1, int np2,
                                 const double *z, double *r,
                                 int nodeVis, int edgeVis)
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron::RotateAroundZ                Date:    27.11.96 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised:          *
 *                                                                     *
 * Function: Create HepPolyhedron for a solid produced by rotation of  *
 *           two polylines around Z-axis                               *
 *                                                                     *
 * Input: nstep - number of discrete steps, if 0 then default          *
 *        phi   - starting phi angle                                   *
 *        dphi  - delta phi                                            *
 *        np1   - number of points in external polyline                *
 *                (must be negative in case of closed polyline)        *
 *        np2   - number of points in internal polyline (may be 1)     *
 *        z[]   - z-coordinates (+z >>> -z for both polylines)         *
 *        r[]   - r-coordinates                                        *
 *        nodeVis - how to Draw edges joing consecutive positions of   *
 *                  node during rotation                               *
 *        edgeVis - how to Draw edges                                  *
 *                                                                     *
 ***********************************************************************/
{
  // static double wholeCircle   = twopi;
    
  // //   S E T   R O T A T I O N   P A R A M E T E R S

  // bool ifWholeCircle = (std::abs(dphi-wholeCircle) < perMillion) ? true : false;
  // double   delPhi  = ifWholeCircle ? wholeCircle : dphi;  
  // int        nSphi    = (nstep > 0) ?
  //   nstep : int(delPhi*GetNumberOfRotationSteps()/wholeCircle+.5);
  // if (nSphi == 0) nSphi = 1;
  // int        nVphi    = ifWholeCircle ? nSphi : nSphi+1;
  // bool ifClosed = np1 > 0 ? false : true;
  
  // //   C O U N T   V E R T E C E S

  // int absNp1 = std::abs(np1);
  // int absNp2 = std::abs(np2);
  // int i1beg = 0;
  // int i1end = absNp1-1;
  // int i2beg = absNp1;
  // int i2end = absNp1+absNp2-1; 
  // int i, j, k;

  // for(i=i1beg; i<=i2end; i++) {
  //   if (std::abs(r[i]) < perMillion) r[i] = 0.;
  // }

  // j = 0;                                                // external nodes
  // for (i=i1beg; i<=i1end; i++) {
  //   j += (r[i] == 0.) ? 1 : nVphi;
  // }

  // bool ifSide1 = false;                           // internal nodes
  // bool ifSide2 = false;

  // if (r[i2beg] != r[i1beg] || z[i2beg] != z[i1beg]) {
  //   j += (r[i2beg] == 0.) ? 1 : nVphi;
  //   ifSide1 = true;
  // }

  // for(i=i2beg+1; i<i2end; i++) {
  //   j += (r[i] == 0.) ? 1 : nVphi;
  // }
  
  // if (r[i2end] != r[i1end] || z[i2end] != z[i1end]) {
  //   if (absNp2 > 1) j += (r[i2end] == 0.) ? 1 : nVphi;
  //   ifSide2 = true;
  // }

  // //   C O U N T   F A C E S

  // k = ifClosed ? absNp1*nSphi : (absNp1-1)*nSphi;       // external faces

  // if (absNp2 > 1) {                                     // internal faces
  //   for(i=i2beg; i<i2end; i++) {
  //     if (r[i] > 0. || r[i+1] > 0.)       k += nSphi;
  //   }

  //   if (ifClosed) {
  //     if (r[i2end] > 0. || r[i2beg] > 0.) k += nSphi;
  //   }
  // }

  // if (!ifClosed) {                                      // side faces
  //   if (ifSide1 && (r[i1beg] > 0. || r[i2beg] > 0.)) k += nSphi;
  //   if (ifSide2 && (r[i1end] > 0. || r[i2end] > 0.)) k += nSphi;
  // }

  // if (!ifWholeCircle) {                                 // phi_side faces
  //   k += ifClosed ? 2*absNp1 : 2*(absNp1-1);
  // }

  // //   A L L O C A T E   M E M O R Y

  // AllocateMemory(j, k);

  // //   G E N E R A T E   V E R T E C E S

  // int *kk;
  // kk = new int[absNp1+absNp2];

  // k = 1;
  // for(i=i1beg; i<=i1end; i++) {
  //   kk[i] = k;
  //   if (r[i] == 0.)
  //   { pV[k++] = Point3D<double>(0, 0, z[i]); } else { k += nVphi; }
  // }

  // i = i2beg;
  // if (ifSide1) {
  //   kk[i] = k;
  //   if (r[i] == 0.)
  //   { pV[k++] = Point3D<double>(0, 0, z[i]); } else { k += nVphi; }
  // }else{
  //   kk[i] = kk[i1beg];
  // }

  // for(i=i2beg+1; i<i2end; i++) {
  //   kk[i] = k;
  //   if (r[i] == 0.)
  //   { pV[k++] = Point3D<double>(0, 0, z[i]); } else { k += nVphi; }
  // }

  // if (absNp2 > 1) {
  //   i = i2end;
  //   if (ifSide2) {
  //     kk[i] = k;
  //     if (r[i] == 0.) pV[k] = Point3D<double>(0, 0, z[i]);
  //   }else{
  //     kk[i] = kk[i1end];
  //   }
  // }

  // double cosPhi, sinPhi;

  // for(j=0; j<nVphi; j++) {
  //   cosPhi = std::cos(phi+j*delPhi/nSphi);
  //   sinPhi = std::sin(phi+j*delPhi/nSphi);
  //   for(i=i1beg; i<=i2end; i++) {
  //     if (r[i] != 0.)
  //       pV[kk[i]+j] = Point3D<double>(r[i]*cosPhi,r[i]*sinPhi,z[i]);
  //   }
  // }

  // //   G E N E R A T E   E X T E R N A L   F A C E S

  // int v1,v2;

  // k = 1;
  // v2 = ifClosed ? nodeVis : 1;
  // for(i=i1beg; i<i1end; i++) {
  //   v1 = v2;
  //   if (!ifClosed && i == i1end-1) {
  //     v2 = 1;
  //   }else{
  //     v2 = (r[i] == r[i+1] && r[i+1] == r[i+2]) ? -1 : nodeVis;
  //   }
  //   RotateEdge(kk[i], kk[i+1], r[i], r[i+1], v1, v2,
  //              edgeVis, ifWholeCircle, nSphi, k);
  // }
  // if (ifClosed) {
  //   RotateEdge(kk[i1end], kk[i1beg], r[i1end],r[i1beg], nodeVis, nodeVis,
  //              edgeVis, ifWholeCircle, nSphi, k);
  // }

  // //   G E N E R A T E   I N T E R N A L   F A C E S

  // if (absNp2 > 1) {
  //   v2 = ifClosed ? nodeVis : 1;
  //   for(i=i2beg; i<i2end; i++) {
  //     v1 = v2;
  //     if (!ifClosed && i==i2end-1) {
  //       v2 = 1;
  //     }else{
  //       v2 = (r[i] == r[i+1] && r[i+1] == r[i+2]) ? -1 :  nodeVis;
  //     }
  //     RotateEdge(kk[i+1], kk[i], r[i+1], r[i], v2, v1,
  //                edgeVis, ifWholeCircle, nSphi, k);
  //   }
  //   if (ifClosed) {
  //     RotateEdge(kk[i2beg], kk[i2end], r[i2beg], r[i2end], nodeVis, nodeVis,
  //                edgeVis, ifWholeCircle, nSphi, k);
  //   }
  // }

  // //   G E N E R A T E   S I D E   F A C E S

  // if (!ifClosed) {
  //   if (ifSide1) {
  //     RotateEdge(kk[i2beg], kk[i1beg], r[i2beg], r[i1beg], 1, 1,
  //                -1, ifWholeCircle, nSphi, k);
  //   }
  //   if (ifSide2) {
  //     RotateEdge(kk[i1end], kk[i2end], r[i1end], r[i2end], 1, 1,
  //                -1, ifWholeCircle, nSphi, k);
  //   }
  // }

  // //   G E N E R A T E   S I D E   F A C E S  for the case of incomplete circle

  // if (!ifWholeCircle) {

  //   int  ii[4], vv[4];

  //   if (ifClosed) {
  //     for (i=i1beg; i<=i1end; i++) {
  //       ii[0] = i;
  //       ii[3] = (i == i1end) ? i1beg : i+1;
  //       ii[1] = (absNp2 == 1) ? i2beg : ii[0]+absNp1;
  //       ii[2] = (absNp2 == 1) ? i2beg : ii[3]+absNp1;
  //       vv[0] = -1;
  //       vv[1] = 1;
  //       vv[2] = -1;
  //       vv[3] = 1;
  //       SetSideFacets(ii, vv, kk, r, dphi, nSphi, k);
  //     }
  //   }else{
  //     for (i=i1beg; i<i1end; i++) {
  //       ii[0] = i;
  //       ii[3] = i+1;
  //       ii[1] = (absNp2 == 1) ? i2beg : ii[0]+absNp1;
  //       ii[2] = (absNp2 == 1) ? i2beg : ii[3]+absNp1;
  //       vv[0] = (i == i1beg)   ? 1 : -1;
  //       vv[1] = 1;
  //       vv[2] = (i == i1end-1) ? 1 : -1;
  //       vv[3] = 1;
  //       SetSideFacets(ii, vv, kk, r, dphi, nSphi, k);
  //     }
  //   }      
  // }

  // delete [] kk;

  // if (k-1 != nface) {
  //   std::cerr
  //     << "Polyhedron::RotateAroundZ: number of generated faces ("
  //     << k-1 << ") is not equal to the number of allocated faces ("
  //     << nface << ")"
  //     << std::endl;
  // }
}

void HepPolyhedron::SetReferences()
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron::SetReferences                Date:    04.12.96 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised:          *
 *                                                                     *
 * Function: For each edge set reference to neighbouring facet         *
 *                                                                     *
 ***********************************************************************/
{
  if (nface <= 0) return;

  struct edgeListMember {
    edgeListMember *next;
    int v2;
    int iface;
    int iedge;
  } *edgeList, *freeList, **headList;

  
  //   A L L O C A T E   A N D   I N I T I A T E   L I S T S

  edgeList = new edgeListMember[2*nface];
  headList = new edgeListMember*[nvert];
  
  int i;
  for (i=0; i<nvert; i++) {
    headList[i] = 0;
  }
  freeList = edgeList;
  for (i=0; i<2*nface-1; i++) {
    edgeList[i].next = &edgeList[i+1];
  }
  edgeList[2*nface-1].next = 0;

  //   L O O P   A L O N G   E D G E S

  int iface, iedge, nedge, i1, i2, k1, k2;
  edgeListMember *prev, *cur;
  
  for(iface=1; iface<=nface; iface++) {
    nedge = (pF[iface].edge[3].v == 0) ? 3 : 4;
    for (iedge=0; iedge<nedge; iedge++) {
      i1 = iedge;
      i2 = (iedge < nedge-1) ? iedge+1 : 0;
      i1 = std::abs(pF[iface].edge[i1].v);
      i2 = std::abs(pF[iface].edge[i2].v);
      k1 = (i1 < i2) ? i1 : i2;          // k1 = ::min(i1,i2);
      k2 = (i1 > i2) ? i1 : i2;          // k2 = ::max(i1,i2);
      
      // check head of the List corresponding to k1
      cur = headList[k1];
      if (cur == 0) {
        headList[k1] = freeList;
        freeList = freeList->next;
        cur = headList[k1];
        cur->next = 0;
        cur->v2 = k2;
        cur->iface = iface;
        cur->iedge = iedge;
        continue;
      }

      if (cur->v2 == k2) {
        headList[k1] = cur->next;
        cur->next = freeList;
        freeList = cur;      
        pF[iface].edge[iedge].f = cur->iface;
        pF[cur->iface].edge[cur->iedge].f = iface;
        i1 = (pF[iface].edge[iedge].v < 0) ? -1 : 1;
        i2 = (pF[cur->iface].edge[cur->iedge].v < 0) ? -1 : 1;
        if (i1 != i2) {
          std::cerr
            << "Polyhedron::SetReferences: different edge visibility "
            << iface << "/" << iedge << "/"
            << pF[iface].edge[iedge].v << " and "
            << cur->iface << "/" << cur->iedge << "/"
            << pF[cur->iface].edge[cur->iedge].v
            << std::endl;
        }
        continue;
      }

      // check List itself
      for (;;) {
        prev = cur;
        cur = prev->next;
        if (cur == 0) {
          prev->next = freeList;
          freeList = freeList->next;
          cur = prev->next;
          cur->next = 0;
          cur->v2 = k2;
          cur->iface = iface;
          cur->iedge = iedge;
          break;
        }

        if (cur->v2 == k2) {
          prev->next = cur->next;
          cur->next = freeList;
          freeList = cur;      
          pF[iface].edge[iedge].f = cur->iface;
          pF[cur->iface].edge[cur->iedge].f = iface;
          i1 = (pF[iface].edge[iedge].v < 0) ? -1 : 1;
          i2 = (pF[cur->iface].edge[cur->iedge].v < 0) ? -1 : 1;
            if (i1 != i2) {
              std::cerr
                << "Polyhedron::SetReferences: different edge visibility "
                << iface << "/" << iedge << "/"
                << pF[iface].edge[iedge].v << " and "
                << cur->iface << "/" << cur->iedge << "/"
                << pF[cur->iface].edge[cur->iedge].v
                << std::endl;
            }
          break;
        }
      }
    }
  }

  //  C H E C K   T H A T   A L L   L I S T S   A R E   E M P T Y

  for (i=0; i<nvert; i++) {
    if (headList[i] != 0) {
      std::cerr
        << "Polyhedron::SetReferences: List " << i << " is not empty"
        << std::endl;
    }
  }

  //   F R E E   M E M O R Y

  delete [] edgeList;
  delete [] headList;
}

void HepPolyhedron::InvertFacets()
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron::InvertFacets                Date:    01.12.99  *
 * Author: E.Chernyaev                              Revised:           *
 *                                                                     *
 * Function: Invert the order of the nodes in the facets               *
 *                                                                     *
 ***********************************************************************/
{
  if (nface <= 0) return;
  int i, k, nnode, v[4],f[4];
  for (i=1; i<=nface; i++) {
    nnode =  (pF[i].edge[3].v == 0) ? 3 : 4;
    for (k=0; k<nnode; k++) {
      v[k] = (k+1 == nnode) ? pF[i].edge[0].v : pF[i].edge[k+1].v;
      if (v[k] * pF[i].edge[k].v < 0) v[k] = -v[k];
      f[k] = pF[i].edge[k].f;
    }
    for (k=0; k<nnode; k++) {
      pF[i].edge[nnode-1-k].v = v[k];
      pF[i].edge[nnode-1-k].f = f[k];
    }
  }
}

// HepPolyhedron & HepPolyhedron::Transform(const Transform3D &t)
/***********************************************************************
 *                                                                     *
 * Name: HepPolyhedron::Transform                    Date:    01.12.99  *
 * Author: E.Chernyaev                              Revised:           *
 *                                                                     *
 * Function: Make transformation of the polyhedron                     *
 *                                                                     *
 ***********************************************************************/
// {
//   if (nvert > 0) {
//     for (int i=1; i<=nvert; i++) { pV[i] = t * pV[i]; }

//     //  C H E C K   D E T E R M I N A N T   A N D
//     //  I N V E R T   F A C E T S   I F   I T   I S   N E G A T I V E

//     Vector3D<double> d = t * Vector3D<double>(0,0,0);
//     Vector3D<double> x = t * Vector3D<double>(1,0,0) - d;
//     Vector3D<double> y = t * Vector3D<double>(0,1,0) - d;
//     Vector3D<double> z = t * Vector3D<double>(0,0,1) - d;
//     if ((x.cross(y))*z < 0) InvertFacets();
//   }
//   return *this;
// }





int
HepPolyhedron::createTwistedTrap(double Dz,
                                 const double xy1[][2],
                                 const double xy2[][2])
/***********************************************************************
 *                                                                     *
 * Name: createTwistedTrap                           Date:    05.11.02 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised:          *
 *                                                                     *
 * Function: Creates polyhedron for twisted trapezoid                  *
 *                                                                     *
 * Input: Dz       - half-length along Z             8----7            *
 *        xy1[2,4] - quadrilateral at Z=-Dz       5----6  !            *
 *        xy2[2,4] - quadrilateral at Z=+Dz       !  4-!--3            *
 *                                                1----2               *
 *                                                                     *
 ***********************************************************************/
{
  AllocateMemory(12,18);
/*
  pV[ 1] = Point3D<double>(xy1[0][0],xy1[0][1],-Dz);
  pV[ 2] = Point3D<double>(xy1[1][0],xy1[1][1],-Dz);
  pV[ 3] = Point3D<double>(xy1[2][0],xy1[2][1],-Dz);
  pV[ 4] = Point3D<double>(xy1[3][0],xy1[3][1],-Dz);

  pV[ 5] = Point3D<double>(xy2[0][0],xy2[0][1], Dz);
  pV[ 6] = Point3D<double>(xy2[1][0],xy2[1][1], Dz);
  pV[ 7] = Point3D<double>(xy2[2][0],xy2[2][1], Dz);
  pV[ 8] = Point3D<double>(xy2[3][0],xy2[3][1], Dz);

  pV[ 9] = (pV[1]+pV[2]+pV[5]+pV[6])/4.;
  pV[10] = (pV[2]+pV[3]+pV[6]+pV[7])/4.;
  pV[11] = (pV[3]+pV[4]+pV[7]+pV[8])/4.;
  pV[12] = (pV[4]+pV[1]+pV[8]+pV[5])/4.;

  enum {DUMMY, BOTTOM,
        LEFT_BOTTOM,  LEFT_FRONT,   LEFT_TOP,  LEFT_BACK,
        BACK_BOTTOM,  BACK_LEFT,    BACK_TOP,  BACK_RIGHT,
        RIGHT_BOTTOM, RIGHT_BACK,   RIGHT_TOP, RIGHT_FRONT,
        FRONT_BOTTOM, FRONT_RIGHT,  FRONT_TOP, FRONT_LEFT,
        TOP};

  pF[ 1]=G4Facet(1,LEFT_BOTTOM, 4,BACK_BOTTOM, 3,RIGHT_BOTTOM, 2,FRONT_BOTTOM);

  pF[ 2]=G4Facet(4,BOTTOM,     -1,LEFT_FRONT,  -12,LEFT_BACK,    0,0);
  pF[ 3]=G4Facet(1,FRONT_LEFT, -5,LEFT_TOP,    -12,LEFT_BOTTOM,  0,0);
  pF[ 4]=G4Facet(5,TOP,        -8,LEFT_BACK,   -12,LEFT_FRONT,   0,0);
  pF[ 5]=G4Facet(8,BACK_LEFT,  -4,LEFT_BOTTOM, -12,LEFT_TOP,     0,0);

  pF[ 6]=G4Facet(3,BOTTOM,     -4,BACK_LEFT,   -11,BACK_RIGHT,   0,0);
  pF[ 7]=G4Facet(4,LEFT_BACK,  -8,BACK_TOP,    -11,BACK_BOTTOM,  0,0);
  pF[ 8]=G4Facet(8,TOP,        -7,BACK_RIGHT,  -11,BACK_LEFT,    0,0);
  pF[ 9]=G4Facet(7,RIGHT_BACK, -3,BACK_BOTTOM, -11,BACK_TOP,     0,0);

  pF[10]=G4Facet(2,BOTTOM,     -3,RIGHT_BACK,  -10,RIGHT_FRONT,  0,0);
  pF[11]=G4Facet(3,BACK_RIGHT, -7,RIGHT_TOP,   -10,RIGHT_BOTTOM, 0,0);
  pF[12]=G4Facet(7,TOP,        -6,RIGHT_FRONT, -10,RIGHT_BACK,   0,0);
  pF[13]=G4Facet(6,FRONT_RIGHT,-2,RIGHT_BOTTOM,-10,RIGHT_TOP,    0,0);

  pF[14]=G4Facet(1,BOTTOM,     -2,FRONT_RIGHT,  -9,FRONT_LEFT,   0,0);
  pF[15]=G4Facet(2,RIGHT_FRONT,-6,FRONT_TOP,    -9,FRONT_BOTTOM, 0,0);
  pF[16]=G4Facet(6,TOP,        -5,FRONT_LEFT,   -9,FRONT_RIGHT,  0,0);
  pF[17]=G4Facet(5,LEFT_FRONT, -1,FRONT_BOTTOM, -9,FRONT_TOP,    0,0);
 
  pF[18]=G4Facet(5,FRONT_TOP, 6,RIGHT_TOP, 7,BACK_TOP, 8,LEFT_TOP);
*/
  return 0;
}

int
HepPolyhedron::createPolyhedron(int Nnodes, int Nfaces,
                                const double xyz[][3],
                                const int  faces[][4])
/***********************************************************************
 *                                                                     *
 * Name: createPolyhedron                            Date:    05.11.02 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised:          *
 *                                                                     *
 * Function: Creates user defined polyhedron                           *
 *                                                                     *
 * Input: Nnodes  - number of nodes                                    *
 *        Nfaces  - number of faces                                    *
 *        nodes[][3] - node coordinates                                *
 *        faces[][4] - faces                                           *
 *                                                                     *
 ***********************************************************************/
{
  AllocateMemory(Nnodes, Nfaces);
  // cpop
  return 0;
  // end cpop
  /*
  if (nvert == 0) return 1;

  for (int i=0; i<Nnodes; i++) {
    pV[i+1] = Point3D<double>(xyz[i][0], xyz[i][1], xyz[i][2]);
  }
  for (int k=0; k<Nfaces; k++) {
    pF[k+1] = G4Facet(faces[k][0],0,faces[k][1],0,faces[k][2],0,faces[k][3],0);
  }
  SetReferences();
  return 0;
  */
}
#undef INTERSECTION

