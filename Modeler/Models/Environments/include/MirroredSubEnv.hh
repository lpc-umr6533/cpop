/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef MIRRORED_SUB_ENVIRONEMENT_HH
#define MIRRORED_SUB_ENVIRONEMENT_HH

#include "SubEnvironment.hh"
#include "SimulatedSubEnv.hh"

/////////////////////////////////////////////////////////////////////////////////////////
/// \brief Defined a sub environement that we will mirror from a simulated environment.
/// That mean that we will duplicate agent included inside the simulatedEnvironement. But we will never 
/// execute them from this object.
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
class MirroredSubEnv : public SubEnvironment<Kernel, Point, Vector>
{
public:
	/// \brief constructor
	MirroredSubEnv(SimulatedSubEnv<Kernel, Point, Vector>* pToMirror, 
		QString pName, Vector pRelativePos, Vector pRelativeOrientation);
	/// \brief destructor
	~MirroredSubEnv();
	/// \brief  export all information needed to sumarize the Writable
	virtual	void write(QXmlStreamWriter& ) const;

private:
	SimulatedSubEnv<Kernel, Point, Vector>* toMirror;	///< \brief the reference environement
	Vector relativePos;									///< the relative position to the reference environment
	Vector relativeOrientation;							///< the relative orientation to the reference environment
};

////////////////////////// FUNCTION DEFINITIONS /////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pToMirror 			The sub environment to mirror
/// \param pName 				The name of the mirrored environment
/// \param pRelativePos 		The relative position of the mirror from the simulated environment.
/// \param pRelativeOrientation The relative orientation of the mirror from the simulated environment.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
MirroredSubEnv<Kernel, Point, Vector>::MirroredSubEnv(
	SimulatedSubEnv<Kernel, Point, Vector>* pToMirror, 
	QString pName, 
	Vector pRelativePos, 
	Vector pRelativeOrientation ):
	SubEnvironment<Kernel, Point, Vector>(pToMirror->getParent(), pName), toMirror(pToMirror), 
	relativePos(pRelativePos), 
	relativeOrientation(pRelativeOrientation)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
MirroredSubEnv<Kernel, Point, Vector>::~MirroredSubEnv()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
void MirroredSubEnv<Kernel, Point, Vector>::write(QXmlStreamWriter& writer) const
{
	writer.writeStartElement(mirrored_sub_env_flag);
	writer.writeAttribute( name_flag, Layer::getName() );
	writer.writeAttribute( mirrored_env_link_ID, toMirror->getName() );

	writer.writeEndElement(); // "mirrored_sub_env_flag"
}

#endif // MIRRORED_SUB_ENVIRONEMENT_HH