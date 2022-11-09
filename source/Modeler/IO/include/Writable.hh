/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef Writable_HH
#define Writable_HH

#include "XMLSettings.hh"

#include <QString>
#include <QXmlStreamWriter>

////////////////////////////////////////////////////////////////////////
/// \brief define function needed to export information and import it.
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////
class Writable
{
public:
	/// \brief constructor
	Writable()				{};
	/// \brief destructor
	virtual ~Writable()				{};
	/// \brief  export all information needed to sumarize the Writable
	virtual	void write(QXmlStreamWriter& writer) const = 0;
};

#endif // Writable_HH