// Copyright 2020 F. Mauger
// Copyright 2020 M. Hoballah
//
// This program is free software: you  can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free  Software Foundation, either  version 3 of the  License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// decay0_generator.cc

// Ourselves:
#include <bxdecay0/Cu66.h>

// Standard library:
#include <cmath>
#include <sstream>
#include <stdexcept>

// This project:
#include <bxdecay0/PbAtShell.h>
#include <bxdecay0/alpha.h>
#include <bxdecay0/beta.h>
#include <bxdecay0/beta1.h>
#include <bxdecay0/beta2.h>
#include <bxdecay0/beta_1fu.h>
#include <bxdecay0/electron.h>
#include <bxdecay0/event.h>
#include <bxdecay0/gamma.h>
#include <bxdecay0/i_random.h>
#include <bxdecay0/nucltransK.h>
#include <bxdecay0/nucltransKL.h>
#include <bxdecay0/nucltransKLM.h>
#include <bxdecay0/nucltransKLM_Pb.h>
#include <bxdecay0/pair.h>
#include <bxdecay0/particle.h>
#include <bxdecay0/positron.h>

namespace bxdecay0 {

  void Cu66(i_random & prng_, event & event_, const double tcnuc_, double & tdnuc_)
  {
    double t;
    double tdlev;
    double p;
    double pbeta;
    double tclev;
    double thlev;
    double thnuc;
    // Scheme of Cu66 decay (National Nuclear Data Center (NNDC), Brookhaven National Laboratory).
    // levels in keV.
    // Input : tcnuc_ - time of creation of nucleus (sec)
    // Output: tdnuc_ - time of decay of nucleus (sec)
    
    // Half-life of Cu66 is 5.120 minutes (307.2 seconds)
    thnuc  = 307.2;
    tdnuc_ = tcnuc_ - thnuc / std::log(2.) * std::log(prng_());
    tclev  = 0.;
    pbeta  = 100. * prng_();
    
    // Beta decay branches from 66Cu (1+) to 66Zn levels
    
    // 1. Feeding to 2371.7 keV level (0+)
    // Intensity = 0.0037%
    if (pbeta <= 0.0037) {
      decay0_beta(prng_, event_, 0.269, +26, 0., 0., t); // Q=2640.9 - 2371.7 = 269.2 keV
      goto label_2372;
    }

    // 2. Feeding to 1872.2 keV level (2+)
    // Intensity = 0.220%
    if (pbeta <= 0.0037 + 0.220) {
      decay0_beta(prng_, event_, 0.769, +26, 0., 0., t); // Q=2640.9 - 1872.2 = 768.7 keV
      goto label_1872;
    }

    // 3. Feeding to 1039.2 keV level (2+)
    // Intensity = 9.01%
    if (pbeta <= 0.0037 + 0.220 + 9.01) {
      decay0_beta(prng_, event_, 1.602, +26, 0., 0., t); // Q=2640.9 - 1039.2 = 1601.7 keV
      goto label_1039;
    }

    // 4. Feeding to Ground State (0 keV, 0+)
    // Intensity = 90.77% (Derived: 100 - sum of others ~90.77, data says 90.77)
    decay0_beta(prng_, event_, 2.641, +26, 0., 0., t); // Q=2640.9 keV
    goto label_10000;


    // --- Nuclear De-excitation States (Zn-66) ---

    // Level 2371.7 keV (0+)
    label_2372:
      thlev = 0.0; // Lifetime unknown/short
      // Transition: 1332.5 keV to 1039 keV level
      decay0_nucltransK(prng_, event_, 1.3325, 0.000190, 0.0, 0.0, tclev, thlev, tdlev);
      goto label_1039;

    // Level 1872.2 keV (2+)
    label_1872:
      thlev = 0.0; // Lifetime unknown/short
      p  = 100. * prng_();
      // Branching: 
      // 833.0 gamma (I=2.38) vs 1872.2 gamma (I<0.005)
      // Normalizing intensities: 2.38 / (2.38 + 0.005) ~ 99.8% goes to 833 branch
      
      // Transition to 1039 keV level (833 keV gamma)
      if (p <= 99.8) {
        decay0_nucltransK(prng_, event_, 0.8330, 0.000434, 0.0, 0.0, tclev, thlev, tdlev);
        goto label_1039;
      }
      
      // Transition to Ground State (1872.2 keV gamma)
      decay0_nucltransK(prng_, event_, 1.8722, 0.000328, 0.0, 0.0, tclev, thlev, tdlev);
      goto label_10000;

    // Level 1039.2 keV (2+)
    // Half-life: 1.68 ps (essentially 0 for this simulation scale)
    label_1039:
      thlev = 0.0; // 1.68e-12 sec
      // Transition to Ground State (1039.2 keV gamma)
      decay0_nucltransK(prng_, event_, 1.0392, 0.000269, 0.0, 0.0, tclev, thlev, tdlev);
      goto label_10000;

    label_10000:
      return;
  
  } // end of Cu66 function

} // end of namespace bxdecay0