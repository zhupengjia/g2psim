#ifndef G2P_SIM_H
#define G2P_SIM_H

#include <cstdio>
#include <cstring>
#include <vector>

#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"

#include "G2PGun.hh"
#include "HRSRecUseDB.hh"
#include "../HRSTransport/HRSTransport.hh"

// definition:
// iArm: Set Arm 0 means left arm, 1 means right arm
// pBPMRes: Set bpm resolution
// fHRSMomentum: Set up HRS Momentum
// iDirection: Set sim direction: 0 means forward, 1 means backward, 2 means both
// iSource: Set data source: 1,2,3 means use delta,flat,gaussian distribution for input position and angle, 0 means use real data in file input_tg.dat and input_fp.dat
// iSetting: Set experiment setting: 10 means normal 484816 septa, 11 means 484816 septa with shim, 12 means 403216 septa with shim, 13 means 400016 septa with shim

//void TestSNAKE(int iNEvent, int iArm, int iSetting, int iSource, int iDirection, double fHRSMomentum, double pBPMRes);

class G2PSim : public TObject
{
public:
    G2PSim();
    ~G2PSim();

    typedef void (G2PSim::*pf_Run)();
    
    void SetNEvent(int n) { nEvent = n; }

    void SetArm(const char *label) { bIsLeftArm = (strcmp(label,"L")==0)?true:false; }
    void SetHRSAngle(double angle) { fHRSAngle = angle; }
    void SetHRSMomentum(double momentum) { fHRSMomentum = momentum; }

    void SetHRSModel(HRSTransport *model) { pHRS = model; }

    void SetGun(G2PGun *gun) { pGun = gun; }
    
    void SetRootName(const char *name) { pFileName = name; }

    bool IsInit() { return bIsInit; }

    void Init();
    void Run();
    void Run(int n) { nEvent = n; Run(); }
    void End();

private:
    void Clear();

    void InitTree();

    void RunSim();
    void RunData();

    bool bIsInit;

    TFile *pFile;
    const char *pFileName; //!

    int nIndex;
    int nEvent;

    bool bIsLeftArm;
    double fHRSAngle;  // Set the same value to optics setting
    double fHRSMomentum;

    G2PGun *pGun;
    double fV3bpm_lab[3];
    double fV5tg_tr[5];
    double fV5tg_lab[5];
    double fV5fpdata_tr[5];
    double fV5fpdata_rot[5];

    HRSTransport *pHRS;
    bool bIsGoodParticle;
    double fV5fp_tr[5];
    double fV5fp_rot[5];
	double fV5rec_tr[5];
    double fV5rec_lab[5];

    HRSRecUseDB *pRecUseDB;
    double fV5recdb_tr[5];
    double fV5recdb_lab[5];

    //HRSXSection *pXSection;
    double fXS;
    
    TTree *pTree;
    TTree *pConfig;

    G2PRand *pRand;
    
    pf_Run pfRunSelector;

    ClassDef(G2PSim,1);
};

#endif