#ifndef _FIGURE_MAKER_HXX
#define _FIGURE_MAKER_HXX

#ifndef _FIGURE_MAKER_H
#error
#endif

#include <point.h>
#include <pointvec.h>
#include <cmath>

void FigureMaker::add_rectangle(Point p, int w, int h){
	int dx, dy;
	int x, y;

	x = p.get_x();
	y = p.get_y();
	dx = (w > 0 ? 1 : -1);
	dy = (h > 0 ? 1 : -1);

	for(int i = 0; i != w; i += dx){
		for(int j = 0; j != h; j += dy){
			p.set_xy(x+i, y+j);
			this->add(p);
		}
	}
}

//Triangle with its base facing downside.
void FigureMaker::add_down_triangle(Point p, int w, int h){
	//translates point p to the bottom-left position.
	if(w < 0) p.offset(w+1, 0), w = -w;
	if(h < 0) p.offset(0, h+1), h = -h;

	int initx, inity;
	initx = p.get_x();
	inity = p.get_y();

	double dw = w/(double) h;

	//Will fill the triangle by height levels.
	//For each height, calculate the initial coordinates,
	//  then fill to the right until we reach the width.
	for(int i = 0; i < h; i++){
		int curx = std::round(i*dw);
		
		while(curx < w){
			p.set_xy(initx + curx, inity + i);
			this->add(p);
			curx++;
		}
	}
}

//Triangle with is base facing upside.
void FigureMaker::add_up_triangle(Point p, int w, int h){
	//translates point p to the bottom-left position.
	if(w < 0) p.offset(w+1, 0), w = -w;
	if(h < 0) p.offset(0, h+1), h = -h;

	int initx, inity;
	initx = p.get_x();
	inity = p.get_y();

	double dw = w/(double) h;

	//Will fill the triangle by height levels.
	//For each height, calculate the initial coordinates,
	//  then fill to the right until we reach the width.
	for(int i = 0; i < h; i++){
		int curx = std::round(i*dw);
		
		while(curx >= 0){
			p.set_xy(initx + curx, inity + i);
			this->add(p);
			curx--;
		}
	}
}

void FigureMaker::add_function(Point p, double(*func)(double), int xi, int xe, double mx, double mf, enum fill_t fill){
	bool checker[40][30] = {{}}; //Should initialize to false.

	int initx, inity, x, y;
	initx = p.get_x();
	inity = p.get_y();

	for(double i = xi; i <= xe; i+=0.05){
		double im;
		im = mf*func(mx*(double)i);

		x = initx + (i - xi);
		y = inity + std::round(im);

		if(x >= 0 && x < 40 && y >= 0 && y < 30)
			if(!checker[x][y]){
				checker[x][y] = true;
				p.set_xy(x, y);
				this->add(p);
			}

		if(fill == RIGHT){
			for(; x < 40; x++)
				if(x >= 0 && x < 40 && y >= 0 && y < 30)
					if(!checker[x][y]){
						checker[x][y] = true;
						p.set_xy(x, y);
						this->add(p);
					}
		}else if(fill == LEFT){
			for(; x >= 0; x--)
				if(x >= 0 && x < 40 && y >= 0 && y < 30)
					if(!checker[x][y]){
						checker[x][y] = true;
						p.set_xy(x, y);
						this->add(p);
					}
		}else if(fill == UP){
			for(; y < 30; y++)
				if(x >= 0 && x < 40 && y >= 0 && y < 30)
					if(!checker[x][y]){
						checker[x][y] = true;
						p.set_xy(x, y);
						this->add(p);
					}
		}else if(fill == LEFT){
			for(; y >= 0; y--)
				if(x >= 0 && x < 40 && y >= 0 && y < 30)
					if(!checker[x][y]){
						checker[x][y] = true;
						p.set_xy(x, y);
						this->add(p);
					}
		}
	}
}

#endif
