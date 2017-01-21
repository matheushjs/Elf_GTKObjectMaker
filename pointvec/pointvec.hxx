#ifndef _POINTVEC_HXX
#define _POINTVEC_HXX

#ifndef _POINTVEC_H
#error
#endif

#include <iostream>
#include <cstdlib>
#include <cstring>
using std::cout;
using std::cerr;

void PointVec::add(Point p){
	if(!p.OOB()){
		p.set_addr(addr);
		addr += 2;
		temp.push_back(p);
	} else cerr << "PointVec::add(): Did not add OOB Point.\n";
}

void PointVec::merge(){
	if(!temp.empty()){
		main.resize(main.size()+1);
		main[main.size()-1].swap(temp);
		temp.clear();
	} else cerr << "Attempted to merge an empty vector."
		<< " Did not add any vector to the main list.\n";
}

void PointVec::undo(){
	merge(); //If temp is empty, it isn't merged.
	if(!main.empty()){
		addr -= main[main.size()-1].size() * 2;
		main.pop_back();
	} else cerr << "There is nothing to undo.\n";
}

void PointVec::display(){
	typedef unsigned int uint;

	for(uint i = 0; i < main.size(); i++)
		for(uint j = 0; j < main[i].size(); j++)
			cout << main[i][j].get_x() << " "
				<< main[i][j].get_y() << " "
				<< main[i][j].get_vga_char() << "\n";

	for(uint i = 0; i < temp.size(); i++)
		cout << temp[i].get_x() << " "
			<< temp[i].get_y() << " "
			<< temp[i].get_vga_char() << "\n";
}

void PointVec::save_to_file(const char *filename){
	typedef unsigned int uint;
	FILE *fp;
	
	merge();
	fp = fopen(filename, "w");
	for(uint i = 0; i < main.size(); i++){
		for(uint j = 0; j < main[i].size(); j++){
			main[i][j].print_to_file(fp);
		}
	}
	fprintf(fp, "\t%d: FFFF;\n\n", addr);
	fclose(fp);
}

void PointVec::save_to_file(FILE *fp){
	typedef unsigned int uint;
	
	merge();
	for(uint i = 0; i < main.size(); i++){
		for(uint j = 0; j < main[i].size(); j++){
			main[i][j].print_to_file(fp);
		}
	}
	fprintf(fp, "\t%d: FFFF;\n\n", addr++);
}


VgaCharMatrix &PointVec::get_vga_char_matrix(void){
	typedef unsigned int uint;
	VgaCharMatrix *mat;

	mat = (VgaCharMatrix *) std::calloc(sizeof(VgaCharMatrix), 1);

	for(uint i = 0; i < main.size(); i++)
		for(uint j = 0; j < main[i].size(); j++){
			int x, y;
			x = main[i][j].get_x();
			y = main[i][j].get_y();
			std::memcpy(mat[0][x][y], main[i][j].get_vga_char(), sizeof(char) * 4);
		}

	for(uint i = 0; i < temp.size(); i++){
		int x, y;
		x = temp[i].get_x();
		y = temp[i].get_y();
		std::memcpy(mat[0][x][y], temp[i].get_vga_char(), sizeof(char) * 4);
	}

	return *mat;
}

#endif
