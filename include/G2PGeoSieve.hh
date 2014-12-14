// -*- C++ -*-

/* class G2PGeoSieve
 * This file defines a class G2PGeoSieve.
 * It defines a sieve slit.
 * Derived from G2PGeoBase so G2PDrift can use it as boundary.
 * Use transport coordinate system in this class.
 */

// History:
//   Mar 2013, C. Gu, First public version.
//   Nov 2014, C. Gu, Rewrite it with G2PGeoPlane class.
//

#ifndef G2P_GEOSIEVE_H
#define G2P_GEOSIEVE_H

#include <vector>

#include "G2PGeoPlane.hh"

using namespace std;

class G2PGeoSieve : public G2PGeoPlane
{
public:
    G2PGeoSieve();
    virtual ~G2PGeoSieve();

    virtual int Begin();

    virtual bool CanPass(double *V3);
    virtual bool CanPass(double *V5, int &id);

    // Gets
    int GetNRow();
    int GetNCol();
    double GetZ();
    void GetPos(int index, double *V3);

    // Sets

protected:
    virtual void MakePrefix();

    double fHRSAngle;

    int fNRow;
    int fNCol;
    vector<double> fX;
    vector<double> fY;

    int fNLargerHole;
    vector<int> fLargerHole; // index of larger sieve holes

    double fDHole;
    double fDLargerHole;
    double fThreshold;

private:
    static G2PGeoSieve *pG2PGeoSieve;

    ClassDef(G2PGeoSieve, 1)
};

#endif
