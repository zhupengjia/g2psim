// -*- C++ -*-

/* class G2PPhysBase
 * Abstract base class of G2PPhys classes.
 * It provides interface functions.
 */

// History:
//   Mar 2013, C. Gu, First public version.
//

#include <cstdlib>
#include <vector>
#include <cmath>

#include "G2PPhysBase.hh"

using namespace std;

static const double kU = 931.49406121;

G2PPhysBase::G2PPhysBase() :
fZ(1), fA(1), fTargetMass(0.0), fPID(11)
{
    fPars.clear();
}

G2PPhysBase::~G2PPhysBase()
{
    // Nothing to do
}

void G2PPhysBase::SetTarget(int Z, int A)
{
    fZ = Z;
    fA = A;

    if (fabs(fTargetMass) < 1.0e-8) SetTargetMass();
}

void G2PPhysBase::SetPars(double* array, int n)
{
    fPars.clear();

    for (int i = 0; i < n; i++) fPars.push_back(array[i]);
}

void G2PPhysBase::SetTargetMass()
{
    double atomicmass = 0.0;

    if (fZ == 1) atomicmass = 1.00794;
    if (fZ == 2) atomicmass = 4.002602;
    if (fZ == 6) atomicmass = 12.0107;
    if (fZ == 7) atomicmass = 14.0067;
    if (fZ == 8) atomicmass = 15.9994;
    if (fZ == 26) atomicmass = 55.845;
    if (fZ == 29) atomicmass = 63.546;
    if (fZ == 74) atomicmass = 183.84;
    if (fabs(atomicmass) < 1.0e-8) atomicmass = fA; // only an estimation

    fTargetMass = atomicmass*kU;
}
