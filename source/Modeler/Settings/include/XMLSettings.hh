/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef XML_SETTINGS_HH
#define XML_SETTINGS_HH

#include <QString>

namespace XML
{
	namespace CPOP_Flag
	{
		// node flag
		static const QString save_flag 							= "CPOP_SAVE";
		static const QString environment_flag 					= "ENVIRONMENT";
		static const QString simulated_sub_env_flag 			= "SIMULATED_SUB_ENVIRONMENT";
		static const QString mirrored_sub_env_flag 				= "MIRORRED_SUB_ENVIRONMENT";
		static const QString cells_flag 						= "CELLS";
		static const QString cell_flag 							= "CELL";
		static const QString cell_population_properties_flag 	= "CELL_POPULATION_PROPERTIES";
		static const QString all_cell_properties_flag 			= "ALL_CELL_PROPERTIES";
		static const QString cell_property_flag 				= "CELL_PROPERTIES";
		static const QString active_life_cycle_flag				= "ACTIVE_LIFE_CYCLE";
	
		// variable flag 
		static const QString cell_shape_type_flag 				= "cell_type";
		static const QString contained_agent_flag 				= "contained_agent";
		static const QString dimension_flag 					= "dimension";
		static const QString name_flag 							= "name";
		static const QString nuclei_flag 						= "Nuclei";
		static const QString nucleus_flag 						= "Nucleus";
		static const QString nucleus_shape_type_flag 			= "nucleus_type";
		static const QString nucleus_pos_type_flag 				= "position_type";
		static const QString radius_flag 						= "radius";
		static const QString position_flag 						= "position";
		static const QString agent_ID_flag 						= "ID";
		static const QString mirrored_env_link_ID 				= "ref_env";
		static const QString mass_flag							= "mass";
		static const QString nucleus_radius_flag				= "nucleus_radius";
		static const QString membrane_radius_flag				= "membrane_radius";
		static const QString life_cycle_flag 					= "life_cycle";
		static const QString min_flag 							= "min";
		static const QString max_flag 							= "max";
		static const QString cell_properties_ID_flag			= "cell_properties_ID";
		static const QString variable_attribute_flag 			= "var_attribute";
		static const QString material_flag 						= "material";
		static const QString cytoplasms_mat_flag				= "cytoplasm_material";
		static const QString nuclei_mat_flag					= "nuclei_material";

		// spatial delimitation flag
		static const QString discsSDelimitation_flag 			= "DiscsSDelimitation";
		static const QString spheresSDelimitation_flag 			= "SpheresSDelimitation";
		static const QString boxesSDelimitationflag 			= "BoxesSDelimitation";
		static const QString internalDelimitation_flag			= "InternalDelimitation";
		static const QString externalDelimitation_flag			= "ExternalDelimitation";
	}
}

#endif