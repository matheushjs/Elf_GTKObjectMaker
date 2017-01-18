#ifndef _POINTVEC_H
#define _POINTVEC_H

#include "point.h"
#include <vector>
#include <cstdio>

using std::vector;
using std::cout;

class pointvec_t {
	vector< vector<point> > main;
	vector<point> temp;

public:
	void add(point &);
	void merge();
	void undo(int &);
	void display();
	void save(const char *, int);
};

void pointvec_t::add(point &p){
	if(!p.OOB()) temp.push_back(p);
	else cout << "Out of bounds point at pointvec_t::add. Better to undo last action.\n";
}

void pointvec_t::merge(){
	if(!temp.empty()){
		main.resize(main.size()+1);
		main[main.size()-1].swap(temp);
		temp.clear();
	}
}

void pointvec_t::undo(int &addr){
	merge();
	if(!main.empty()){
		addr -= main[main.size()-1].size() * 2;
		main.pop_back();
	}
}

void pointvec_t::display(){
	int col, row, nvecs, i;
	bool good;

	printf("   ");
	for(i = 0; i < 40; printf("%2d ", i++));
	printf("\n");

	for(row = 29; row >= 0; row--){
		printf("%2d: ", row);
		for(col = 0; col < 40; col++){
			good = false;
			
			for(nvecs = 0; nvecs < (int) main.size(); nvecs++){
				for(i = 0; i < (int) main[nvecs].size(); i++){
					if(main[nvecs][i].x == col && main[nvecs][i].y == row){
						printf("0  ");
						good = true;
						break;
					}
				}
			}
			
			if(!good)
				for(i = 0; i < (int) temp.size(); i++)
					if(temp[i].x == col && temp[i].y == row){
						printf("0  ");
						good = true;
						break;
					}
			if(!good) printf("   ");
		}
		printf("\n");
	}
}

void pointvec_t::save(const char *filename, int address){
	int i, j;
	FILE *fp;
	
	merge();

	fp = fopen(filename, "w");
	for(i = 0; i < (int) main.size(); i++){
		for(j = 0; j < (int) main[i].size(); j++){
			main[i][j].print(fp);
		}
	}
	fprintf(fp, "\t%d: FFFF;\n\n", address);
	fclose(fp);
}

#endif
