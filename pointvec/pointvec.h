/* pointvec_t
 *
 * 	Structure that will hold a vector of vector of points.
 * 	Each vector should be a "figure" made up of multiple points.
 * 	Figures can be undone as a whole. It can never be half undone.
 * 	The 'main' vector stores all of these "figures".
 * 	The 'temp' vector is the current vector being constructed by the user.
 * 	The 'temp' can be merge into 'main' by calling the merge() function.
 * 	When merge() is called, 'temp' gets emptied so that the user can start
 * 		building a new "figure".
 *
 * 	An 'addr' integer is kept to store what is the address for the next point added.
 *
 */

#ifndef _POINTVEC_H
#define _POINTVEC_H

#include "point.h"
#include <vector>
#include <cstdio>

using std::vector;

class pointvec_t {
	vector< vector<point> > main;
	vector<point> temp;
	int addr;

public:
	pointvec_t() : addr(0) {};
	pointvec_t(const int caddr) : addr(caddr) {};

	unsigned int temp_size(){ return temp.size(); };

	//Adds a point to current working vector.
	void add(point &);

	//Merges current vector into the main list of vectors.
	void merge();

	//Erases the 'temp' vector.
	//If 'temp' is empty, erases the most recent vector in 'main'.
	void undo();

	//Prints 'main' and 'temp' into the terminal screen.
	void display();

	//Saves all points in a file, following the objects map .mif file format.
	//See point.h for information about such format.
	//'temp' is merged into 'main' before saving.
	void save_to_file(const char *filename);
};

#include <pointvec.hxx>

#endif
