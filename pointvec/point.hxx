#ifndef _POINT_HXX_
#define _POINT_HXX_

#ifndef _POINT_H
#error
#endif

//Implementation for functions of class point.

#include <stdlib.h>
#include <string.h>
#define DIE(X) { fprintf(stderr, "%s:%s:%d: %s\n", __FILE__, __func__, __LINE__, X); exit(1); }

void point::set_vga_char(const char *str){
	if(strlen(str) != 4) DIE("Passed invalid vga_char string to function");
}

bool point::OOB(){
	return (x < 0 || x >= 40 || y < 0 || y >= 30) ? true : false;
}

void point::print_to_file(FILE *fp){
	fprintf(fp, "\t%d: %4s; --%s\n", addr, (char *) vga_char, comment.c_str());
	fprintf(fp, "\t%d: %04X;\n", addr+1, CARTESIAN_TO_LINEAR(x, y));
}

void point::append_to_comment(const char *app){
	std::string str(app);
	comment.append(" " + str);
}

void point::append_to_comment(const std::string app){
	comment.append(" " + app);
}

#endif
