#include "ClassicUserSpectrum.hh"
#include "Randomize.hh"

using namespace std;

ClassicUserSpectrum::ClassicUserSpectrum(G4String file_to_read)
{
	G4int nline;
	G4float proba_read;
	G4float energy_read;
	FILE *file;

	// Read energy and proba
	file=fopen(file_to_read,"r");

	fscanf(file,"%i %i %f",&dim_spectrum,&mode,&emin_spectrum);

	tab_energy= new G4float[dim_spectrum];
	tab_proba= new G4float[dim_spectrum];

	nline=0;

	while(nline<dim_spectrum){
		fscanf(file,"%f %f",&energy_read,&proba_read);
		tab_energy[nline]=energy_read;
		tab_proba[nline]=proba_read;
		nline++;
	}

	fclose(file);  
	 
	// Construct proba table
	sum_proba=0;
	nline=0;

	switch(mode){
		case 1:
			G4cout << "Discrete spectrum" << G4endl;
			tab_sumproba=new G4float[dim_spectrum];
			while(nline<dim_spectrum){
				sum_proba+=tab_proba[nline];
				tab_sumproba[nline]=sum_proba;
				nline++;
			}
			break;
		case 2:
			G4cout << "Histogram spectrum" << G4endl;
			tab_sumproba=new G4float[dim_spectrum];
			sum_proba=tab_proba[0]*(tab_energy[0]-emin_spectrum);
			tab_sumproba[0]=sum_proba;
			for(nline=1;nline<dim_spectrum;nline++){
				sum_proba+=(tab_energy[nline]-tab_energy[nline-1])*tab_proba[nline];
				tab_sumproba[nline]=sum_proba;
			}
			break;
		case 3:
			G4cout << "Interpolated spectrum" << G4endl;
			tab_sumproba= new G4float[dim_spectrum-1];
			for(nline=1;nline<dim_spectrum;nline++){
				sum_proba+=(tab_energy[nline]-tab_energy[nline-1])*tab_proba[nline-1]-0.5*(tab_energy[nline]-tab_energy[nline-1])*(tab_proba[nline-1]-tab_proba[nline]);
				tab_sumproba[nline-1]=sum_proba;
			}
			break;
		default:
			G4cout << "Mode not recognized, check spectrum file!" << G4endl;
			break;
	}
}


ClassicUserSpectrum::~ClassicUserSpectrum(){
delete[] tab_proba;
delete[] tab_sumproba;
delete[] tab_energy;
}

G4float ClassicUserSpectrum::GetEnergy() const
{
	return GetEnergy(G4UniformRand());
}


G4float ClassicUserSpectrum::GetEnergy(G4double my_rndm) const
{
	
	G4double energy=0.;
	G4int i=0;

	while(my_rndm>=(tab_sumproba[i])/sum_proba) i++;


	G4double a,b,alpha,beta,gamma,X;

	switch(mode){
		case 1:
			//Discrete Spectrum
			energy=tab_energy[i];
			break;
		case 2:
			//Histogram spectrum
			if(i==0) 
			{
				energy=(tab_energy[i]-emin_spectrum)*G4UniformRand()+emin_spectrum;
			}
			else 
			{
				energy=(tab_energy[i]-tab_energy[i-1])*G4UniformRand()+tab_energy[i-1];		
			}
			break;
		case 3:
			//Interpolated spectrum
			a=tab_energy[i];
			b=tab_energy[i+1];

			if(tab_proba[i]==tab_proba[i+1])
			{
				if(i==0)
				{
					energy=(tab_energy[0]-emin_spectrum)*G4UniformRand()+emin_spectrum;
				}
				else
				{
					energy=(tab_energy[i]-tab_energy[i-1])*G4UniformRand()+tab_energy[i-1];
				}
			}
			else
			{
				alpha=(tab_proba[i+1]-tab_proba[i])/(tab_energy[i+1]-tab_energy[i]);
				beta=tab_proba[i+1]-tab_energy[i+1]*alpha;
				gamma=(alpha/2)*(b*b-a*a)+beta*(b-a);
				// cout << "alpha = " << alpha << endl;
				// cout << "beta = " << beta << endl;
				// cout << "gamma = " << gamma << endl;
				my_rndm=G4UniformRand();
				X =(-beta+sqrt((alpha*a+beta)*(alpha*a+beta)+2*alpha*gamma*my_rndm))/(alpha);
				if((X-a)*(X-b)<=0)
				{
					energy=X;
				}
				else
				{
				energy=(-beta-sqrt((alpha*a+beta)*(alpha*a+beta)+2*alpha*gamma*my_rndm))/(alpha);
				}
			}
			break;
		default:
			energy=0;
			break;
	}

	// std::cout << " energy returned = " << energy << std::endl;
	return energy;

}


G4float ClassicUserSpectrum::GetEnergy(G4double rnd1, G4double rnd2) const
{
		
	G4double energy=0.;
	G4int i=0;
	G4double my_rndm=rnd1;
	// std::cout << " fist shotted = " << my_rndm << std::endl;
	while(my_rndm>=(tab_sumproba[i])/sum_proba) i++;


	G4double a,b,alpha,beta,gamma,X;

	switch(mode){
		case 1:
			//Discrete Spectrum
			energy=tab_energy[i];
			break;
		case 2:
			//Histogram spectrum
			if(i==0) energy=(tab_energy[0]-emin_spectrum)*rnd2+emin_spectrum;
			else energy=(tab_energy[i]-tab_energy[i-1])*rnd2+tab_energy[i-1];		
			break;
		case 3:
			//Interpolated spectrum
			a=tab_energy[i];
			b=tab_energy[i+1];
			if(tab_proba[i]==tab_proba[i+1])
				{
					if(i==0)
						{
							energy=(tab_energy[0]-emin_spectrum)*rnd2+emin_spectrum;
						}
					else
						{
							energy=(tab_energy[i]-tab_energy[i-1])*rnd2+tab_energy[i-1];
						}
				}
			else
				{
					alpha=(tab_proba[i+1]-tab_proba[i])/(tab_energy[i+1]-tab_energy[i]);
					beta=tab_proba[i+1]-tab_energy[i+1]*alpha;
					gamma=(alpha/2)*(b*b-a*a)+beta*(b-a);
					my_rndm=rnd2;
					X =(-beta+sqrt((alpha*a+beta)*(alpha*a+beta)+2*alpha*gamma*my_rndm))/(alpha);
					if((X-a)*(X-b)<=0)
						{
							energy=X;
						}
					else
						{
						energy=(-beta-sqrt((alpha*a+beta)*(alpha*a+beta)+2*alpha*gamma*my_rndm))/(alpha);
						}
				}
			break;
		default:
			energy=0;
			break;
	}

	// std::cout << " energy returned = " << energy << std::endl;
	return energy;

}
