/* point.h
 * 
 *
 * == REMINDER about object map and charmap memory files.
 *
 * 	Object map .mif files follow the pattern
 * 		#: 0x####
 * 		#: 0x####
 * 	Where the first entry is data formatted for vga_char,
 * 		and the second entry is data formatted for vga_pos.
 * 
 * 	The first hexadecimal in vga_char represents the size of the object to be printed.
 * 	The second represents the color of the object.
 * 	The third represents the number of the object in the charmap.mif file.
 *
 * 	The vga_pos entry represents the position [0..1199] where the object should be printed.
 *
 * == class Point
 *
 * 	The Point class is supposed to represent a pair of entries vga_char and vga_pos.
 * 	For holding the vga char, a char[5] is used to hold the hexadecimal characters.
 * 	For the vga_pos, 2 integers (x and y) are used. When vga_pos is read from a file, 
 * 		it is converted to cartesian coordinates x and y. When printing,
 * 		it is converted back to [0..1199] format.
 * 	We also need to hold one of the entry's address. The address of the first entry (vga_char)
 * 		has been chosen as the one to be stored in the variable 'addr'.
 *
 */


#ifndef _POINT_H
#define _POINT_H

#include <string>
#include <stdio.h>

//Converts cartesian coordinates (x,y) to linear [0..1199] coordinate.
#define CARTESIAN_TO_LINEAR(X, Y) (29-Y)*40+X

//Converts linear [0..1199] coordinates to (x, y) cartesian coordinates.
#define LINEAR_TO_CARTESIAN_X(L) L%40
#define LINEAR_TO_CARTESIAN_Y(L) 29-(L/40)

//Convenience macro for functions that receive (x,y) arguments, in this order.
#define LINEAR_TO_CARTESIAN_XY(L) L%40,29-(L/40)

class Point {
	char vga_char[5]; //Array allows for automatic operator= function.
	int x, y;
	int addr;
	std::string comment; //Optional comment after the vga_char entry.

	public:
		//Initializers
		Point() : vga_char() {};
		Point(const int cx, const int cy) : vga_char(), x(cx), y(cy) {};
		Point(const int cx, const int cy, const int caddr) : vga_char(), x(cx), y(cy), addr(caddr) {};

		//Accessing
		int get_x(){ return x; };
		int get_y(){ return y; };
		int get_address(){ return addr; };
		const char *get_vga_char(){ return (const char *) vga_char; };

		//Modifying
		void set_x(const int cx){ x = cx; };
		void set_y(const int cy){ y = cy; };
		void set_xy(const int cx, const int cy){ x = cx; y = cy; };
		void set_addr(const int caddr){ addr = caddr; };
		void set_vga_char(const char *str);
		void offset(const int dx, const int dy){ x += dx; y += dy; };

		//Prints Point into FILE* following the object map .mif file format.
		void print_to_file(FILE *);

		//Checks if Point is out of bounds (outside the 40x30 grid).
		bool OOB();

		//Adds a comment to be printed together with the vga_char entry.
		void append_to_comment(const char *app);
		void append_to_comment(const std::string app);
};

#include <point.hxx>

#endif
