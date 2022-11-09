/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ENUM_AGENT_CONCERNED_BY_FORCE_HH
#define ENUM_AGENT_CONCERNED_BY_FORCE_HH

enum ForceInputType
{
	FRCE_INTERACT_WITH_ALL,			/// force will be applied from all the agent possible
	FRCE_INTERACT_WITH_NEIGHBOURS,		/// force will be applied from neighbours agent
	FRCE_INTERACT_WITH_UNIQUE,			/// force will ba applied from a unique agent.
	FRCE_INTERACT_WITH_NONE, 
	FRCE_INTERACT_WITH_OTHER
};

#endif // ENUM_AGENT_CONCERNED_BY_FORCE_HH