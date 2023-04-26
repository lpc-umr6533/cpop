#include "Population.hh"
#include <stdexcept>

#include "cReader/zupply.hpp"
#include "EnvironmentSettings.hh"
#include "MeshFactory.hh"
#include "SpheroidalCellMesh.hh"
#include "CPOP_Loader.hh"
#include "CGAL_Utils.hh"

#include "G4UnitsTable.hh"

#include "analysis.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

namespace cpop {

Population::Population()
    :messenger_(std::make_unique<PopulationMessenger>(this))
{

}

Population::~Population()
{
    delete voronoi_mesh_;
}

int Population::verbose_level() const
{
    return verbose_level_;
}

void Population::setVerbose_level(double verbose_level)
{
    verbose_level_ = verbose_level;
}

double Population::spheroid_radius() const
{
    return spheroid_radius_;
}

void Population::setSpheroid_radius(double spheroidRadius)
{
    spheroid_radius_ = spheroidRadius*conversionFrmCPOPToG4;
}

Settings::Geometry::Point_3 Population::spheroid_centroid() const
{
    return spheroid_centroid_;
}

void Population::setSpheroid_centroid(const Settings::Geometry::Point_3 &spheroidCentroid)
{
    spheroid_centroid_ = Utils::myCGAL::to_G4(spheroidCentroid);
}

unsigned int Population::number_max_facet_poly() const
{
    return number_max_facet_poly_;
}

void Population::setNumber_max_facet_poly(unsigned int number_max_facet_poly)
{
    number_max_facet_poly_ = number_max_facet_poly;
}

double Population::delta_reffinement() const
{
    return delta_reffinement_;
}

void Population::setDelta_reffinement(double delta_reffinement)
{
    delta_reffinement_ = delta_reffinement;
}

std::string Population::population_file() const
{
    return population_file_;
}

void Population::setPopulation_file(const std::string &population_file)
{
    population_file_ = population_file;
}

G4int Population::calculateNumberOfCells_InXML_File()
//VictorLevrague
{
  QString qstr = QString::fromStdString(population_file_);
  QFile file (qstr);

  QString cells_flag = "CELLS" ;

  if(file.open(QIODevice::ReadOnly))
  {

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);

  // skip information until found the cells_flag
  while(!xmlReader.atEnd() &&  !xmlReader.hasError() &&	(xmlReader.name() != "CELLS"))
  {
      xmlReader.readNext();
  }

  xmlReader.readNext();

  while(!xmlReader.atEnd() &&  !xmlReader.hasError() &&	(xmlReader.name() != "CELLS"))
  {
    if(xmlReader.name() == cell_flag)
    {
      nb_cell_xml+=1;
    }
    xmlReader.readNext();

  }

  }

  nb_cell_xml/=2;  //Both variables (positions and nuclei of CELL tag will be counted)

  return nb_cell_xml;

}

void Population::loadPopulation()
{

    if (number_max_facet_poly() <= 0)
        throw std::runtime_error("nbMaxFacetPerPoly should be strictly positive. Current value : " + to_string(number_max_facet_poly()));

    if (delta_reffinement() < 0)
        throw std::runtime_error("deltaReffinement should be positive. Current value : " + to_string(delta_reffinement()));

    if (population_file().empty())
        throw std::runtime_error("Population file not set. Use /cpop/detector/setPopulation file.xml to set it.");
    else {
        zz::fs::Path path = "./" + population_file();

        if(!path.is_file()) {
            std::string errorMsg = "Could not find population file at " + path.abs_path();
            throw std::runtime_error(errorMsg.c_str());
        }
    }

    Settings::nEnvironment::t_Environment_3* env;
    {
        CPOP_Loader loader;
        // G4cout << "\n\n\n load3DEnvironment \n\n\n" << G4endl;
        env = loader.load3DEnvironment(population_file().c_str(), true);

    }

    G4cout << "\n\n nb_cell_xml \n\n" <<calculateNumberOfCells_InXML_File() << G4endl;

    if(!env)
    {
        throw std::runtime_error("No environment found on the given file. Is the file name/path correct ?");
    }

    assert(env);
    assert(  dynamic_cast<Settings::nEnvironment::t_SimulatedSubEnv_3*>( env->getFirstChild() ));

    assert(env->getFirstChild());
    set<Agent*> lAgts = env->getFirstChild()->getAgents();
    // warning : this should be done before the meshing because only cell positions are modified, not their meshes
    set<t_SpatialableAgent_3*> spaAgts;
    // G4int int_test = 0 ;
    for(set<Agent*>::iterator itAgt = lAgts.begin(); itAgt != lAgts.end(); ++itAgt)
    {
        t_SpatialableAgent_3* lSpaAgt = dynamic_cast<t_SpatialableAgent_3*>(*itAgt);
        if(lSpaAgt)
        {
            // int_test += 1;
            spaAgts.insert(lSpaAgt);
            // G4cout << "\n lSpaAgt.getID()" << lSpaAgt->getID() << G4endl;
        }
    }
    //G4cout << "\n int_test" << int_test << G4endl;
    Utils::setBarycenter(spaAgts.begin(), spaAgts.end(), spheroid_centroid());

    // define a mesh
    int error;                        //< error value when create_3DMesh is executed
    voronoi_mesh_ = MeshFactory::getInstance()->create_3DMesh(&error,  dynamic_cast<t_SimulatedSubEnv_3*>( env->getFirstChild() ), MeshTypes::Round_Cell_Tesselation, number_max_facet_poly(), delta_reffinement());
    dynamic_cast<SpheroidalCellMesh*>(voronoi_mesh_)->generateMesh();

    std::ofstream masses_cell_file;
    masses_cell_file.open("MassesCell.txt");

    //Call of this function allow to write geometry informations in .gdml (HACKED FOR NOW)
    //A sub call of SpheroidalCell:convertToG4Structure writes the masses of cells in MassesCell.txt
    dynamic_cast<SpheroidalCellMesh*>(voronoi_mesh_)->exportToFile("",MeshOutFormats::GDML , false);

    masses_cell_file.close();
    std::cout << '\n' << " Masses of cells written in MassesCell.txt "  <<'\n';



    // create the vector of cells
    {
        set<t_Cell_3*> lCells = voronoi_mesh_->getCellsWithShape();
        cells_.clear();
        cells_.insert(cells_.begin(), lCells.begin(), lCells.end());
    }


    // compute spheroid radius from the farthest cell
    double nearest, farthest;
    Utils::getNearestAndFarthestPoints(spheroid_centroid_, cells_.begin(), cells_.end(), nearest, farthest );
    setSpheroid_radius(farthest);

    if(verbose_level() > 0){
        printPopulationInfo();
    }

    if (writePositionsDirectionsTxt)
    {  std::ofstream positions_directions_file;
       positions_directions_file.open("positionsDirections.txt");}
}

void Population::printPopulationInfo()
{
    zz::fs::Path path = "./" + population_file();

    std::cout << "\t Defining geometry" << std::endl;
    std::cout << "Found "<< population_file() << " at " << path.abs_path() << std::endl;
    cout << "Spheroid centroid = " << spheroid_centroid() << " radius of the spheroid = " << G4BestUnit(spheroid_radius(),"Length") << endl;
}

void Population::defineRegion()
{

    if (internal_layer_ratio_ < 0 || internal_layer_ratio_ > 1)
        throw std::runtime_error("InternalLayerRatio should be in [0;1]. Current value : " + to_string(internal_layer_ratio_));

    if (intermediary_layer_ratio_ < 0 || intermediary_layer_ratio_ > 1)
        throw std::runtime_error("IntermediaryLayerRatio should be in [0;1]. Current value : " + to_string(intermediary_layer_ratio_));

    double intermediaryLayerRadius = spheroid_radius_*intermediary_layer_ratio_;
    double internalLayerRadius = spheroid_radius_*internal_layer_ratio_;

    // region creation
    regions_.reserve(3);
    regions_.emplace_back("Necrosis"    , spheroid_centroid_, 0, internalLayerRadius, cells_.begin(), cells_.end());
    regions_.emplace_back("Intermediary", spheroid_centroid_, internalLayerRadius, intermediaryLayerRadius, cells_.begin(), cells_.end());
    regions_.emplace_back("External"    , spheroid_centroid_, intermediaryLayerRadius, spheroid_radius_, cells_.begin(), cells_.end());

    for(SpheroidRegion& region : regions_) {
        auto samples = region.sample(number_sampling_cell_per_region_);
        sampled_cells_.insert(sampled_cells_.end(), samples.begin(), samples.end());
    }

    if(verbose_level() > 0)
        printRegionInfo();

}

void Population::printRegionInfo()
{
    double intermediaryLayerRadius = spheroid_radius_*intermediary_layer_ratio_;
    double internalLayerRadius = spheroid_radius_*internal_layer_ratio_;

    std::cout << "\tCreating Regions" << std::endl;
    std::cout << "IntermediaryLayerRadius " << G4BestUnit(intermediaryLayerRadius,"Length") << std::endl;
    std::cout << "InternalLayerRadius " << G4BestUnit(internalLayerRadius,"Length") << std::endl;
    std::cout << "Spheroid radius " << G4BestUnit(spheroid_radius_,"Length") << std::endl;
    for(SpheroidRegion& region : regions_) {
        auto samples = region.sample();
        std::cout << "Region " << region.name() << " observes " << samples.size() << " cells" << std::endl;
        if(verbose_level_ > 0) {
            for(auto sample : samples)
                std::cout << "  Cell id : " << sample->getID() <<'\n';
        }
    }
}

const std::vector<const Settings::nCell::t_Cell_3 *>& Population::sampled_cells() const
{
    return sampled_cells_;
}

PopulationMessenger &Population::messenger()
{
    return (*messenger_);
}

double Population::internal_layer_ratio() const
{
    return internal_layer_ratio_;
}

double Population::intermediary_layer_ratio() const
{
    return intermediary_layer_ratio_;
}

double Population::number_sampling_cell_per_region() const
{
    return number_sampling_cell_per_region_;
}

std::vector<SpheroidRegion> Population::regions() const
{
    return regions_;
}

void Population::setNumber_sampling_cell_per_region(double value)
{
    number_sampling_cell_per_region_ = value;
}

void Population::setIntermediary_layer_ratio(double intermediary_layer_ratio)
{
    intermediary_layer_ratio_ = intermediary_layer_ratio;
}

void Population::setInternal_layer_ratio(double internal_layer_ratio)
{
    internal_layer_ratio_ = internal_layer_ratio;
}

const std::vector<const Settings::nCell::t_Cell_3 *> &Population::cells() const
{
    return cells_;
}

void Population::set_Stepping_level_info_bool(int stepping_level_info_arg)
{
  stepping_level_info_ = stepping_level_info_arg;
}

void Population::set_Event_level_info_bool(int event_level_info_arg)
{
  event_level_info_ = event_level_info_arg;
}

void Population::EnableWritingPositionsDirections(G4String choice)
{
  if (choice.compare("yes")==0)
  {writePositionsDirectionsTxt = true;}
  else if (choice.compare("no")==0)
  {writePositionsDirectionsTxt = false;}
  else
  { std::stringstream error_msg;
    error_msg << "Chose yes or no for the choice of writing positions"
                      "and directions in a txt file";
   throw std::runtime_error(error_msg.str());}
}

}
