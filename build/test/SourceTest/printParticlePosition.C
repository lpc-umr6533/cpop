#include "TCanvas.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TH3D.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"



void printParticlePosition() {
	double minVal = -0.3;
	double maxVal = 0.3;
	int nBin = 100;
	auto c_sum= new TCanvas();
	auto  histo = new TH3D("h", "Particle position in spheroid",nBin,minVal,maxVal,nBin,minVal,maxVal,nBin,minVal,maxVal);
	
	ifstream inp;
	inp.open("position.txt");
	double x,y,z;
	
	while (inp >> x >> y >> z) {
		histo->Fill(x,y,z,1.0);
	}
	histo->Draw();
	inp.close();
}
