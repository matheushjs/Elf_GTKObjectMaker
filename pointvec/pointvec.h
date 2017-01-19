/* PointVec
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

#include <point.h>
#include <vector>
#include <cstdio>
#include <cstdlib>

using std::vector;

typedef char VgaCharMatrix[40][30][5];

class PointVec {
	vector< vector<Point> > main;
	vector<Point> temp;
	int addr;

public:
	PointVec() : addr(0) {};
	PointVec(const int caddr) : addr(caddr) {};

	unsigned int temp_size(){ return temp.size(); };

	//Adds a Point to current working vector.
	void add(Point);

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
	void save_to_file(FILE *filename);
	
	//Creates a 40x30 (colxrow) matrix storing vga_char sequences.
	VgaCharMatrix &get_vga_char_matrix(void);
	void destroy_vga_char_matrix(VgaCharMatrix &m){ std::free(&m); };
};

#include <pointvec.hxx>

#endif
