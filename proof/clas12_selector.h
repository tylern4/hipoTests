//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan  7 11:52:42 2019 by ROOT version 6.15/01
// from TTree clas12/clas12
// found on file: run_5771.root
//////////////////////////////////////////////////////////

#ifndef clas12_selector_h
#define clas12_selector_h

#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include "TLine.h"

// Headers needed by this particular selector
#include <vector>

static const double MASS_E = 0.000511;
static const double MASS_P = 0.93827203;
static const double MASS_PIP = 0.13957018;
const double c_special_units = 29.9792458;

TLorentzVector e_mu(0.0, 0.0, 10.6, 10.6);
TLorentzVector p_mu(0.0, 0.0, 0.0, MASS_P);

double Q2_calc(TLorentzVector e_mu_prime) {
  TLorentzVector q_mu = (e_mu - e_mu_prime);
  return -q_mu.Mag2();
}

double W_calc(TLorentzVector e_mu_prime) {
  TLorentzVector q_mu = (e_mu - e_mu_prime);
  return (p_mu + q_mu).Mag();
}

double vertex_time(double sc_time, double sc_pathlength, double relatavistic_b) {
  return sc_time - sc_pathlength / (relatavistic_b * c_special_units);
}

double delta_t_calc(double vertex, double momentum, double sc_time, double sc_pathlength, double mass) {
  double b = 1.0 / sqrt(1.0 + (mass / momentum) * (mass / momentum));
  double dt = vertex - vertex_time(sc_time, sc_pathlength, b);
  return dt;
}

double missM(TLorentzVector e_mu_prime, TLorentzVector pionP) {
  TLorentzVector react = ((e_mu - e_mu_prime) + p_mu) - pionP;
  return react.M();
}

class clas12_selector : public TSelector {
 public:
  TTreeReader fReader;  //! the tree reader
  TTree *fChain = 0;    //! pointer to the analyzed TTree or TChain
  TH2D *wq2_elec;
  TH1F *w;
  TH1F *w_cut;
  TH2D *w_mm;
  TH1F *missingMass;
  TH1F *missingMass_cut;
  TH2D *sf_elec;
  TH2D *dt_pip_hist;

  // Readers to access the data (delete the ones you do not need).
  TTreeReaderArray<int> pid = {fReader, "pid"};
  TTreeReaderArray<float> p = {fReader, "p"};
  TTreeReaderArray<float> p2 = {fReader, "p2"};
  TTreeReaderArray<float> px = {fReader, "px"};
  TTreeReaderArray<float> py = {fReader, "py"};
  TTreeReaderArray<float> pz = {fReader, "pz"};
  TTreeReaderArray<float> vx = {fReader, "vx"};
  TTreeReaderArray<float> vy = {fReader, "vy"};
  TTreeReaderArray<float> vz = {fReader, "vz"};
  TTreeReaderArray<int> charge = {fReader, "charge"};
  TTreeReaderArray<float> beta = {fReader, "beta"};
  TTreeReaderArray<float> chi2pid = {fReader, "chi2pid"};
  TTreeReaderArray<int> status = {fReader, "status"};
  TTreeReaderArray<float> ec_tot_energy = {fReader, "ec_tot_energy"};
  TTreeReaderArray<float> ec_pcal_energy = {fReader, "ec_pcal_energy"};
  TTreeReaderArray<int> ec_pcal_sec = {fReader, "ec_pcal_sec"};
  TTreeReaderArray<float> ec_pcal_time = {fReader, "ec_pcal_time"};
  TTreeReaderArray<float> ec_pcal_path = {fReader, "ec_pcal_path"};
  TTreeReaderArray<float> ec_pcal_x = {fReader, "ec_pcal_x"};
  TTreeReaderArray<float> ec_pcal_y = {fReader, "ec_pcal_y"};
  TTreeReaderArray<float> ec_pcal_z = {fReader, "ec_pcal_z"};
  TTreeReaderArray<float> ec_pcal_lu = {fReader, "ec_pcal_lu"};
  TTreeReaderArray<float> ec_pcal_lv = {fReader, "ec_pcal_lv"};
  TTreeReaderArray<float> ec_pcal_lw = {fReader, "ec_pcal_lw"};
  TTreeReaderArray<float> ec_ecin_energy = {fReader, "ec_ecin_energy"};
  TTreeReaderArray<int> ec_ecin_sec = {fReader, "ec_ecin_sec"};
  TTreeReaderArray<float> ec_ecin_time = {fReader, "ec_ecin_time"};
  TTreeReaderArray<float> ec_ecin_path = {fReader, "ec_ecin_path"};
  TTreeReaderArray<float> ec_ecin_x = {fReader, "ec_ecin_x"};
  TTreeReaderArray<float> ec_ecin_y = {fReader, "ec_ecin_y"};
  TTreeReaderArray<float> ec_ecin_z = {fReader, "ec_ecin_z"};
  TTreeReaderArray<float> ec_ecin_lu = {fReader, "ec_ecin_lu"};
  TTreeReaderArray<float> ec_ecin_lv = {fReader, "ec_ecin_lv"};
  TTreeReaderArray<float> ec_ecin_lw = {fReader, "ec_ecin_lw"};
  TTreeReaderArray<float> ec_ecout_energy = {fReader, "ec_ecout_energy"};
  TTreeReaderArray<int> ec_ecout_sec = {fReader, "ec_ecout_sec"};
  TTreeReaderArray<float> ec_ecout_time = {fReader, "ec_ecout_time"};
  TTreeReaderArray<float> ec_ecout_path = {fReader, "ec_ecout_path"};
  TTreeReaderArray<float> ec_ecout_x = {fReader, "ec_ecout_x"};
  TTreeReaderArray<float> ec_ecout_y = {fReader, "ec_ecout_y"};
  TTreeReaderArray<float> ec_ecout_z = {fReader, "ec_ecout_z"};
  TTreeReaderArray<float> ec_ecout_lu = {fReader, "ec_ecout_lu"};
  TTreeReaderArray<float> ec_ecout_lv = {fReader, "ec_ecout_lv"};
  TTreeReaderArray<float> ec_ecout_lw = {fReader, "ec_ecout_lw"};
  TTreeReaderArray<int> dc_sec = {fReader, "dc_sec"};
  TTreeReaderArray<float> dc_px = {fReader, "dc_px"};
  TTreeReaderArray<float> dc_py = {fReader, "dc_py"};
  TTreeReaderArray<float> dc_pz = {fReader, "dc_pz"};
  TTreeReaderArray<float> dc_vx = {fReader, "dc_vx"};
  TTreeReaderArray<float> dc_vy = {fReader, "dc_vy"};
  TTreeReaderArray<float> dc_vz = {fReader, "dc_vz"};
  TTreeReaderArray<float> cvt_px = {fReader, "cvt_px"};
  TTreeReaderArray<float> cvt_py = {fReader, "cvt_py"};
  TTreeReaderArray<float> cvt_pz = {fReader, "cvt_pz"};
  TTreeReaderArray<float> cvt_vx = {fReader, "cvt_vx"};
  TTreeReaderArray<float> cvt_vy = {fReader, "cvt_vy"};
  TTreeReaderArray<float> cvt_vz = {fReader, "cvt_vz"};
  TTreeReaderArray<float> cc_nphe_tot = {fReader, "cc_nphe_tot"};
  TTreeReaderArray<int> cc_ltcc_sec = {fReader, "cc_ltcc_sec"};
  TTreeReaderArray<float> cc_ltcc_nphe = {fReader, "cc_ltcc_nphe"};
  TTreeReaderArray<float> cc_ltcc_time = {fReader, "cc_ltcc_time"};
  TTreeReaderArray<float> cc_ltcc_path = {fReader, "cc_ltcc_path"};
  TTreeReaderArray<float> cc_ltcc_theta = {fReader, "cc_ltcc_theta"};
  TTreeReaderArray<float> cc_ltcc_phi = {fReader, "cc_ltcc_phi"};
  TTreeReaderArray<int> cc_htcc_sec = {fReader, "cc_htcc_sec"};
  TTreeReaderArray<float> cc_htcc_nphe = {fReader, "cc_htcc_nphe"};
  TTreeReaderArray<float> cc_htcc_time = {fReader, "cc_htcc_time"};
  TTreeReaderArray<float> cc_htcc_path = {fReader, "cc_htcc_path"};
  TTreeReaderArray<float> cc_htcc_theta = {fReader, "cc_htcc_theta"};
  TTreeReaderArray<float> cc_htcc_phi = {fReader, "cc_htcc_phi"};
  TTreeReaderArray<int> cc_rich_sec = {fReader, "cc_rich_sec"};
  TTreeReaderArray<float> cc_rich_nphe = {fReader, "cc_rich_nphe"};
  TTreeReaderArray<float> cc_rich_time = {fReader, "cc_rich_time"};
  TTreeReaderArray<float> cc_rich_path = {fReader, "cc_rich_path"};
  TTreeReaderArray<float> cc_rich_theta = {fReader, "cc_rich_theta"};
  TTreeReaderArray<float> cc_rich_phi = {fReader, "cc_rich_phi"};
  TTreeReaderArray<int> sc_ftof_1a_sec = {fReader, "sc_ftof_1a_sec"};
  TTreeReaderArray<float> sc_ftof_1a_time = {fReader, "sc_ftof_1a_time"};
  TTreeReaderArray<float> sc_ftof_1a_path = {fReader, "sc_ftof_1a_path"};
  TTreeReaderArray<float> sc_ftof_1a_energy = {fReader, "sc_ftof_1a_energy"};
  TTreeReaderArray<int> sc_ftof_1a_component = {fReader, "sc_ftof_1a_component"};
  TTreeReaderArray<float> sc_ftof_1a_x = {fReader, "sc_ftof_1a_x"};
  TTreeReaderArray<float> sc_ftof_1a_y = {fReader, "sc_ftof_1a_y"};
  TTreeReaderArray<float> sc_ftof_1a_z = {fReader, "sc_ftof_1a_z"};
  TTreeReaderArray<float> sc_ftof_1a_hx = {fReader, "sc_ftof_1a_hx"};
  TTreeReaderArray<float> sc_ftof_1a_hy = {fReader, "sc_ftof_1a_hy"};
  TTreeReaderArray<float> sc_ftof_1a_hz = {fReader, "sc_ftof_1a_hz"};
  TTreeReaderArray<int> sc_ftof_1b_sec = {fReader, "sc_ftof_1b_sec"};
  TTreeReaderArray<float> sc_ftof_1b_time = {fReader, "sc_ftof_1b_time"};
  TTreeReaderArray<float> sc_ftof_1b_path = {fReader, "sc_ftof_1b_path"};
  TTreeReaderArray<float> sc_ftof_1b_energy = {fReader, "sc_ftof_1b_energy"};
  TTreeReaderArray<int> sc_ftof_1b_component = {fReader, "sc_ftof_1b_component"};
  TTreeReaderArray<float> sc_ftof_1b_x = {fReader, "sc_ftof_1b_x"};
  TTreeReaderArray<float> sc_ftof_1b_y = {fReader, "sc_ftof_1b_y"};
  TTreeReaderArray<float> sc_ftof_1b_z = {fReader, "sc_ftof_1b_z"};
  TTreeReaderArray<float> sc_ftof_1b_hx = {fReader, "sc_ftof_1b_hx"};
  TTreeReaderArray<float> sc_ftof_1b_hy = {fReader, "sc_ftof_1b_hy"};
  TTreeReaderArray<float> sc_ftof_1b_hz = {fReader, "sc_ftof_1b_hz"};
  TTreeReaderArray<int> sc_ftof_2_sec = {fReader, "sc_ftof_2_sec"};
  TTreeReaderArray<float> sc_ftof_2_time = {fReader, "sc_ftof_2_time"};
  TTreeReaderArray<float> sc_ftof_2_path = {fReader, "sc_ftof_2_path"};
  TTreeReaderArray<float> sc_ftof_2_energy = {fReader, "sc_ftof_2_energy"};
  TTreeReaderArray<int> sc_ftof_2_component = {fReader, "sc_ftof_2_component"};
  TTreeReaderArray<float> sc_ftof_2_x = {fReader, "sc_ftof_2_x"};
  TTreeReaderArray<float> sc_ftof_2_y = {fReader, "sc_ftof_2_y"};
  TTreeReaderArray<float> sc_ftof_2_z = {fReader, "sc_ftof_2_z"};
  TTreeReaderArray<float> sc_ftof_2_hx = {fReader, "sc_ftof_2_hx"};
  TTreeReaderArray<float> sc_ftof_2_hy = {fReader, "sc_ftof_2_hy"};
  TTreeReaderArray<float> sc_ftof_2_hz = {fReader, "sc_ftof_2_hz"};
  TTreeReaderArray<float> sc_ctof_time = {fReader, "sc_ctof_time"};
  TTreeReaderArray<float> sc_ctof_path = {fReader, "sc_ctof_path"};
  TTreeReaderArray<float> sc_ctof_energy = {fReader, "sc_ctof_energy"};
  TTreeReaderArray<int> sc_ctof_component = {fReader, "sc_ctof_component"};
  TTreeReaderArray<float> sc_ctof_x = {fReader, "sc_ctof_x"};
  TTreeReaderArray<float> sc_ctof_y = {fReader, "sc_ctof_y"};
  TTreeReaderArray<float> sc_ctof_z = {fReader, "sc_ctof_z"};
  TTreeReaderArray<float> sc_ctof_hx = {fReader, "sc_ctof_hx"};
  TTreeReaderArray<float> sc_ctof_hy = {fReader, "sc_ctof_hy"};
  TTreeReaderArray<float> sc_ctof_hz = {fReader, "sc_ctof_hz"};
  TTreeReaderArray<float> sc_cnd_time = {fReader, "sc_cnd_time"};
  TTreeReaderArray<float> sc_cnd_path = {fReader, "sc_cnd_path"};
  TTreeReaderArray<float> sc_cnd_energy = {fReader, "sc_cnd_energy"};
  TTreeReaderArray<int> sc_cnd_component = {fReader, "sc_cnd_component"};
  TTreeReaderArray<float> sc_cnd_x = {fReader, "sc_cnd_x"};
  TTreeReaderArray<float> sc_cnd_y = {fReader, "sc_cnd_y"};
  TTreeReaderArray<float> sc_cnd_z = {fReader, "sc_cnd_z"};
  TTreeReaderArray<float> sc_cnd_hx = {fReader, "sc_cnd_hx"};
  TTreeReaderArray<float> sc_cnd_hy = {fReader, "sc_cnd_hy"};
  TTreeReaderArray<float> sc_cnd_hz = {fReader, "sc_cnd_hz"};
  TTreeReaderArray<float> ft_cal_energy = {fReader, "ft_cal_energy"};
  TTreeReaderArray<float> ft_cal_time = {fReader, "ft_cal_time"};
  TTreeReaderArray<float> ft_cal_path = {fReader, "ft_cal_path"};
  TTreeReaderArray<float> ft_cal_x = {fReader, "ft_cal_x"};
  TTreeReaderArray<float> ft_cal_y = {fReader, "ft_cal_y"};
  TTreeReaderArray<float> ft_cal_z = {fReader, "ft_cal_z"};
  TTreeReaderArray<float> ft_cal_dx = {fReader, "ft_cal_dx"};
  TTreeReaderArray<float> ft_cal_dy = {fReader, "ft_cal_dy"};
  TTreeReaderArray<float> ft_cal_radius = {fReader, "ft_cal_radius"};
  TTreeReaderArray<float> ft_hodo_energy = {fReader, "ft_hodo_energy"};
  TTreeReaderArray<float> ft_hodo_time = {fReader, "ft_hodo_time"};
  TTreeReaderArray<float> ft_hodo_path = {fReader, "ft_hodo_path"};
  TTreeReaderArray<float> ft_hodo_x = {fReader, "ft_hodo_x"};
  TTreeReaderArray<float> ft_hodo_y = {fReader, "ft_hodo_y"};
  TTreeReaderArray<float> ft_hodo_z = {fReader, "ft_hodo_z"};
  TTreeReaderArray<float> ft_hodo_dx = {fReader, "ft_hodo_dx"};
  TTreeReaderArray<float> ft_hodo_dy = {fReader, "ft_hodo_dy"};
  TTreeReaderArray<float> ft_hodo_radius = {fReader, "ft_hodo_radius"};

  clas12_selector(TTree * /*tree*/ = 0) {
    wq2_elec = 0;
    w = 0;
    w_cut = 0;
    w_mm = 0;
    missingMass = 0;
    missingMass_cut = 0;
    sf_elec = 0;
    dt_pip_hist = 0;
  }
  virtual ~clas12_selector() {}
  virtual Int_t Version() const { return 2; }
  virtual void Begin(TTree *tree);
  virtual void SlaveBegin(TTree *tree);
  virtual void Init(TTree *tree);
  virtual Bool_t Notify();
  virtual Bool_t Process(Long64_t entry);
  virtual Int_t GetEntry(Long64_t entry, Int_t getall = 0) {
    return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
  }
  virtual void SetOption(const char *option) { fOption = option; }
  virtual void SetObject(TObject *obj) { fObject = obj; }
  virtual void SetInputList(TList *input) { fInput = input; }
  virtual TList *GetOutputList() const { return fOutput; }
  virtual void SlaveTerminate();
  virtual void Terminate();

  ClassDef(clas12_selector, 0);
};

#endif

#ifdef clas12_selector_cxx
void clas12_selector::Init(TTree *tree) {
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the reader is initialized.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  fReader.SetTree(tree);
}

Bool_t clas12_selector::Notify() {
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

#endif  // #ifdef clas12_selector_cxx
