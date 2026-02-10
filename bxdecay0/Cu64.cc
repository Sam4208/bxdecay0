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
#include <bxdecay0/Cu64.h>

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

  void Cu64(i_random & prng_, event & event_, const double tcnuc_, double & tdnuc_)
  {
    double t;
    double tdlev;
    double p;
    double pbeta;
    double tclev;
    double thlev;
    double thnuc;
    
    // Half-life: 12.7006 hours = 45722.16 seconds
    thnuc  = 45722.16;
    tdnuc_ = tcnuc_ - thnuc / std::log(2.) * std::log(prng_());
    tclev  = 0.;
    
    // Branching Ratios (2012Be24):
    // 1. Beta- (Zn-64 g.s.): 38.48%
    // 2. Beta+ (Ni-64 g.s.): 17.52%
    // 3. EC (Ni-64 1346 keV): 0.475%
    // 4. EC (Ni-64 g.s.):    43.53%
    
    pbeta = 100. * prng_();

    // ============================================================
    // MODE 1: Beta- Decay to Zinc-64 (Ground State)
    // Probability: 0.00 -> 38.48
    // ============================================================
    if (pbeta <= 38.48) {
      // Q-value = 579.6 keV. Daughter Z=30 (Zn).
      // decay0_beta args: (prng, event, Energy, Z, t_create, t_half, t_decay_ref)
      decay0_beta(prng_, event_, 0.5796, +30, 0., 0., t);
      goto label_10000;
    }

    // ============================================================
    // MODE 2: Beta+ Decay to Nickel-64 (Ground State)
    // Probability: 38.48 -> 56.00 (Width = 17.52)
    // ============================================================
    if (pbeta <= 38.48 + 17.52) {
      // Q(EC) = 1674.62 keV.
      // Beta+ Endpoint = Q - 1022 keV = 652.6 keV.
      
      // FIX: Use decay0_beta with NEGATIVE Z (-28) to trigger Positron emission.
      // We pass 't' (a variable) as the last argument to satisfy double& reference.
      decay0_beta(prng_, event_, 0.6526, -28.0, 0., 0., t);
      
      goto label_10000;
    }

    // ============================================================
    // MODE 3: Electron Capture to Nickel-64 (Excited State 1346 keV)
    // Probability: 56.00 -> 56.475 (Width = 0.475)
    // ============================================================
    if (pbeta <= 38.48 + 17.52 + 0.475) {
      // EC produces no nuclear beta particle. 
      // We jump directly to the excited state label to emit the Gamma.
      goto label_1346;
    }

    // ============================================================
    // MODE 4: Electron Capture to Nickel-64 (Ground State)
    // Probability: 56.475 -> 100.00
    // ============================================================
    // "Silent" decay (Neutrinos + Atomic X-rays only).
    // No nuclear function call needed.
    goto label_10000;


    // --- Nuclear De-excitation Levels (Ni-64) ---

    label_1346:
      thlev = 0.0; // Lifetime is picoseconds, effectively 0
      
      // Transition: 1345.77 keV Gamma to Ground State
      decay0_nucltransK(prng_, event_, 1.34577, 0.000111, 0.0, 0.0, tclev, thlev, tdlev);
      goto label_10000;

    label_10000:
      return;
  
  } // end of Cu64

} // end of namespace bxdecay0