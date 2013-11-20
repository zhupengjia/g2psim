// -*- C++ -*-

/* class G2PMaterial
 * Calculate energy loss and multi-scattering for defined material.
 * Most of the algorithm is from SAMC package.
 * Thanks to the author of SAMC.
 */

// History:
//   Sep 2013, C. Gu, First public version.
//   Oct 2013, J. Liu, Check formulas.
//

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "TROOT.h"
#include "TError.h"
#include "TObject.h"

#include "G2PAppBase.hh"
#include "G2PAppList.hh"
#include "G2PMaterial.hh"
#include "G2PRand.hh"
#include "G2PVarDef.hh"

using namespace std;

static const double kELECTRONMASS = 0.510998918; //MeV

G2PAppList* G2PMaterial::pG2PMaterial = new G2PAppList();

G2PMaterial::G2PMaterial() : fName(NULL)
{
    // Only for ROOT I/O
}

G2PMaterial::G2PMaterial(const char* name, double z, double a, double x0, double density) :
fName(name), fZ(z), fA(a), fMass(0), fDensity(density), fX0(x0)
{
    // Constructor

    pG2PMaterial->Add(this);
}

G2PMaterial::~G2PMaterial()
{
    // Destructor

    pG2PMaterial->Remove(this);
}

double G2PMaterial::EnergyLoss(double E, double l)
{
    double EMeV = E * 1000; // MeV

    double result = 0;
    result += Ionization(EMeV, l);
    result += Bremsstrahlung(EMeV, l);

    return result / 1000.0; // GeV
}

double G2PMaterial::MultiScattering(double E, double l)
{
    // only for electron

    double EMeV = E * 1000;

    double thicknessr = l * fDensity / fX0;

    double lPsq = EMeV * EMeV - kELECTRONMASS*kELECTRONMASS;
    double bcp = lPsq / EMeV;
    double ltheta0 = 13.6 / bcp * sqrt(thicknessr)*(1 + 0.038 * log(thicknessr));
    if (thicknessr != 0)
        return pRand->Gaus(0, ltheta0); // rad
    else
        return 0.0;
}

double G2PMaterial::Ionization(double E, double l)
{
    // Particle Data Group Booklet Equ (27.9)

    double thickness = l*fDensity;

    double lK = 0.307075; // cm^2/g for A=1 g/mol
    double lbetasq = 1 - kELECTRONMASS * kELECTRONMASS / (E * E);
    double lxi = lK / 2 * fZ / fA * thickness / lbetasq; // fThickness: g/cm^2
    double lhbarwsq = 28.816 * 28.816 * fDensity * fZ / fA * 1e-12; // MeV // fDensity is density of absorber
    double j = 0.200;
    double Delta_p = lxi * (log(2 * kELECTRONMASS * lxi / lhbarwsq) + j);
    double lw = 4 * lxi;
    double result = 0;
    if (fZ != 0 && fA != 0 && thickness != 0 && fDensity != 0)
        result = pRand->Landau(Delta_p, lw);
    if (result > (E - kELECTRONMASS))
        result = E - kELECTRONMASS;
    if (result < 0)
        result = 0;

    return result; // GeV!
}

double G2PMaterial::Bremsstrahlung(double E, double l)
{
    // Bremsstrahlung Energy Loss for external and internal(equivalent radiator)
    // Xiaodong Jiang, PhD. thesis Equ (5.15)
    // http://filburt.mit.edu/oops/Html/Pub/theses/xjiang.ps
    // *0.999 to avoid lose all energy

    double bt = l * fDensity / fX0 * b();

    double result = 0;
    if (bt != 0)
        result = E * pow(pRand->Uniform()*0.999, 1. / bt);
    if (result > (E - kELECTRONMASS)) // GeV!
        result = E - kELECTRONMASS;
    if (result < 0)
        result = 0;

    return result; // GeV!
}

double G2PMaterial::b()
{
    // Rev. Mod. Phys. 46(1974)815

    double Lrad, Lradp;

    if (fZ != 0) {
        if (fZ <= 2) {
            Lrad = (4.79 - 5.31) * (fZ - 1) + 5.31;
            Lradp = (5.621 - 6.144) * (fZ - 1) + 6.144;
        } else if (fZ <= 3) {
            Lrad = (4.74 - 4.79) * (fZ - 2) + 4.79;
            Lradp = (5.805 - 5.621) * (fZ - 2) + 5.621;
        } else if (fZ <= 4) {
            Lrad = (4.71 - 4.74) * (fZ - 3) + 4.74;
            Lradp = (5.924 - 5.805) * (fZ - 3) + 5.805;
        } else {
            Lrad = log(184.15 * pow(fZ, -1.0 / 3.0));
            Lradp = log(1194.0 * pow(fZ, -2.0 / 3.0));
        }

        return (4.0 / 3.0) * (1.0 + (1.0 / 9.0) * (fZ + 1) / (Lrad * fZ + Lradp));
    }

    return 0;
}

int G2PMaterial::Configure(EMode mode)
{
    if (mode == kREAD || mode == kTWOWAY) {
        if (fIsInit) return 0;

        else fIsInit = true;
    }

    ConfDef confs[] = {
        {"z", "Z", kINT, &fZ},
        {"a", "A", kINT, &fA},
        {"mass", "Mass", kDOUBLE, &fMass},
        {"density", "Density", kDOUBLE, &fDensity},
        {"radlen", "Radiation Length", kDOUBLE, &fX0},
        {0}
    };

    return ConfigureFromList(confs, mode);
}

void G2PMaterial::MakePrefix()
{
    const char* base = "material";

    G2PAppBase::MakePrefix(Form("%s.%s", base, fName));
}

ClassImp(G2PMaterial)
