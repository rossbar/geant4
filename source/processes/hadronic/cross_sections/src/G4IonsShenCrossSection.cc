//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// 18-Sep-2003 First version is written by T. Koi
// 12-Nov-2003 Add energy check at lower side T. Koi

#include "G4IonsShenCrossSection.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"


G4double G4IonsShenCrossSection::
GetCrossSection(const G4DynamicParticle* aParticle, const G4Element* anElement, G4double )
{
   G4double xsection = 0.0;

   G4int Ap = aParticle->GetDefinition()->GetBaryonNumber();
   G4int Zp = int ( aParticle->GetDefinition()->GetPDGCharge() / eplus + 0.5 ); 
   G4double ke_per_N = aParticle->GetKineticEnergy() / Ap; 

// Apply energy check, if less than lower limit then 0 value is returned
   if (  ke_per_N < lowerLimit )
      return xsection;

   G4int At = int ( anElement->GetN() + 0.5 );
   G4int Zt = int ( anElement->GetZ() + 0.5 );  

 
   G4double one_third = 1.0 / 3.0;

   G4double cubicrAt = pow ( G4double(At) , G4double(one_third) );  
   G4double cubicrAp = pow ( G4double(Ap) , G4double(one_third) );  

   G4double Rt = 1.12 * cubicrAt - 0.94 * ( 1.0 / cubicrAt );
   G4double Rp = 1.12 * cubicrAp - 0.94 * ( 1.0 / cubicrAp );

   G4double r = Rt + Rp + 3.2;   // in fm
   G4double b = 1.0;   // in MeV/fm

   G4double B = 1.44 * Zt * Zp / r - b * Rt * Rp / ( Rt + Rp ); 

   //G4double ke_per_N = aParticle->GetKineticEnergy() / Ap; 

   G4double c = calCeValue ( ke_per_N / MeV  );  

   G4double R1 = r0 * ( cubicrAt + cubicrAp + 1.85 * cubicrAt * cubicrAp / ( cubicrAt + cubicrAp ) - c); 

   G4double R2 = 1.0 * ( At - 2 * Zt ) * Zp / ( Ap * At );

   G4double targ_mass = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIonMass( Zt , At );
   G4double proj_mass = aParticle->GetMass();
   G4double proj_momentum = aParticle->GetMomentum().mag();

   G4double Ecm = calEcmValue ( proj_mass , targ_mass , proj_momentum ); 

   G4double R3 = 0.176 / pow ( G4double(Ecm) , G4double(one_third) ) * cubicrAt * cubicrAp / ( cubicrAt + cubicrAp );

   G4double R = R1 + R2 + R3;

   xsection = 10 * pi * R * R * ( 1 - B / Ecm );   
   xsection = xsection * millibarn;   // mulitply xsection by millibarn    
  
   return xsection; 
}



G4double G4IonsShenCrossSection::calEcmValue( const G4double mp , const G4double mt , const G4double Plab )
{
   G4double Elab = sqrt ( mp * mp + Plab * Plab );
   G4double Ecm = sqrt ( mp * mp + mt * mt + 2 * Elab * mt );
   G4double Pcm = Plab * mt / Ecm;
   G4double KEcm = sqrt ( Pcm * Pcm + mp * mp ) - mp;
   return KEcm;
}


G4double G4IonsShenCrossSection::calCeValue( const G4double ke )
{
   // Calculate c value 
   // This value is indepenent from projectile and target particle 
   // ke is projectile kinetic energy per nucleon in the Lab system with MeV unit 
   // fitting function is made by T. Koi 
   // There are no data below 30 MeV/n in Kox et al., 

   G4double Ce; 
   G4double log10_ke = log10 ( ke );   
   if ( log10_ke > 1.5 ) 
   {
      Ce = - 10.0 / pow ( G4double(log10_ke) , G4double(5) ) + 2.0;
   }
   else
   {
      Ce = ( - 10.0 / pow ( G4double(1.5) , G4double(5) ) + 2.0 ) / pow ( G4double(1.5) , G4double(3) ) * pow ( G4double(log10_ke) , G4double(3) );
   }
   return Ce;
}