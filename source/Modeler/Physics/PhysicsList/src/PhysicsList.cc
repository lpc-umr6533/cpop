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
#include "G4EmProcessOptions.hh"
#include "G4ParallelWorldPhysics.hh"
#include "G4AutoDelete.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4RunManager.hh"
#include "PhysicsListMessenger.hh"



namespace cpop {

PhysicsList::PhysicsList()
    :G4VModularPhysicsList(),
      messenger_(std::make_unique<PhysicsListMessenger>(this))
{
    step_max_process_ = new StepMax();
}

void PhysicsList::ConstructParticle()
{
    // Construct all bosons
    G4BosonConstructor boson;
    boson.ConstructParticle();

    // Construct all leptons
    G4LeptonConstructor lepton;
    lepton.ConstructParticle();

    //  Construct all mesons
    G4MesonConstructor meson;
    meson.ConstructParticle();

    //  Construct all barions
    G4BaryonConstructor barion;
    barion.ConstructParticle();

    //  Construct light ions
    G4IonConstructor ion;
    ion.ConstructParticle();

    //  Construct  resonaces and quarks
    G4ShortLivedConstructor slive;
    slive.ConstructParticle();

    //Construct G4DNA particles


    G4DNAGenericIonsManager* dnagenericIonsManager;
    dnagenericIonsManager=G4DNAGenericIonsManager::Instance();
    dnagenericIonsManager->GetIon("hydrogen");
    dnagenericIonsManager->GetIon("alpha+");
    dnagenericIonsManager->GetIon("alpha++");
    dnagenericIonsManager->GetIon("helium");
    dnagenericIonsManager->GetIon("carbon");
    dnagenericIonsManager->GetIon("nitrogen");
    dnagenericIonsManager->GetIon("iron");
    dnagenericIonsManager->GetIon("oxygen");
}

void PhysicsList::ConstructProcess()
{
    // Transportation
    //
    AddTransportation();

    // Electromagnetic physics list
    //
    if (name_ != "emDNAphysics_opt2")
    {
    physics_list_->ConstructProcess();
    }
    else if (name_ == "emDNAphysics_opt2")
    {

      G4EmParameters* param = G4EmParameters::Instance();

      param->SetDefaults();
      param->SetFluo(true);
      param->SetAuger(true);
      param->SetAugerCascade(true);
      param->SetDeexcitationIgnoreCut(true);

      G4ProductionCutsTable::GetProductionCutsTable()->
          SetEnergyRange(100*eV, 1*GeV);
      param->SetMinEnergy(100*eV);
      param->SetMaxEnergy(1*GeV);

      decay_List = new G4DecayPhysics();
      decay_List -> ConstructParticle();

      ConstructEM();
    }

    // step limitation (as a full process)
    //
    AddStepMax();
}

void PhysicsList::AddPhysicsList(const G4String &name)
{
    if (verboseLevel>1) {
        std::cout << "PhysicsList::AddPhysicsList: <" << name << ">\n";
    }

    if (name == name_) return;

    if (name == "emstandard") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics>(verboseLevel);

    } else if (name == "emstandard_opt1") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics_option1>(verboseLevel);

    } else if (name == "emstandard_opt2") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics_option2>(verboseLevel);

    } else if (name == "emstandard_opt3") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics_option3>(verboseLevel);

    } else if (name == "emstandard_opt4") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics_option4>(verboseLevel);

    } else if (name == "emlivermore") {
        name_ = name;
        physics_list_ = std::make_unique<G4EmLivermorePhysics>(verboseLevel);

    } else if (name == "empenelope") {
        name_ = name;
        physics_list_ = std::make_unique<G4EmPenelopePhysics>(verboseLevel);

    } else if (name == "emlowenergy") {
        name_ = name;
        physics_list_ = std::make_unique<G4EmLowEPPhysics>(verboseLevel);

    } else if (name == "emDNAphysics") {
        name_ = name;
        physics_list_ = std::make_unique<G4EmDNAPhysics>(verboseLevel);

    }
      else if (name == "emDNAphysics_opt2") {
        name_ = name;
    }
    else {
        std::cout << "PhysicsList::AddPhysicsList: <" << name << ">"
               << " is not defined\n";
    }
}

PhysicsListMessenger &PhysicsList::messenger()
{
    return *messenger_;
}

void PhysicsList::AddStepMax()
{
    auto theParticleIterator = this->GetParticleIterator();
    theParticleIterator->reset();
    while ((*theParticleIterator)()){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();

        if (step_max_process_->IsApplicable(*particle) && pmanager)
        {
            pmanager ->AddDiscreteProcess(new StepMax(*step_max_process_));
        }
    }
}

StepMax *PhysicsList::step_max_process()
{
    return step_max_process_;
}

void PhysicsList::ConstructEM()
{

  // G4-DNA option 2 process are implemented 'by hand'. The G4ElectronCapture process is included

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();

  while( (*particleIterator)() )
  {

    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    // *********************************
    // 1) Processes for the World region
    // *********************************

    if (particleName == "e-") {

      // STANDARD msc is active in the world
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      msc->AddEmModel(1,new G4UrbanMscModel());
      ph->RegisterProcess(msc, particle);

      // STANDARD ionisation is active in the world
      G4eIonisation* eion = new G4eIonisation();
      eion->SetEmModel(new G4MollerBhabhaModel(), 1);
      ph->RegisterProcess(eion, particle);

      // DNA elastic is not active in the world
      G4DNAElastic* theDNAElasticProcess = new G4DNAElastic("e-_G4DNAElastic");
      theDNAElasticProcess->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(theDNAElasticProcess);

      // DNA excitation is not active in the world
      G4DNAExcitation* dnaex = new G4DNAExcitation("e-_G4DNAExcitation");
      dnaex->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaex);

      // DNA ionisation is not active in the world
      G4DNAIonisation* dnaioni = new G4DNAIonisation("e-_G4DNAIonisation");
      dnaioni->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaioni);

      // DNA attachment is not active in the world
      G4DNAAttachment* dnaatt = new G4DNAAttachment("e-_G4DNAAttachment");
      dnaatt->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaatt);

      // DNA vib. excitation is not active in the world
      G4DNAVibExcitation* dnavib =
          new G4DNAVibExcitation("e-_G4DNAVibExcitation");
      dnavib->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnavib);

      // THE FOLLOWING PROCESS WILL KILL ALL ELECTRONS BELOW A
      // SELECTED ENERY THRESHOLD
      // Capture of low-energy e-
      G4ElectronCapture* ecap1 = new G4ElectronCapture("Spheroid", 10*MeV);
      pmanager->AddDiscreteProcess(ecap1);
      // 7.4 eV is compatible with the validity range of the elastic model

    } else if ( particleName == "proton" ) {

      // STANDARD msc is active in the world
      G4hMultipleScattering* msc = new G4hMultipleScattering();
      msc->AddEmModel(1,new G4WentzelVIModel());
      ph->RegisterProcess(msc, particle);

      // STANDARD Coulomb scattering is active in the world
      G4CoulombScattering* pcou = new G4CoulombScattering();
      ph->RegisterProcess(pcou, particle);

      // STANDARD ionisation is active in the world
      G4hIonisation* hion = new G4hIonisation();
      hion->SetEmModel(new G4BraggModel(), 1);
      hion->SetEmModel(new G4BetheBlochModel(), 2);
      ph->RegisterProcess(hion, particle);

      // DNA excitation is not active in the world
      G4DNAExcitation* dnaex = new G4DNAExcitation("proton_G4DNAExcitation");
      dnaex->SetEmModel(new G4DummyModel(),1);
      dnaex->SetEmModel(new G4DummyModel(),2);
      pmanager->AddDiscreteProcess(dnaex);

      // DNA ionisation is not active in the world
      G4DNAIonisation* dnaioni = new G4DNAIonisation("proton_G4DNAIonisation");
      dnaioni->SetEmModel(new G4DummyModel(),1);
      dnaioni->SetEmModel(new G4DummyModel(),2);
      pmanager->AddDiscreteProcess(dnaioni);

      // DNA elastic is not active in the world
      G4DNAElastic* dnael = new G4DNAElastic("proton_G4DNAElastic");
      dnael->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnael);

      // DNA charge decrease is not active in the world
      G4DNAChargeDecrease* dnacd = new G4DNAChargeDecrease
        ("proton_G4DNAChargeDecrease");
      dnacd->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnacd);

    } else if ( particleName == "hydrogen" ) {

      // DNA excitation is not active in the world
      G4DNAExcitation* dnaex = new G4DNAExcitation("hydrogen_G4DNAExcitation");
      dnaex->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaex);

      // DNA ionisation is not active in the world
      G4DNAIonisation* dnaioni = new G4DNAIonisation
        ("hydrogen_G4DNAIonisation");
      dnaioni->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaioni);

      // DNA elastic is not active in the world
      G4DNAElastic* dnael = new G4DNAElastic("hydrogen_G4DNAElastic");
      dnael->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnael);

      // DNA charge increase is not active in the world
      G4DNAChargeIncrease* dnaci = new G4DNAChargeIncrease
        ("hydrogen_G4DNAChargeIncrease");
      dnaci->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaci);

    } else if (particleName == "GenericIon") {

      // WARNING : THIS IS NEEDED FOR STANDARD ALPHA G4ionIonisation PROCESS

      // STANDARD msc is active in the world
      G4hMultipleScattering* msc = new G4hMultipleScattering();
      msc->AddEmModel(1, new G4UrbanMscModel());
      ph->RegisterProcess(msc, particle);

      // STANDARD ionisation is active in the world
      G4ionIonisation* hion = new G4ionIonisation();
      hion->SetEmModel(new G4BraggIonModel(), 1);
      hion->SetEmModel(new G4BetheBlochModel(), 2);
      ph->RegisterProcess(hion, particle);

    } else if ( particleName == "alpha" ) {

      // STANDARD msc is active in the world
      G4hMultipleScattering* msc = new G4hMultipleScattering();
      msc->AddEmModel(1, new G4UrbanMscModel());
      ph->RegisterProcess(msc, particle);

      // STANDARD ionisation is active in the world
      G4ionIonisation* hion = new G4ionIonisation();
      hion->SetEmModel(new G4BraggIonModel(), 1);
      hion->SetEmModel(new G4BetheBlochModel(), 2);
      ph->RegisterProcess(hion, particle);

      // DNA excitation is not active in the world
      G4DNAExcitation* dnaex = new G4DNAExcitation("alpha_G4DNAExcitation");
      dnaex->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaex);

      // DNA ionisation is not active in the world
      G4DNAIonisation* dnaioni = new G4DNAIonisation("alpha_G4DNAIonisation");
      dnaioni->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaioni);

      // DNA elastic is not active in the world
      G4DNAElastic* dnael = new G4DNAElastic("alpha_G4DNAElastic");
      dnael->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnael);

      // DNA charge decrease is not active in the world
      G4DNAChargeDecrease* dnacd = new G4DNAChargeDecrease
        ("alpha_G4DNAChargeDecrease");
      dnacd->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnacd);

    }  else if ( particleName == "alpha+" ) {

      // STANDARD msc is active in the world
      G4hMultipleScattering* msc = new G4hMultipleScattering();
      msc->AddEmModel(1, new G4UrbanMscModel());
      ph->RegisterProcess(msc, particle);

      // STANDARD ionisation is active in the world
      G4ionIonisation* hion = new G4ionIonisation();
      hion->SetEmModel(new G4BraggIonModel(),1);
      hion->SetEmModel(new G4BetheBlochModel(), 2);
      ph->RegisterProcess(hion, particle);

      // DNA excitation is not active in the world
      G4DNAExcitation* dnaex = new G4DNAExcitation("alpha+_G4DNAExcitation");
      dnaex->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaex);

      // DNA ionisation is not active in the world
      G4DNAIonisation* dnaioni = new G4DNAIonisation("alpha+_G4DNAIonisation");
      dnaioni->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaioni);

      // DNA elastic is not active in the world
      G4DNAElastic* dnael = new G4DNAElastic("alpha+_G4DNAElastic");
      dnael->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnael);

      // DNA charge decrease is not active in the world
      G4DNAChargeDecrease* dnacd = new G4DNAChargeDecrease
        ("alpha+_G4DNAChargeDecrease");
      dnacd->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnacd);

      // DNA charge increase is not active in the world
      G4DNAChargeIncrease* dnaci = new G4DNAChargeIncrease
        ("alpha+_G4DNAChargeIncrease");
      dnaci->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaci);

    } else if ( particleName == "helium" ) {

      // DNA excitation is not active in the world
      G4DNAExcitation* dnaex = new G4DNAExcitation("helium_G4DNAExcitation");
      dnaex->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaex);

      // DNA ionisation is not active in the world
      G4DNAIonisation* dnaioni = new G4DNAIonisation("helium_G4DNAIonisation");
      dnaioni->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaioni);

      // DNA elastic is not active in the world
      G4DNAElastic* dnael = new G4DNAElastic("helium_G4DNAElastic");
      dnael->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnael);

      // DNA charge increase is not active in the world
      G4DNAChargeIncrease* dnaci = new G4DNAChargeIncrease
        ("helium_G4DNAChargeIncrease");
      dnaci->SetEmModel(new G4DummyModel(),1);
      pmanager->AddDiscreteProcess(dnaci);

    } else if (particleName == "gamma") {

      // photoelectric effect - Livermore model only
      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      thePhotoElectricEffect->SetEmModel(new G4LivermorePhotoElectricModel());
      ph->RegisterProcess(thePhotoElectricEffect, particle);

      // Compton scattering - Livermore model only
      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      theComptonScattering->SetEmModel(new G4LivermoreComptonModel());
      ph->RegisterProcess(theComptonScattering, particle);

      // gamma conversion - Livermore model below 80 GeV
      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      theGammaConversion->SetEmModel(new G4LivermoreGammaConversionModel());
      ph->RegisterProcess(theGammaConversion, particle);

      // default Rayleigh scattering is Livermore
      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      ph->RegisterProcess(theRayleigh, particle);
    }
  }



  // **************************************
  // 2) Define processes for Target region
  // **************************************

  // STANDARD EM processes should be inactivated when
  // corresponding DNA processes are used
  // - STANDARD EM e- processes are inactivated below 1 MeV
  // - STANDARD EM proton & alpha processes are inactivated below
  //   standEnergyLimit
  G4double standEnergyLimit = 9.9*MeV;
  //

  G4double massFactor = 1.0079/4.0026;
  G4EmConfigurator* em_config =
      G4LossTableManager::Instance()->EmConfigurator();

  G4VEmModel* mod;

  // *** e-

  // ---> STANDARD EM processes are inactivated below 1 MeV

  mod =  new G4UrbanMscModel();
  mod->SetActivationLowEnergyLimit(1.*MeV);
  em_config->SetExtraEmModel("e-","msc",mod,"Spheroid");

  mod = new G4MollerBhabhaModel();
  mod->SetActivationLowEnergyLimit(1.*MeV);
  em_config->SetExtraEmModel("e-","eIoni", mod, "Spheroid", 0.0, 100*TeV, new G4UniversalFluctuation());

  // ---> DNA processes activated

  mod = new G4DNAChampionElasticModel();
  em_config->SetExtraEmModel("e-","e-_G4DNAElastic", mod, "Spheroid", 7.4*eV, 1.*MeV);

  mod = new G4DNABornIonisationModel();
  em_config->SetExtraEmModel("e-","e-_G4DNAIonisation", mod,"Spheroid",11.*eV,1.*MeV);
  // Note: valid from 11 eV to 0.999.. MeV then switch to std models at
  // higher energies ; same for other models

  mod = new G4DNABornExcitationModel();
  em_config->SetExtraEmModel("e-","e-_G4DNAExcitation", mod,"Spheroid",9.*eV,1.*MeV);

  mod = new G4DNAMeltonAttachmentModel();
  em_config->SetExtraEmModel("e-","e-_G4DNAAttachment", mod,"Spheroid",4.*eV,13.*eV);

  mod = new G4DNASancheExcitationModel();
  em_config->SetExtraEmModel("e-","e-_G4DNAVibExcitation", mod,"Spheroid",2.*eV,100.*eV);

  // *** proton

  // ---> STANDARD EM processes inactivated below standEnergyLimit
  //      or below 1 MeV for scattering

  // Inactivate following STANDARD processes

  mod =  new G4WentzelVIModel();
  mod->SetActivationLowEnergyLimit(1.*MeV);
  em_config->SetExtraEmModel("proton","msc",mod,"Spheroid", 0, 100*TeV);

  mod =  new G4eCoulombScatteringModel();
  mod->SetActivationLowEnergyLimit(1.*MeV);
  em_config->SetExtraEmModel("proton","CoulombScat",mod,"Spheroid", 0, 100*TeV);

  mod = new G4BraggModel();
  mod->SetActivationLowEnergyLimit(standEnergyLimit);
  em_config->SetExtraEmModel("proton","hIoni", mod,"Spheroid",0.0,2*MeV, new G4IonFluctuations());

  mod = new G4BetheBlochModel();
  mod->SetActivationLowEnergyLimit(standEnergyLimit);
  em_config->SetExtraEmModel("proton","hIoni", mod,"Spheroid",2*MeV,100*TeV, new G4UniversalFluctuation());

  // ---> DNA processes activated

  mod = new G4DNAIonElasticModel();
  em_config->SetExtraEmModel("proton","proton_G4DNAElastic", mod,"Spheroid",100*eV,1.*MeV);

  mod = new G4DNARuddIonisationModel();
  em_config->SetExtraEmModel("proton","proton_G4DNAIonisation", mod,"Spheroid",100*eV,0.5*MeV);

  mod = new G4DNABornIonisationModel();
  em_config->SetExtraEmModel("proton","proton_G4DNAIonisation", mod,"Spheroid",0.5*MeV,10*MeV);

  mod = new G4DNAMillerGreenExcitationModel();
  em_config->SetExtraEmModel("proton","proton_G4DNAExcitation", mod,"Spheroid",10*eV,0.5*MeV);

  mod = new G4DNABornExcitationModel();
  em_config->SetExtraEmModel("proton","proton_G4DNAExcitation", mod,"Spheroid",0.5*MeV,10*MeV);

  mod = new G4DNADingfelderChargeDecreaseModel();
  em_config->SetExtraEmModel("proton","proton_G4DNAChargeDecrease", mod,"Spheroid",100*eV,10*MeV);

  // *** hydrogen

  // ---> NO STANDARD EM processes

  // ---> DNA processes activated

  mod = new G4DNAIonElasticModel();
  em_config->SetExtraEmModel("hydrogen","hydrogen_G4DNAElastic", mod,"Spheroid",100.*eV,1.*MeV);

  mod = new G4DNARuddIonisationModel();
  em_config->SetExtraEmModel("hydrogen","hydrogen_G4DNAIonisation", mod,"Spheroid",100*eV,10*MeV);

  mod = new G4DNAMillerGreenExcitationModel();
  em_config->SetExtraEmModel("hydrogen","hydrogen_G4DNAExcitation", mod,"Spheroid",10*eV,0.5*MeV);

  mod = new G4DNADingfelderChargeIncreaseModel();
  em_config->SetExtraEmModel("hydrogen","hydrogen_G4DNAChargeIncrease", mod,"Spheroid",100*eV,10*MeV);

  // *** alpha

  // ---> STANDARD EM processes inactivated below standEnergyLimit
  //      or below 1 MeV for scattering

  // Inactivate following STANDARD processes

  mod =  new G4UrbanMscModel();
  mod->SetActivationLowEnergyLimit(1.*MeV);
  em_config->SetExtraEmModel("alpha","msc",mod,"Spheroid", 0, 100*TeV);

  mod = new G4BraggIonModel();
  mod->SetActivationLowEnergyLimit(standEnergyLimit);
  em_config->SetExtraEmModel("alpha","ionIoni", mod,"Spheroid",0.0,2*MeV/massFactor, new G4IonFluctuations());

  mod = new G4BetheBlochModel();
  mod->SetActivationLowEnergyLimit(standEnergyLimit);
  em_config->SetExtraEmModel("alpha","ionIoni", mod,"Spheroid",2*MeV/massFactor,100*TeV, new G4UniversalFluctuation());

  // ---> DNA processes activated

  mod = new G4DNAIonElasticModel();
  em_config->SetExtraEmModel("alpha","alpha_G4DNAElastic", mod,"Spheroid",100*eV,1.*MeV);

  mod = new G4DNARuddIonisationModel();
  em_config->SetExtraEmModel("alpha","alpha_G4DNAIonisation", mod,"Spheroid",1*keV,100*MeV);

  mod = new G4DNAMillerGreenExcitationModel();
  em_config->SetExtraEmModel("alpha","alpha_G4DNAExcitation", mod,"Spheroid",1*keV,100*MeV);

  mod = new G4DNADingfelderChargeDecreaseModel();
  em_config->SetExtraEmModel("alpha","alpha_G4DNAChargeDecrease", mod,"Spheroid",1*keV,100*MeV);


  // *** alpha+

  // ---> STANDARD EM processes inactivated below standEnergyLimit
  //      or below 1 MeV for scattering

  // Inactivate following STANDARD processes

  mod =  new G4UrbanMscModel();
  mod->SetActivationLowEnergyLimit(1.*MeV);
  em_config->SetExtraEmModel("alpha+","msc",mod,"Spheroid", 0, 100*TeV);

  mod = new G4BraggIonModel();
  mod->SetActivationLowEnergyLimit(standEnergyLimit);
  em_config->SetExtraEmModel("alpha+","ionIoni", mod,"Spheroid",0.0,2*MeV/massFactor, new G4IonFluctuations());

  mod = new G4BetheBlochModel();
  mod->SetActivationLowEnergyLimit(standEnergyLimit);
  em_config->SetExtraEmModel("alpha+","ionIoni", mod,"Spheroid",2*MeV/massFactor,100*TeV, new G4UniversalFluctuation());

  // ---> DNA processes activated

  mod = new G4DNAIonElasticModel();
  em_config->SetExtraEmModel("alpha+","alpha+_G4DNAElastic", mod,"Spheroid",100*eV,1.*MeV);

  mod = new G4DNARuddIonisationModel();
  em_config->SetExtraEmModel("alpha+","alpha+_G4DNAIonisation", mod,"Spheroid",1*keV,10*MeV);

  mod = new G4DNAMillerGreenExcitationModel();
  em_config->SetExtraEmModel("alpha+","alpha+_G4DNAExcitation", mod,"Spheroid",1*keV,10*MeV);

  mod = new G4DNADingfelderChargeIncreaseModel();
  em_config->SetExtraEmModel("alpha+","alpha+_G4DNAChargeIncrease", mod,"Spheroid",1*keV,10*MeV);

  mod = new G4DNADingfelderChargeDecreaseModel();
  em_config->SetExtraEmModel("alpha+","alpha+_G4DNAChargeDecrease", mod,"Spheroid",1*keV,10*MeV);

  // *** helium

  // ---> NO STANDARD EM processes

  // ---> DNA processes activated

  mod = new G4DNAIonElasticModel();
  em_config->SetExtraEmModel("helium","helium_G4DNAElastic", mod,"Spheroid",100*eV,1.*MeV);

  mod = new G4DNARuddIonisationModel();
  em_config->SetExtraEmModel("helium","helium_G4DNAIonisation", mod,"Spheroid",1*keV,10*MeV);

  mod = new G4DNAMillerGreenExcitationModel();
  em_config->SetExtraEmModel("helium","helium_G4DNAExcitation", mod,"Spheroid",1*keV,10*MeV);

  mod = new G4DNADingfelderChargeIncreaseModel();
  em_config->SetExtraEmModel("helium","helium_G4DNAChargeIncrease", mod,"Spheroid",1*keV,10*MeV);

   //Deexcitation

  G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);

}


}
