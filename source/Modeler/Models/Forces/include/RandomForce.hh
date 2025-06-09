#ifndef RANDOM_FORCE_HH
#define RANDOM_FORCE_HH

#include "Force.hh"

/// \brief define a force directed randomly of a given intensity
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class RandomForce : public Force<Kernel, Point, Vector> {
public:
	RandomForce(const Cell<Kernel, Point, Vector>* pCell, Kernel pIntensity);

	/// \brief the definition of the force
	[[nodiscard]] Vector computeForce() const override;

private:
	Kernel _intensity;	///< \brief intensity of the force
};

/// \param pCell the cell for which we want to apply a force
/// \param pIntensity The intensity of the force
template<typename Kernel, typename Point, typename Vector>
RandomForce<Kernel, Point, Vector>::RandomForce(const Cell<Kernel, Point, Vector>* pCell, Kernel pIntensity):
	Force<Kernel, Point, Vector>(pCell, FRCE_INTERACT_WITH_NONE),
	_intensity(pIntensity)
{
}

/// \return The force generated
template<typename Kernel, typename Point, typename Vector>
Vector RandomForce<Kernel, Point, Vector>::computeForce() const {
	InformationSystemManager::getInstance()->Message(
		InformationSystemManager::CANT_PROCESS_MES,
		"Random force not defined for this type of template parameter",
		"RandomForce"
	);
	return {};
}

/// \return The force generated
template<>
Vector_2 RandomForce<double, Point_2, Vector_2>::computeForce() const;

/// \return The force generated
template<>
Vector_3 RandomForce<double, Point_3, Vector_3>::computeForce() const;

#endif
