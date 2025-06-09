#ifndef ENUM_AGENT_CONCERNED_BY_FORCE_HH
#define ENUM_AGENT_CONCERNED_BY_FORCE_HH

enum ForceInputType {
	FRCE_INTERACT_WITH_ALL,         /// force will be applied from all the agent possible
	FRCE_INTERACT_WITH_NEIGHBOURS,  /// force will be applied from neighbours agent
	FRCE_INTERACT_WITH_UNIQUE,      /// force will ba applied from a unique agent.
	FRCE_INTERACT_WITH_NONE, 
	FRCE_INTERACT_WITH_OTHER
};

#endif
