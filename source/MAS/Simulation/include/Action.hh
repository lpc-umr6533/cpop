#ifndef ACTION_HH
#define ACTION_HH

/// \brief class defining the action to process
/// \warning remember that our simulator is discountinuous. That mean that
/// at each step we will check simulation time and if the action's time is reach
/// or beyond then the action will be launch.
/// \details
/// On the worst case the action time can be exceed near a simulation step duration
/// @author Henri Payno
class Action {
public:
	/// \brief define the potential action frequency calues
	enum ACTION_FREQUENCY {
		PUNCTUAL_BEFORE_ITERATION,  ///< append once at a given time before iterate
		PUNCTUAL_AFTER_ITERATION,   ///< append once at a given time after iterate
		EACH_BEGIN_ITERATION,       ///< append at each iteration before launching agent
		EACH_END_ITERATION          ///< append at each iteration after launching agent
	};

public:
	Action(ACTION_FREQUENCY pFrequency, double pTime = 0.);

	/// \brief destructor
	virtual ~Action() = default;

	/// The function executed when the action his called.
	virtual bool exec() = 0;

	/// \brief return the time the action shoud append
	[[nodiscard]] double getActionTime() const { return _actionTime; }
	/// \brief return the frequency of the actions
	[[nodiscard]] ACTION_FREQUENCY getFrequency() const { return _frequency; }

private:
	ACTION_FREQUENCY _frequency;  ///< The freqeuncy of the action
	double _actionTime;           ///< The time requested for this action.

};
#endif	// ACTION_HH
