
void run(std::string files = "files.root", double beam_energy = 7.5) {
  TChain *ch = new TChain("clas12", "My Chain");
  ch->Add(files.c_str());

  auto plite = TProof::Open("");
  ch->SetProof();
  ch->Process("clas12_selector.cxx+");
}
