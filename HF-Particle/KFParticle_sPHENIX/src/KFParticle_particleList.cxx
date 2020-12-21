/*
 * This file is part of KFParticle package
 * Copyright ( C ) 2007-2019 FIAS Frankfurt Institute for Advanced Studies
 *               2007-2019 Goethe University of Frankfurt
 *               2007-2019 Ivan Kisel <I.Kisel@compeng.uni-frankfurt.de>
 *               2007-2019 Maksym Zyzak
 *
 * KFParticle is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * ( at your option ) any later version.
 *
 * KFParticle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY ); without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*****************/
/* Cameron Dean  */
/*   LANL 2020   */
/* cdean@bnl.gov */ 
/*****************/

/** Mass Hypothesis Codes
 ** These are the codes used in the PDG to identify different particles
 ** that have also been coded into KFParticle
 ** PDG Code | Mass Index | Particle
 **         11 |  0 | Electron
 **         13 |  1 | Muon
 **         19 |  1 | Muon
 **        211 |  2 | Pion ( Charged )
 **        321 |  3 | Kaon ( Charged )
 **       2212 |  4 | Proton
 ** 1000010020 |  5 | Deuteron
 ** 1000010030 |  6 | Triton
 ** 1000020030 |  7 | Helium-3
 ** 1000020040 |  8 | Helium-4
 **       3112 |  9 | Sigma ( - )
 **       3222 | 10 | Sigma ( + )
 **       3312 | 11 | Xi
 **       3334 | 12 | Omega
 ** Use PDG codes in your analysis
 ** All other codes return a pion
 ** Get PDG ID's from:
 ** https://pdg.lbl.gov/2019/reviews/rpp2019-rev-monte-carlo-numbering.pdf
 **/

#include "KFParticle_particleList.h"

//KFParticle stuff
#include "KFParticleDatabase.h"

using namespace std;

typedef pair<int, float> particle_pair;

/// KFParticle constructor
KFParticle_particleList::KFParticle_particleList(){}

/// KFParticle destructor
KFParticle_particleList::~KFParticle_particleList(){}

//map<string, float> KFParticle_particleList::getParticleList()
map<string, particle_pair> KFParticle_particleList::getParticleList()
{
  //There is a scope issue here and kfpDatabase must be declared in function unlike in KFPTools
  KFParticleDatabase kfpDatabase;
  map<string, particle_pair> particleMasses; 

  //Leptons
  particleMasses["electron"] = make_pair( 11, kfpDatabase.GetMass(11) );
  particleMasses["muon"]     = make_pair( 13, kfpDatabase.GetMass(13) );
  particleMasses["tau"]      = make_pair( 15, 1.77686 );

  //Gauge bosons and Higgs
  particleMasses["W+"]    = make_pair( 24, 80.379 );
  particleMasses["W-"]    = make_pair( 24, 80.379 );
  particleMasses["Z"]     = make_pair( 23, 91.1876 );
  particleMasses["Higgs"] = make_pair( 25, 125.10 );

  //Light, unflavoured mesons
  particleMasses["pion"]      = make_pair( 211,     kfpDatabase.GetMass(211) );
  particleMasses["pi+"]       = make_pair( 211,     kfpDatabase.GetMass(211) );
  particleMasses["pi-"]       = make_pair( 211,     kfpDatabase.GetMass(211) );
  particleMasses["pi0"]       = make_pair( 111,     kfpDatabase.GetPi0Mass() );
  particleMasses["eta"]       = make_pair( 221,     0.547862 );
  particleMasses["f0(500)"]   = make_pair( 9000221, 0.5 );
  particleMasses["rho"]       = make_pair( 113,     0.77526 );
  particleMasses["rho(770)"]  = make_pair( 113,     0.77526 );
  particleMasses["f0(980)"]   = make_pair( 9010221, 0.990 );
  particleMasses["phi"]       = make_pair( 333,     1.019461 );
  particleMasses["phi(1020)"] = make_pair( 333,     1.019461 );

  //Strange mesons   
  particleMasses["kaon"]    = make_pair( 321, kfpDatabase.GetMass(321) );
  particleMasses["K+"]      = make_pair( 321, kfpDatabase.GetMass(321) );
  particleMasses["K-"]      = make_pair( 321, kfpDatabase.GetMass(321) );
  particleMasses["K0"]      = make_pair( 311, 0.497611 );
  particleMasses["KS0"]     = make_pair( 310, 0.497611 );
  particleMasses["KL0"]     = make_pair( 130, 0.497611 ); //130 is correct according to the PDG
  particleMasses["K*(892)"] = make_pair( 313, 0.89166 );

  //Light baryons
  particleMasses["proton"]  = make_pair( 2212, kfpDatabase.GetMass(2212) );
  particleMasses["neutron"] = make_pair( 2112, 0.93957 );
  particleMasses["Lambda"]  = make_pair( 3122, 1.11568 );
  particleMasses["Sigma+"]  = make_pair( 3222, kfpDatabase.GetMass(3222) );
  particleMasses["Sigma0"]  = make_pair( 3212, 1.192642 );
  particleMasses["Sigma-"]  = make_pair( 3112, kfpDatabase.GetMass(3112) );
  particleMasses["Xi0"]     = make_pair( 3322, 1.31486 );
  particleMasses["Xi+"]     = make_pair( 3312, 1.32171 );
  particleMasses["Xi-"]     = make_pair( 3312, 1.32171 );

  //Charm-hadrons
  particleMasses["D0"]       = make_pair( 421,  1.86483 );
  particleMasses["D0bar"]    = make_pair( 421,  1.86483 );
  particleMasses["D+"]       = make_pair( 411,  1.86965 );
  particleMasses["D-"]       = make_pair( 411,  1.86965 );
  particleMasses["Ds+"]      = make_pair( 431,  1.96834 );
  particleMasses["Ds-"]      = make_pair( 431,  1.96834 );
  particleMasses["D*0"]      = make_pair( 423,  2.00685 );
  particleMasses["D*+"]      = make_pair( 413,  2.01026 );
  particleMasses["D*-"]      = make_pair( 413,  2.01026 );
  particleMasses["Ds*+"]     = make_pair( 433,  2.11220 );
  particleMasses["Ds*-"]     = make_pair( 433,  2.11220 );
  particleMasses["Lc+"]      = make_pair( 4122, 2.28646 );
  particleMasses["Lambdac+"] = make_pair( 4122, 2.28646 );
  particleMasses["Xic0"]     = make_pair( 4132, 2.47090 );
  particleMasses["Xic+"]     = make_pair( 4232, 2.46794 );
  particleMasses["Xic-"]     = make_pair( 4232, 2.46794 );
  particleMasses["Omegac"]   = make_pair( 4332, 2.6952  );
  particleMasses["Xicc++"]   = make_pair( 4422, 3.6212  );

  //B-hadrons
  particleMasses["B+"]       = make_pair( 521,  5.279 ); 
  particleMasses["B-"]       = make_pair( 521,  5.279 );
  particleMasses["B0"]       = make_pair( 511,  5.279 );
  particleMasses["Bs0"]      = make_pair( 531,  5.366 );
  particleMasses["Bc+"]      = make_pair( 541,  6.2749 );
  particleMasses["Bc-"]      = make_pair( 541,  6.2749 );
  particleMasses["Bc"]       = make_pair( 541,  6.2749 );
  particleMasses["Bc(2S)"]   = make_pair( 545,  6.8716 );
  particleMasses["Lambdab0"] = make_pair( 5122,  5.61960 );
  particleMasses["Sigmab+"]  = make_pair( 5222,  5.81056 );
  particleMasses["Sigmab-"]  = make_pair( 5112,  5.81056 );
  particleMasses["Xib0"]     = make_pair( 5232,  5.7919 );
  particleMasses["Xib+"]     = make_pair( 5132,  5.7970 );
  particleMasses["Xib-"]     = make_pair( 5132,  5.7970 );
  particleMasses["Omegab+"]  = make_pair( 5332,  6.0461 );
  particleMasses["Omegab-"]  = make_pair( 5332,  6.0461 );
  
  //Quarkonia
  //c-cbar
  particleMasses["J/psi"]       = make_pair( 443,    3.09690 );
  particleMasses["psi(2S)"]     = make_pair( 100443, 3.68610 );
  particleMasses["X(3872)"]     = make_pair( 0,      3.87169 );
  particleMasses["chic1(3872)"] = make_pair( 0,      3.87169 );
  //b-bbar
  particleMasses["Upsilon(1S)"] = make_pair( 553,      9.46030 );
  particleMasses["Upsilon(2S)"] = make_pair( 100553,  10.02326 );
  particleMasses["Upsilon(3S)"] = make_pair( 200553,  10.3552 );
  particleMasses["Upsilon(4S)"] = make_pair( 300553,  10.5794 );
  particleMasses["Upsilon(5S)"] = make_pair( 9000553, 10.8852 );

  return particleMasses;
}


float KFParticle_particleList::returnPDGMass( const int pdgIndex ) ///Return mother masses from KFParticleDatabase ( pdg.lbl.gov/2019/reviews/rpp2019-rev-monte-carlo-numbering.pdf )
{ 
  KFParticleDatabase kfpDatabase;

   float mass, width;
   kfpDatabase.GetMotherMass( pdgIndex, mass, width );
   return mass;
 }
