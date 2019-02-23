#define clas12_selector_cxx
// The class definition in clas12_selector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("clas12_selector.C")
// root> T->Process("clas12_selector.C","some options")
// root> T->Process("clas12_selector.C+")
//

#include <TH2.h>
#include <TStyle.h>
#include "clas12_selector.h"

void clas12_selector::Begin(TTree * /*tree*/) {
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();
}

void clas12_selector::SlaveBegin(TTree * /*tree*/) {
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

  wq2_elec = new TH2D("wq2", "W vs Q^{2}", 500, 0, 4.0, 500, 0, 2.5);
  fOutput->Add(wq2_elec);
  w = new TH1F("w", "W", 500, 0, 4.0);
  fOutput->Add(w);
  w_cut = new TH1F("w_cut", "w_cut", 250, 0, 4.0);
  fOutput->Add(w_cut);
  w_mm = new TH2D("w_mm", "w_mm", 500, 0, 4.0, 500, 0, 3);
  fOutput->Add(w_mm);
  missingMass = new TH1F("missingMass", "missingMass", 200, 0, 5.5);
  fOutput->Add(missingMass);
  missingMass_cut = new TH1F("missingMass_cut", "missingMass_cut", 200, 0, 5.5);
  fOutput->Add(missingMass_cut);
  sf_elec = new TH2D("sf_elec", "sf_elec", 200, 0, 7.5, 200, 0.0, 0.5);
  fOutput->Add(sf_elec);
  dt_pip_hist = new TH2D("dt_pip_hist", "dt_pip_hist", 200, 0, 5.5, 200, -5.0, 5.0);
  fOutput->Add(dt_pip_hist);
}

Bool_t clas12_selector::Process(Long64_t entry) {
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // When processing keyed objects with PROOF, the object is already loaded
  // and is available via the fObject pointer.
  //
  // This function should contain the \"body\" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.

  fReader.SetEntry(entry);
  TLorentzVector e_mu_prime;
  size_t len = 0;
  for (auto l = pid.begin(); l != pid.end(); l++) len++;

  if (len == 0) return false;
  if (charge[0] != -1) return false;

  e_mu_prime.SetXYZM(px[0], py[0], pz[0], MASS_E);
  sf_elec->Fill(p[0], ec_tot_energy[0] / p[0]);

  double e_vertex = vertex_time(sc_ftof_1b_time[0], sc_ftof_1b_path[0], 1.0);

  if ((ec_tot_energy[0] / p[0]) < 0.2 || (ec_tot_energy[0] / p[0]) > 0.3) return false;
  wq2_elec->Fill(W_calc(e_mu_prime), Q2_calc(e_mu_prime));
  w->Fill(W_calc(e_mu_prime));

  TLorentzVector *pionP = new TLorentzVector();
  int numpip = 0;
  int numOther = 0;
  for (int part = 1; part < len; part++) {
    pionP->SetXYZM(px[part], py[part], pz[part], MASS_PIP);

    double pip_dt = NAN;
    if (charge[part] == 1) {
      double sc_t = sc_ftof_1b_time[part];
      double sc_r = sc_ftof_1b_path[part];
      pip_dt = delta_t_calc(e_vertex, p[part], sc_t, sc_r, MASS_PIP);
      dt_pip_hist->Fill(p[part], pip_dt);
    }

    if (abs(pip_dt) < 0.5 && charge[part] == 1) {
      numpip++;
    } else {
      numOther++;
    }
  }
  if (numpip == 1 && numOther == 0) {
    missingMass->Fill(missM(e_mu_prime, *pionP));
    w_mm->Fill(W_calc(e_mu_prime), missM(e_mu_prime, *pionP));
    if (missM(e_mu_prime, *pionP) >= 0.8 && missM(e_mu_prime, *pionP) <= 1.1) {
      missingMass_cut->Fill(missM(e_mu_prime, *pionP));
      w_cut->Fill(W_calc(e_mu_prime));
    }
  }

  return kTRUE;
}

void clas12_selector::SlaveTerminate() {
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
}

void clas12_selector::Terminate() {
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  TCanvas *can = new TCanvas("can", "can", 1600, 900);
  can->Divide(3, 2);
  can->cd(1);
  sf_elec->Draw("colz");
  TLine *sf_lint_t = new TLine(0, 0.2, 7.5, 0.2);
  sf_lint_t->SetLineColor(38);
  sf_lint_t->SetLineWidth(4);
  sf_lint_t->Draw("same");
  TLine *sf_lint_b = new TLine(0, 0.3, 7.5, 0.3);
  sf_lint_b->SetLineColor(38);
  sf_lint_b->SetLineWidth(4);
  sf_lint_b->Draw("same");
  can->cd(2);
  wq2_elec->Draw("colz");
  can->cd(3);
  w->Draw();
  can->cd(4);
  dt_pip_hist->Draw("colz");
  TLine *lint_t = new TLine(0, 0.5, 5.5, 0.5);
  lint_t->SetLineColor(38);
  lint_t->SetLineWidth(4);
  lint_t->Draw("same");
  TLine *lint_b = new TLine(0, -0.5, 5.5, -0.5);
  lint_b->SetLineColor(38);
  lint_b->SetLineWidth(4);
  lint_b->Draw("same");
  can->cd(5);
  missingMass->Draw();
  missingMass_cut->SetFillColor(38);
  missingMass_cut->Draw("same");
  can->cd(6);
  w_cut->SetFillColor(38);
  w_cut->Draw();

  TFile *outFile = new TFile("rootTest_output.root", "RECREATE");
  outFile->cd();
  can->Write();
  can->SaveAs("rootTest_analysis.png");
  wq2_elec->Write();
  w->Write();
  w_cut->Write();
  w_mm->Write();
  missingMass->Write();
  missingMass_cut->Write();
  sf_elec->Write();
  dt_pip_hist->Write();
  outFile->Write();
  outFile->Close();
}
