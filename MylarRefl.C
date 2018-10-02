#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "Style.C"

void MylarRefl() {
  GeneralStyle();

  std::ifstream bothCompFile("./both_components.csv");
  std::ifstream oneCompFile("./one_component.csv");

  std::vector<double> rawWL1;
  std::vector<double> rawWL2;
  std::vector<double> rawRefl1;
  std::vector<double> rawRefl2;
  double x, y;

  while (bothCompFile >> x >> y) {
    rawWL1.push_back(x);
    rawRefl1.push_back(y);
  }
  bothCompFile.close();

  while (oneCompFile >> x >> y) {
    rawWL2.push_back(x);
    rawRefl2.push_back(y);
  }
  oneCompFile.close();

  const unsigned int N_both = rawWL1.size();
  const unsigned int N_one = rawWL2.size();
  double* WL1 = (double*) malloc(N_both * sizeof(double));
  double* Refl1 = (double*) malloc(N_both * sizeof(double));
  double* WL2 = (double*) malloc(N_one * sizeof(double));
  double* Refl2 = (double*) malloc(N_one * sizeof(double));
  for (unsigned int i = 0; i < N_both; i++) {
    WL1[i] = rawWL1.at(i);
    Refl1[i] = rawRefl1.at(i);
  }
  for (unsigned int i = 0; i < N_one; i++) {
    WL2[i] = rawWL2.at(i);
    Refl2[i] = rawRefl2.at(i);
  }

  TGraph* gr1 = new TGraph(N_both, WL1, Refl1);
  TGraph* gr2 = new TGraph(N_one, WL2, Refl2);

  double lowerWL = 360.;
  double upperWL = 780.;
  const unsigned int N_intervals = 50;
  double* finalWL = (double*) malloc((N_intervals + 1) * sizeof(double));
  double* finalRefl = (double*) malloc((N_intervals + 1) * sizeof(double));
  double deltaWL = (upperWL - lowerWL) / N_intervals;
  for (unsigned int i = 0; i < N_intervals + 1; i++) {
    finalWL[i] = lowerWL + i * deltaWL;
    finalRefl[i] = gr1->Eval(finalWL[i], 0, "S") - gr2->Eval(finalWL[i], 0, "S");
    std::cout << finalWL[i] << "\t" << finalRefl[i] << std::endl;
  }

  TGraph* gr = new TGraph(N_intervals, finalWL, finalRefl);
  TCanvas* c = new TCanvas("c", "c", 400, 400);
  StyleLinearLinear(c);
  gr->SetTitle("");
  gr->GetYaxis()->SetRangeUser(0, 100);
  gr->GetXaxis()->SetLimits(300, 800);
  gr->GetXaxis()->SetTitle("Wavelength (nm)");
  gr->GetYaxis()->SetTitle("Reflectivity (%)");
  gr->GetYaxis()->CenterTitle();
  gr->GetXaxis()->CenterTitle();
  gr->SetLineWidth(3);
  gr->SetLineColor(kRed);
  gr->SetMarkerColor(kRed);
  gr->Draw("APL");
  c->SetFixedAspectRatio();
  c->SaveAs("MylarRefl.pdf");
}
