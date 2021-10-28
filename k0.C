#if !defined(CLING) || defined(ROOTCLING)
#include "CommonDataFormat/RangeReference.h"
#include "ReconstructionDataFormats/Cascade.h"
#include "ReconstructionDataFormats/PID.h"
#include "ReconstructionDataFormats/V0.h"
#include "SimulationDataFormat/MCCompLabel.h"
#include "SimulationDataFormat/MCTrack.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TString.h"
#include "TTree.h"
#endif

using GIndex = o2::dataformats::VtxTrackIndex;
using V0 = o2::dataformats::V0;
using Cascade = o2::dataformats::Cascade;
using RRef = o2::dataformats::RangeReference<int, int>;
using VBracket = o2::math_utils::Bracket<int>;

void k0(std::string secFile = "o2_secondary_vertex.root")
{

  auto fMCTracks = TFile::Open("sgn_Kine.root");
  auto fSecondaries = TFile::Open(secFile.data());
  auto fITS = TFile::Open("o2trac_its.root");

  // trees
  auto treeMCTracks = (TTree *)fMCTracks->Get("o2sim");
  auto treeSecondaries = (TTree *)fSecondaries->Get("o2sim");
  auto treeITS = (TTree *)fITS->Get("o2sim");

  // labels
  std::vector<o2::MCCompLabel> *labITSvec = nullptr;

  std::vector<o2::MCTrack> *MCtracks = nullptr;
  std::vector<V0> *v0vec = nullptr;

  treeSecondaries->SetBranchAddress("V0s", &v0vec);
  treeMCTracks->SetBranchAddress("MCTrack", &MCtracks);
  treeITS->SetBranchAddress("ITSTrackMCTruth", &labITSvec);

  std::map<std::string, std::vector<o2::MCCompLabel> *> map{{"ITS", labITSvec}};

  // fill MC matrix
  std::vector<std::vector<o2::MCTrack>> mcTracksMatrix;
  auto nev = treeMCTracks->GetEntriesFast();

  mcTracksMatrix.resize(nev);
  for (int n = 0; n < nev; n++)
  { // loop over MC events
    treeMCTracks->GetEvent(n);
    mcTracksMatrix[n].resize(MCtracks->size());
    for (unsigned int mcI{0}; mcI < MCtracks->size(); ++mcI)
    {
      mcTracksMatrix[n][mcI] = MCtracks->at(mcI);
    }
  }

  auto outFile = TFile("Secondaries.root", "recreate");
  TH1D *histInvMass = new TH1D("k0 mass", "rec k0s mass", 40, 0., 5);

  treeSecondaries->GetEntry();
  treeITS->GetEntry();

  for (auto &v0 : *v0vec)
  {
    std::cout << "---------------------------------" << std::endl;
    std::cout << "V0 P: " << v0.getP() << std::endl;
    for (int iV0 = 0; iV0 < 2; iV0++)
    {

      if (map[v0.getProngID(iV0).getSourceName()])
      {
        auto labTrackType = map[v0.getProngID(iV0).getSourceName()];
        auto lab = labTrackType->at(v0.getProngID(iV0).getIndex());
        int trackID, evID, srcID;
        bool fake;
        lab.get(trackID, evID, srcID, fake);
        if (!lab.isNoise() && lab.isValid())
        {
          std::cout << "Track type: " << v0.getProngID(iV0).getSourceName() << std::endl;
          std::cout << "Track PDG: " << mcTracksMatrix[evID][trackID].GetPdgCode() << std::endl;
          std::cout << "Generated MC P: " << mcTracksMatrix[evID][trackID].GetP() << std::endl;
          std::cout << "Track Reco P: " << v0.getProng(iV0).getP() << std::endl;
          std::cout << TMath::Sqrt(v0.calcMass2(0.139570**2, 0.139570**2)) << std::endl;
          histInvMass->Fill(TMath::Sqrt(v0.calcMass2(0.139570**2, 0.139570**2)));
        }
      }
    }
  }
  outFile.cd();
  histInvMass->Write();
  outFile.Close();
}
