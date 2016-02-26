#include "TArrow.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLine.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TRandom3.h"

#include <iostream>
using namespace std;

//*********************************************************
// Read N histograms from a file (Signal and backgrounds);
// Read data histogram 
// prepare stacked histograms (S+B)
// plot histograms
//********************************************************* 



void evjj_inv_mass(){

// get Higgs signal, background and data from file

TDirectory* dir = gDirectory;
TFile *myfile1 = new TFile("./histos_datasets_v2/histos_H550.root", "READ");
dir->cd();
TH1F *h_sig  = (TH1F*) myfile1->Get("evjj_mass")->Clone("h_sig");
myfile1->Close();

TDirectory* dir2 = gDirectory;
TFile *myfile2 = new TFile("./histos_datasets_v2/histos_W2jets_SemPeso.root");
dir2->cd();
TH1F *h_W2jets  = (TH1F*) myfile2->Get("evjj_mass")->Clone("h_W2jets");
myfile2->Close();

TDirectory* dir3 = gDirectory;
TFile *myfile3 = new TFile("./histos_datasets_v2/histos_DATA_btagveto.root");
dir3->cd();
TH1F *h_data = (TH1F*) myfile3->Get("evjj_mass")->Clone("h_data");
myfile3->Close();

TDirectory* dir4 = gDirectory;
TFile *myfile4 = new TFile("./histos_datasets_v2/histos_TTbar_sempeso.root");
dir4->cd();
TH1F *h_ttbar = (TH1F*) myfile4->Get("evjj_mass")->Clone("h_ttbar");
myfile4->Close();

TDirectory* dir5 = gDirectory;
TFile *myfile5 = new TFile("./histos_datasets_v2/histos_WW_sempeso.root");
dir5->cd();
TH1F *h_WW = (TH1F*) myfile5->Get("evjj_mass")->Clone("h_WW");
myfile5->Close();

TDirectory* dir6 = gDirectory;
TFile *myfile6 = new TFile("./histos_datasets_v2/histos_WZ_sempeso.root");
dir6->cd();
TH1F *h_WZ = (TH1F*) myfile6->Get("evjj_mass")->Clone("h_WZ");
myfile6->Close();


int Irebin = 2;

//rebin histograms
h_data->Rebin(Irebin);
h_sig->Rebin(Irebin);
h_W2jets->Rebin(Irebin);
h_ttbar->Rebin(Irebin);
h_WW->Rebin(Irebin);
h_WZ->Rebin(Irebin);


printf("\n INFO: Rebinning the histograms with a factor %d. Binwidth is now %5.2f GeV\n\n", Irebin, h_data->GetBinWidth(1));



// prepare cumulative histogram for signal + W2Jets
TH1D *h_sig_plus_W2jets = (TH1D*) h_W2jets->Clone("h_sig_plus_W2jets");
h_sig_plus_W2jets->Reset();
for (int i_bin = 1; i_bin < h_W2jets->GetNbinsX(); i_bin++){
  h_sig_plus_W2jets->SetBinContent (i_bin, h_sig->GetBinContent(i_bin) + h_W2jets->GetBinContent(i_bin));
}

// prepare cumulative histogram for signal + W2Jets + ttbar
TH1D *h_sig_plus_W2jets_ttbar = (TH1D*) h_sig_plus_W2jets->Clone("h_sig_plus_W2jets_ttbar");
h_sig_plus_W2jets_ttbar->Reset();
for (int i_bin = 1; i_bin < h_sig_plus_W2jets->GetNbinsX(); i_bin++){
  h_sig_plus_W2jets_ttbar->SetBinContent (i_bin, h_ttbar->GetBinContent(i_bin) + h_sig_plus_W2jets->GetBinContent(i_bin));
}

// prepare cumulative histogram for signal + W2Jets + ttbar + WW
TH1D *h_sig_plus_W2jets_ttbar_WW = (TH1D*) h_sig_plus_W2jets_ttbar->Clone("h_sig_plus_W2jets_ttbar");
h_sig_plus_W2jets_ttbar_WW->Reset();
for (int i_bin = 1; i_bin < h_sig_plus_W2jets_ttbar->GetNbinsX(); i_bin++){
  h_sig_plus_W2jets_ttbar_WW->SetBinContent (i_bin, h_WW->GetBinContent(i_bin) + h_sig_plus_W2jets_ttbar->GetBinContent(i_bin));
}

// prepare cumulative histogram for signal + W2Jets + ttbar + WW + WZ
TH1D *h_sig_plus_W2jets_ttbar_WW_WZ = (TH1D*) h_sig_plus_W2jets_ttbar_WW->Clone("h_sig_plus_W2jets_ttbar_WW");
h_sig_plus_W2jets_ttbar_WW_WZ->Reset();
for (int i_bin = 1; i_bin < h_sig_plus_W2jets_ttbar_WW->GetNbinsX(); i_bin++){
  h_sig_plus_W2jets_ttbar_WW_WZ->SetBinContent (i_bin, h_WZ->GetBinContent(i_bin) + h_sig_plus_W2jets_ttbar_WW->GetBinContent(i_bin));
}

// prepare canvas
TCanvas *canvas1 = new TCanvas( "canvas1","Standard Canvas", 600, 400);
canvas1->cd();


//plot histograms (sig + W2jets + ttbar + WW + WZ)
h_WW->SetFillColor(5);
h_W2jets->SetFillColor(5);
h_ttbar->SetFillColor(5);
h_WZ->SetFillColor(5);
h_sig_plus_W2jets_ttbar_WW_WZ->SetFillColor(5);
h_sig_plus_W2jets_ttbar_WW_WZ->SetAxisRange(0.,25000.,"Y");
h_sig_plus_W2jets_ttbar_WW_WZ->SetAxisRange(100., 600., "X");
h_sig_plus_W2jets_ttbar_WW_WZ->Draw("hist");
h_sig_plus_W2jets_ttbar_WW_WZ->SetStats(0);

//add histograms (sig + W2jets + ttbar + WW)
h_WW->SetFillColor(6);
h_W2jets->SetFillColor(6);
h_ttbar->SetFillColor(6);
h_sig_plus_W2jets_ttbar_WW->SetFillColor(6);
h_sig_plus_W2jets_ttbar_WW->Draw("same");
h_sig_plus_W2jets_ttbar_WW->Draw("axis same");
h_sig_plus_W2jets_ttbar_WW->SetStats(0);

//plot histograms (sig + W2jets + ttbar)
h_W2jets->SetFillColor(8);
h_ttbar->SetFillColor(8);
h_sig_plus_W2jets_ttbar->SetFillColor(8);
h_sig_plus_W2jets_ttbar->Draw("same");
h_sig_plus_W2jets_ttbar->Draw("axis same");
h_sig_plus_W2jets_ttbar->SetStats(0);

//add histograms (sig + W2jets)
h_W2jets->SetFillColor(2);
h_sig_plus_W2jets->SetFillColor(2);
h_sig_plus_W2jets->Draw("same");
h_sig_plus_W2jets->Draw("axis same");
h_sig_plus_W2jets->SetStats(0);

//add sig
h_sig->SetFillColor(7);
h_sig->Draw("same");
h_sig->Draw("axis same");
h_data->Draw("e same");
h_sig->SetStats(0);

// add axes
h_sig_plus_W2jets_ttbar->SetTitle("4-body invariant mass (mH = 550 GeV)");
h_sig_plus_W2jets_ttbar->SetXTitle("evjj invariant mass [GeV]");
h_sig_plus_W2jets_ttbar_WW_WZ->SetYTitle(Form("Numero de eventos / %3.1f GeV", h_W2jets->GetBinWidth(1)));

//legend
TLegend *legend = new TLegend(0.65,0.65,0.90,0.80);
legend->SetBorderSize(0);
legend->SetFillColor(0);
TLegendEntry *legend_entry1 = legend->AddEntry(h_sig," Higgs", "f");
TLegendEntry *legend_entry2 = legend->AddEntry(h_W2jets," Wjets", "f");
TLegendEntry *legend_entry3 = legend->AddEntry(h_ttbar, " TTbar", "f");
TLegendEntry *legend_entry4 = legend->AddEntry(h_WW, " WW", "f");
TLegendEntry *legend_entry5 = legend->AddEntry(h_WZ, " WZ", "f");
TLegendEntry *legend_entry6 = legend->AddEntry(h_data, " dados", "f");
legend_entry1->SetTextSize(0.03);
legend_entry2->SetTextSize(0.03);
legend_entry3->SetTextSize(0.03);
legend_entry4->SetTextSize(0.03);
legend_entry5->SetTextSize(0.03);
legend_entry6->SetTextSize(0.03);
legend->Draw();

canvas1->SaveAs("./evjj_mass/btag_veto/4body_hist_h550_v4.png");

return;

}
