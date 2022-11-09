/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ELEMENT_MANAGER_HH
#define ELEMENT_MANAGER_HH

#include "G4Element.hh"

#include <map>
#include <QString>

///////////////////////////////////////////////////////////////////////////
/// \brief define the element manager
/// \warning elements are defined from the G4Element definition
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////
class ElementManager
{
public:
	/// \brief constructor
	ElementManager();
	/// \brief destructor
	~ElementManager();
	/// \brief return the ElementManager singleton
	static ElementManager* getInstance();
	/// \brief return the requested element
	G4Element* getElement(QString);
	/// \brief register the G4Element*
	bool registerElement(G4Element*);
	/// \brief unregister the G4Element
	void unregisterElement(QString);

private:
	/// \brief define some basic elements
	void generateStandardElements();

	/// \brief the map registred elements
	std::map<QString, G4Element*> elementsMap;
};

#endif // ELEMENT_MANAGER_HH