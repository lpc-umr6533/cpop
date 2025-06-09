#include "PhysicsList.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4DNAGenericIonsManager.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmDNAPhysics.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "StepMax.hh"
#include "G4SystemOfUnits.hh"
// Geant4-DNA MODELS
#include "G4DNAElastic.hh"
#include "G4DNAChampionElasticModel.hh"
#include "G4DNAScreenedRutherfordElasticModel.hh"
#include "G4DNAExcitation.hh"
#include "G4DNAMillerGreenExcitationModel.hh"
#include "G4DNABornExcitationModel.hh"
#include "G4DNAIonisation.hh"
#include "G4DNABornIonisationModel.hh"
#include "G4DNARuddIonisationModel.hh"
#include "G4DNAChargeDecrease.hh"
#include "G4DNADingfelderChargeDecreaseModel.hh"
#include "G4DNAChargeIncrease.hh"
#include "G4DNADingfelderChargeIncreaseModel.hh"
#include "G4DNAAttachment.hh"
#include "G4DNAMeltonAttachmentModel.hh"
#include "G4DNAVibExcitation.hh"
#include "G4DNASancheExcitationModel.hh"
#include "G4DNAElectronSolvation.hh"
//
#include "G4LossTableManager.hh"
#include "G4EmConfigurator.hh"
#include "G4VEmModel.hh"
#include "G4DummyModel.hh"
#include "G4eIonisation.hh"
#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4BraggIonGasModel.hh"
#include "G4BetheBlochIonGasModel.hh"
#include "G4UrbanMscModel.hh"
#include "G4MollerBhabhaModel.hh"
#include "G4IonFluctuations.hh"
#include "G4UniversalFluctuation.hh"
#include "G4ElectronCapture.hh"
//seb
#include "G4PhysicsListHelper.hh"
#include "G4WentzelVIModel.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4BraggIonModel.hh"
#include "G4BraggModel.hh"
#include "G4BetheBlochModel.hh"
// gamma
#include "G4Gamma.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"
#include "G4RayleighScattering.hh"
#include "G4LivermoreRayleighModel.hh"
#include "G4EmParameters.hh"
#include "G4EmDNAPhysics_option2.hh"
#include "G4EmDNAPhysics_option4.hh"
#include "G4EmDNAPhysics_option6.hh"
// *** Processes and models for Geant4-DNA
// particles
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4GenericIon.hh"
// Warning : the following is needed in order to use EM Physics builders
// e+
#include "G4Positron.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
// end of warning
#include "G4BuilderType.hh"
// factory
#include "G4PhysicsConstructorFactory.hh"
//Include Hadron Physics
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4UnitsTable.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4ParallelWorldPhysics.hh"
#include "G4AutoDelete.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4RunManager.hh"
#include "PhysicsListMessenger.hh"

namespace cpop {

PhysicsList::PhysicsList():
	G4VModularPhysicsList(),
	_messenger(std::make_unique<PhysicsListMessenger>(this))
{
	_stepMaxProcess = new StepMax;
	fElectronCut = 7.4*eV;
}

void PhysicsList::ConstructParticle() {
	G4BosonConstructor::ConstructParticle();
	G4LeptonConstructor::ConstructParticle();
	G4MesonConstructor::ConstructParticle();
	G4BaryonConstructor::ConstructParticle();
	G4IonConstructor::ConstructParticle();
	G4ShortLivedConstructor::ConstructParticle();

	auto* dnagenericIonsManager = G4DNAGenericIonsManager::Instance();
	dnagenericIonsManager->GetIon("hydrogen");
	dnagenericIonsManager->GetIon("alpha+");
	dnagenericIonsManager->GetIon("alpha++");
	dnagenericIonsManager->GetIon("helium");
	dnagenericIonsManager->GetIon("carbon");
	dnagenericIonsManager->GetIon("nitrogen");
	dnagenericIonsManager->GetIon("iron");
	dnagenericIonsManager->GetIon("oxygen");
}

void PhysicsList::ConstructProcess() {
	AddTransportation();
	_physicsList->ConstructProcess();
	AddStepMax();
	AddElectronCapture();
}

void PhysicsList::AddPhysicsList(const G4String &name) {
	if (verboseLevel>1) {
		std::cout << "PhysicsList::AddPhysicsList: <" << name << ">\n";
	}

	if (name == _name) return;

	if (name == "emstandard") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics>(verboseLevel);
	} else if (name == "emstandard_opt1") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics_option1>(verboseLevel);
	} else if (name == "emstandard_opt2") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics_option2>(verboseLevel);
	} else if (name == "emstandard_opt3") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics_option3>(verboseLevel);
	} else if (name == "emstandard_opt4") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics_option4>(verboseLevel);
	} else if (name == "emlivermore") {
		_name = name;
		_physicsList = std::make_unique<G4EmLivermorePhysics>(verboseLevel);
	} else if (name == "empenelope") {
		_name = name;
		_physicsList = std::make_unique<G4EmPenelopePhysics>(verboseLevel);
	} else if (name == "emlowenergy") {
		_name = name;
		_physicsList = std::make_unique<G4EmLowEPPhysics>(verboseLevel);
	} else if (name == "emDNAphysics") {
		_name = name;
		_physicsList = std::make_unique<G4EmDNAPhysics>(verboseLevel);
	} else if (name == "emDNAphysics_opt2") {
		_name = name;
		_physicsList = std::make_unique<G4EmDNAPhysics_option2>(verboseLevel);
	} else if (name == "emDNAphysics_opt4") {
		_name = name;
		_physicsList = std::make_unique<G4EmDNAPhysics_option4>(verboseLevel);
	} else if (name == "emDNAphysics_opt6") {
		_name = name;
		_physicsList = std::make_unique<G4EmDNAPhysics_option6>(verboseLevel);
	} else {
		std::cout << "PhysicsList::AddPhysicsList: <" << name << ">" << " is not defined\n";
	}
}

PhysicsListMessenger &PhysicsList::messenger() {
	return *_messenger;
}

void PhysicsList::AddStepMax() {
	auto theParticleIterator = GetParticleIterator();
	theParticleIterator->reset();
	while ((*theParticleIterator)()) {
		auto* particle = theParticleIterator->value();
		auto* pmanager = particle->GetProcessManager();

		if (_stepMaxProcess->IsApplicable(*particle) && pmanager)
			pmanager ->AddDiscreteProcess(new StepMax(*_stepMaxProcess));
	}
}

void PhysicsList::AddElectronCapture() {
  auto theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();

  while ((*theParticleIterator)()) {
	G4ParticleDefinition* particle = theParticleIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4String particleName = particle->GetParticleName();
		if (particleName == "e-") {
			auto* ecap = new G4ElectronCapture("", fElectronCut);
			pmanager->AddDiscreteProcess(ecap);
		}
  }

  G4cout << "====================================" << G4endl;
  G4cout << "e- cut applied " << G4endl;
  G4cout << "====================================" << G4endl;
}

StepMax *PhysicsList::step_max_process() {
	return _stepMaxProcess;
}

void PhysicsList::SetElectronCut(G4double value) {
  fElectronCut = value;
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

}
