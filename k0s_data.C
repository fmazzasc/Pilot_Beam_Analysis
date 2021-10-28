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

void k0s_data(std::string secFile = "o2_secondary_vertex.root")
{

  auto fSecondaries = TFile::Open(secFile.data());
  auto treeSecondaries = (TTree *)fSecondaries->Get("o2sim");

  std::vector<V0> *v0vec = nullptr;

  treeSecondaries->SetBranchAddress("V0s", &v0vec);

  std::map<std::string, std::vector<o2::MCCompLabel> *> map{{"ITS", labITSvec}};

  auto outFile = TFile("k0_inv_mass.root", "recreate");
  TH1D *histInvMass = new TH1D("k0 mass", "rec k0s mass", 80, 0.3, 1.);

  treeSecondaries->GetEntry();

  for (auto &v0 : *v0vec)
  {
    std::cout << "---------------------------------" << std::endl;
    std::cout << "V0 P: " << v0.getP() << std::endl;
    histInvMass->Fill(TMath::Sqrt(v0.calcMass2(0.139570 * 0.139570, 0.139570 * 0.139570)));
  }
  outFile.cd();
  histInvMass->Write();
  outFile.Close();
}