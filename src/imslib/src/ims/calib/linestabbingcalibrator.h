#ifndef IMS_LINESTABBINGCALIBRATOR_H
#define IMS_LINESTABBINGCALIBRATOR_H

#include <ims/calib/linepairstabber.h>
#include <ims/calib/geometriccalibrator.h>

namespace ims {

template <typename ListA, typename ListB=ListA>
class LineStabbingCalibrator : public GeometricCalibrator<ListA,ListB> {
	public:
		LineStabbingCalibrator(double delta, double epsilon);

	protected:
		using GeometricCalibrator<ListA,ListB>::points;
		virtual LinearTransformation estimateLinearTransformation();

	private:
		double delta;
};


template <typename ListA, typename ListB>
LineStabbingCalibrator<ListA,ListB>::LineStabbingCalibrator(double delta, double epsilon) :
	GeometricCalibrator<ListA,ListB>(epsilon),
	delta(delta)
{
}


template <typename ListA, typename ListB>
LinearTransformation LineStabbingCalibrator<ListA,ListB>::estimateLinearTransformation()
{
	std::pair<double,double> line = LinePairStabber::stab_ordinate(points, delta);
	return LinearTransformation(line.first, line.second + 0.5 * delta);
}

} // namespace ims

#endif
