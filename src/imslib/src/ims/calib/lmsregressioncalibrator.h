#error not working

#if 0
#ifndef IMS_LMSREGRESSIONCALIBRATOR_H
#define IMS_LMSREGRESSIONCALIBRATOR_H

#include <ims/calib/lmsregression.h>
#include <ims/calib/geometriccalibrator.h>


template <typename ListA, typename ListB=ListA>
class LMSRegressionCalibrator : public GeometricCalibrator<ListA,ListB> {
	public:
		LMSRegressionCalibrator(Logger& logger, double fraction, double epsilon);

	protected:
		virtual LinearTransformation estimateLinearTransformation();
		using GeometricCalibrator<ListA,ListB>::points;

	private:
		double fraction;
};


template <typename ListA, typename ListB>
LMSRegressionCalibrator<ListA,ListB>::LMSRegressionCalibrator(Logger& logger, double fraction, double epsilon) :
	GeometricCalibrator<ListA,ListB>(logger, epsilon),
	fraction(fraction)
{
}


template <typename ListA, typename ListB>
LinearTransformation LMSRegressionCalibrator<ListA,ListB> ::estimateLinearTransformation() {
	std::pair<double,double> line = lms_regression(points, fraction);
	return LinearTransformation(line.first, line.second);
}


#endif
#endif
