// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4MagErrorStepper.hh,v 2.4 1998/11/12 16:22:21 japost Exp $
// GEANT4 tag $Name: geant4-00 $
//
// Abstract base class (ie Interface)
// -------------------
//    for integrator of particle's equation of motion,
//    used in tracking in space dependent magnetic field
// -----------------------------------------------------
//
// History:
// 09.12.97  W.Wander <wwc@mit.edu>  Created G4MagErrorStepper
// 09.03.98  W.Wander <wwc@mit.edu>  Added AdvanceHelix functionality
// 09.11.98  J.Apostolakis           Moved AdvanceHelix to G4MagHelicalStepper
// $ Id:  $

#ifndef G4MAGERRORSTEPPER_HH
#define G4MAGERRORSTEPPER_HH
#include "globals.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4Mag_EqRhs.hh"
#include "G4ThreeVector.hh"

class G4MagErrorStepper : public G4MagIntegratorStepper
{
  public:

  G4MagErrorStepper(G4Mag_EqRhs *EqRhs,G4int numberOfVariables);
  ~G4MagErrorStepper(){} ;
  
  //   The stepper for the Runge Kutta integration. The stepsize 
  // is fixed, with the Step size given by h.
  //  Integrates ODE starting values y[0 to 6 ]
  // Outputs yout[] and its estimated error yerr[].
  
  void  Stepper(  const G4double y[],
		  const G4double dydx[],
		  const G4double h,
		  G4double yout[],
		  G4double yerr[]  );

  // performs a 'dump' Step without error calculation.
  
  virtual  void  DumbStepper(  const G4double y[],
			       const G4double dydx[],
			       const G4double h,
			       G4double yout[] ) = 0;
  
  G4double DistChord()   const;
private:
  
  // Data stored in order to find the chord
  G4ThreeVector yInitial, yMidPoint, yFinal;
  
  G4int theNumberOfVariables ; 
};
#include  "G4MagErrorStepper.icc"

#endif  /* G4MAGERRORSTEPPER_HH */