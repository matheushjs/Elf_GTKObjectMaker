/* FigureMaker class
 *
 * This class derives from PointVec, and implements functions to create figures within
 * 	the PointVec, which can only add points.
 */

#ifndef _FIGURE_MAKER_H
#define _FIGURE_MAKER_H

#include <point.h>
#include <pointvec.h>

class FigureMaker : public PointVec {
public:
	FigureMaker() : PointVec() {};
	FigureMaker(int caddr) : PointVec(caddr) {};

	//Adds a rectangle to the temp vector of points.
	//Starts at 'p' coordinates and expands to have 'width' and 'height' dimensions.
	void add_rectangle(Point p, int width, int height);

	//Adds a down (base pointing downside)/up(pointing upside) triangle,
	//starts on Point p's coordinates,
	//extends for dimensions 'width' and 'height'.
	void add_down_triangle(Point p, int width, int height);
	void add_up_triangle(Point p, int width, int height);

	enum fill_t { NONE, UP, DOWN, RIGHT, LEFT };

	//Adds the shape of a function.
	//Point p represents the (0,0).
	//Function is applied at interval [xi,xe], but translated to x=0.
	//mx multiplicates the domain f(mx * x).
	//mf multiplicates the image mf*f(x).
	//The filling is decided by fill.
	void add_function(Point p, double(*func)(double),
			int xi, int xe, double mx, double mf,
			enum fill_t fill);
};

#include <figure_maker.hxx>

#endif
