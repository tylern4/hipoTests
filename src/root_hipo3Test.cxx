#include <chrono>
#include <iostream>
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include "TLorentzVector.h"
// Hipo libs
#include "hipo3/reader.h"

TH2D* wq2_elec = new TH2D("wq2", "W vs Q^{2}", 500, 0, 4.0, 500, 0, 2.5);
TH1F* w = new TH1F("w", "W", 500, 0, 4.0);
TH1F* w_cut = new TH1F("w_cut", "w_cut", 250, 0, 4.0);
TH2D* w_mm = new TH2D("w_mm", "w_mm", 500, 0, 4.0, 500, 0, 3);
TH1F* missingMass = new TH1F("missingMass", "missingMass", 200, 0, 5.5);
TH1F* missingMass_cut = new TH1F("missingMass_cut", "missingMass_cut", 200, 0, 5.5);
TH2D* sf_elec = new TH2D("sf_elec", "sf_elec", 200, 0, 7.5, 200, 0.0, 0.5);
TH2D* dt_pip_hist = new TH2D("dt_pip_hist", "dt_pip_hist", 200, 0, 5.5, 200, -5.0, 5.0);

TChain* clas12 = new TChain("clas12", "clas12");

static const double MASS_E = 0.000511;
static const double MASS_P = 0.93827203;
static const double MASS_PIP = 0.13957018;
const double c_special_units = 29.9792458;

TLorentzVector e_mu;
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

int root_hipo3Test(std::string file = "test.hipo", double BEAM = 2.2) {
  e_mu.SetXYZT(0.0, 0.0, BEAM, BEAM);
  TLorentzVector e_mu_prime;
  int eventNumber = 0;
  auto start_full = std::chrono::high_resolution_clock::now();
  hipo::reader* reader = new hipo::reader(InFileName.c_str());
  long tot_hipo_events = reader->numEvents();

  hipo::node<int32_t>* pid_node = reader->getBranch<int32_t>(331, 1);
  hipo::node<float>* px_node = reader->getBranch<float>(331, 2);
  hipo::node<float>* py_node = reader->getBranch<float>(331, 3);
  hipo::node<float>* pz_node = reader->getBranch<float>(331, 4);
  hipo::node<float>* vx_node = reader->getBranch<float>(331, 5);
  hipo::node<float>* vy_node = reader->getBranch<float>(331, 6);
  hipo::node<float>* vz_node = reader->getBranch<float>(331, 7);
  hipo::node<int8_t>* charge_node = reader->getBranch<int8_t>(331, 8);
  hipo::node<float>* beta_node = reader->getBranch<float>(331, 9);
  hipo::node<float>* chi2pid_node = reader->getBranch<float>(331, 10);
  hipo::node<int16_t>* status_node = reader->getBranch<int16_t>(331, 11);

  hipo::node<int16_t>* cal_pindex_node = reader->getBranch<int16_t>(332, 2);
  hipo::node<int8_t>* cal_detector_node = reader->getBranch<int8_t>(332, 3);
  hipo::node<float>* cal_energy_node = reader->getBranch<float>(332, 6);

  hipo::node<int16_t>* scint_pindex_node = reader->getBranch<int16_t>(335, 2);
  hipo::node<int8_t>* scint_detector_node = reader->getBranch<int8_t>(335, 3);
  hipo::node<int8_t>* scint_layer_node = reader->getBranch<int8_t>(335, 5);
  hipo::node<float>* scint_time_node = reader->getBranch<float>(335, 8);
  hipo::node<float>* scint_path_node = reader->getBranch<float>(335, 9);

  while (reader->next()) {
    if (pid_node->getLength() == 0) continue;
    if (charge_node->getValue(0) != -1) continue;
    eventNumber++;
    e_mu_prime.SetXYZM(px_node->getValue(0), py_node->getValue(0), pz_node->getValue(0), MASS_E);

    double energy = 0;
    for (int icalo = 0; icalo < cal_pindex_node->getLength(); icalo++) {
      if (cal_pindex_node->getValue(icalo) == 0) {
        if (cal_detector_node->getValue(icalo) == 7) {
          energy += cal_energy_node->getValue(icalo);
        }
      }
    }

    if (energy != 0) sf_elec->Fill(e_mu_prime.P(), ec_tot_energy_node->getValue(0) / e_mu_prime.P());

    double e_vertex = NaN;
    for (int iscint = 0; iscint < scint.getMaxSize(); iscint++) {
      if (scint_pindex_node->getValue(iscint) == 0) {
        if (scint_detector_node->getValue(iscint) == 12) {
          if (scint_layer_node->getValue(iscint) == 2) {
            double sc_t = scint_time_node->getValue(iscint);
            double sc_r = scint_path_node->getValue(iscint);
            e_vertex = vertex_time(sc_t, sc_r, 1.0);
            break;
          }
        }
      }
    }

    if ((energy / e_mu_prime.P()) < 0.2 || (energy / e_mu_prime.P()) > 0.3) continue;
    wq2_elec->Fill(W_calc(e_mu_prime), Q2_calc(e_mu_prime));
    w->Fill(W_calc(e_mu_prime));

    TLorentzVector* pionP = new TLorentzVector();
    int numpip = 0;
    int numOther = 0;
    for (int part = 1; part < pid->size(); part++) {
      pionP->SetXYZM(px->getValue(part), py->getValue(part), pz->getValue(part), MASS_PIP);

      double pip_dt = NAN;
      double sc_t = NAN;
      double sc_r = NAN;
      if (charge->getValue(part) == 1) {
        for (int iscint = 0; iscint < scint.getMaxSize(); iscint++) {
          if (scint_pindex_node->getValue(iscint) == part) {
            if (scint_detector_node->getValue(iscint) == 12) {
              if (scint_layer_node->getValue(iscint) == 2) {
                sc_t = scint_time_node->getValue(iscint);
                sc_r = scint_path_node->getValue(iscint);
                break;
              }
            }
          }
        }
        pip_dt = delta_t_calc(e_vertex, pionP->P(), sc_t, sc_r, MASS_PIP);
        dt_pip_hist->Fill(pionP->P(), pip_dt);
      }

      if (abs(pip_dt) < 0.5 && charge->getValue(part) == 1) {
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
  }

  std::chrono::duration<double> elapsed_full = (std::chrono::high_resolution_clock::now() - start_full);
  std::cout << "Elapsed time for " << eventNumber << " events: " << elapsed_full.count() << " s" << std::endl;
  std::cout << "Events/Sec: " << (eventNumber / elapsed_full.count()) << " Hz" << std::endl;

  TCanvas* can = new TCanvas("can", "can", 1600, 900);
  can->Divide(3, 2);
  can->cd(1);
  sf_elec->Draw("colz");
  TLine* sf_lint_t = new TLine(0, 0.2, 7.5, 0.2);
  sf_lint_t->SetLineColor(38);
  sf_lint_t->SetLineWidth(4);
  sf_lint_t->Draw("same");
  TLine* sf_lint_b = new TLine(0, 0.3, 7.5, 0.3);
  sf_lint_b->SetLineColor(38);
  sf_lint_b->SetLineWidth(4);
  sf_lint_b->Draw("same");
  can->cd(2);
  wq2_elec->Draw("colz");
  can->cd(3);
  w->Draw();
  can->cd(4);
  dt_pip_hist->Draw("colz");
  TLine* lint_t = new TLine(0, 0.5, 5.5, 0.5);
  lint_t->SetLineColor(38);
  lint_t->SetLineWidth(4);
  lint_t->Draw("same");
  TLine* lint_b = new TLine(0, -0.5, 5.5, -0.5);
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

#ifndef __CLING__
  TFile* outFile = new TFile("rootTest_output.root", "RECREATE");
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
#endif

  return 0;
}

#ifndef __CLING__
int main(int argc, char const* argv[]) {
  if (argc < 3) {
    std::cerr << "Not enough arguments" << std::endl;
    std::cerr << "To Use:\n\trootTest dst2root_file.root beam_energy" << std::endl;
    exit(1);
  }
  return root_hipo3Test(argv[1], atof(argv[2]));
}
#endif
