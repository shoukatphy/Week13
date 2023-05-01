////////////////////////////////////
//Used TKDE func example from root//
////////////////////////////////////

#include "TKDE.h"
#include "Math/DistFunc.h"
#include "TLegend.h"

#include<iostream>
using namespace std;

 
void estim(int n) {
 
   // generate some gaussian points
 
   int nbin = n/10;
   double xmin = -5;
   double xmax = 5;
 

   TRandom3 *rn = new TRandom3();//for random numbers
    
   TH1D * h1 = new TH1D("h1","h1",nbin,xmin,xmax); //histo for the data points

 
   // generate some points with bi- gaussian distribution
   
    vector<double> data(n);
   for (int i = 0; i < n; ++i) {
    if (i < 0.3*n) {
         data[i] = rn->Gaus(1,0.5);
         h1->Fill(data[i]);
      }
      else {
         data[i] = rn->Gaus(-3,.5);
         h1->Fill(data[i]);
      }
   }
 
   auto *c = new TCanvas();
   
   // scale histogram
  h1->Scale(1./h1->Integral(),"width" );
   h1->SetStats(false);
   h1->SetTitle("Bi-Gaussian data and estimated fit using TKDE()");
   h1->SetMarkerStyle(22);
   h1->SetMarkerColor(4);
   h1->Draw();
 
   // drawn true normalized density
   TF1 * f1 = new TF1("f1","0.4*ROOT::Math::normal_pdf(x,3,0.5)+0.6*ROOT::Math::normal_pdf(x,1,0.5)",xmin,xmax);

   f1->SetLineColor(7);
   f1->Draw("SAME");
 
   // create TKDE class
   double rho = 1.0; //default value
   TKDE * kde = new TKDE(n, &data[0], xmin,xmax, "", rho);
   h1->GetXaxis()->SetTitle("Trials");
   h1->GetYaxis()->SetTitle("Probability");
  kde->Draw("SAME");
 
   TLegend * leg = new TLegend(0.6,0.7,0.9,0.9);
   leg->AddEntry(f1,"True function");
   leg->AddEntry(kde->GetDrawnFunction(),"TKDE");
   leg->AddEntry(h1, "Data points");
   leg->Draw();

  c->SaveAs("TKDE_estimation.pdf");
}
