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
};

#include <figure_maker.hxx>

#endif

/*
double f1(double x){ return x; };
double nf1(double x){ return -x; };

double f2(double x){ return fabs(x); };
double nf2(double x){ return -fabs(x); };

double f3(double x){ return x*x; };
double nf3(double x){ return -(x*x); };

double f4(double x){ return x*x*x; };
double nf4(double x){ return -(x*x*x); };

double f5(double x){ return fabs(x*x*x); };
double nf5(double x){ return -fabs(x*x*x); };

double f6(double x){ return cos(x); };
double nf6(double x){ return -cos(x); };

double f7(double x){ return fabs(cos(x)); };
double nf7(double x){ return -fabs(cos(x)); };

double f8(double x){ return sin(x); };
double nf8(double x){ return -sin(x); };

double f9(double x){ return fabs(sin(x)); };
double nf9(double x){ return -fabs(sin(x)); };

double f10(double x){ return pow(2, x); };
double nf10(double x){ return -pow(2, x); };

double (*select_function(char *string))(double){
	static char fdesc[][100] = {
		"x",
		"|x|",
		"x**2",
		"x**3",
		"|x**3|",
		"cos(x)",
		"|cos(x)|",
		"sin(x)",
		"|sin(x)|",
		"2**x",
	};
	bool positive;
	int n;

	cout << "Select a number (make it negative for -f(x)):\n";
	for(unsigned int i = 0; i < sizeof(fdesc)/sizeof(fdesc[0])/sizeof(char); i++)
		cout << i+1 << ":\tf(x) = " << fdesc[i] << "\n";

	cin >> n;
	positive = n > 0 ? true : false;
	n = abs(n);

	strcpy(string, fdesc[n-1]);
	
	switch(n){
		case 1	: return positive ?	f1  	:	nf1	;
		case 2	: return positive ?	f2  	:	nf2	;
		case 3	: return positive ?	f3  	:	nf3	;
		case 4	: return positive ?	f4  	:	nf4	;
		case 5	: return positive ?	f5  	:	nf5	;
		case 6	: return positive ?	f6  	:	nf6	;
		case 7	: return positive ?	f7  	:	nf7	;
		case 8	: return positive ?	f8  	:	nf8	;
		case 9	: return positive ?	f9  	:	nf9	;
		case 10	: return positive ?	f10 	:	nf10	;
		default	: return NULL;
	}
}

enum fill_t { TOP, RIGHT, LEFT, BOTTOM, NONE};
bool make_func(pointvec_t &vec, point &p1, int &addr, enum fill_t fill){
	int x1, x2, oldy, oldx;
	double mult, stretch;
	char retstring[101], c;
	point p;
	double (*func)(double);

	c = 's';
	do{
		func = select_function(retstring);	
		
		do{
			cout << "Type a multiplicative real constant to multiply f(x): \n";
			scanf("%lf", &mult);
			if(mult == 0) cout << "Cannot be 0.\n";
		} while(mult == 0);
	
		do{
			cout << "Type a stretching constant k to have f(k*x): \n";
			scanf("%lf", &stretch);
			if(stretch == 0) cout << "Cannot be 0, try again\n";
		} while(stretch == 0);

		cout << "Type x interval (2 numbers): \n";
		scanf("%d %d%*[^\n]", &x1, &x2);
		fgetc(stdin);
		if(x1 > x2){ cout << "Invalid interval\n"; return false; }

		printf("\nStarting at cartesian point (%d,%d).\n", p1.x, p1.y);
		printf("Function will generate the following values:\n");
		for(int i = x1; i <= x2; i++)
			cout << (int) round(mult*func(stretch*i)) << " ";
		cout << "\n\n";

		cout << "Type:\n\t's' to start again.\n\tAnything else to continue.\n";
		c = fgetc(stdin);
	} while(c == 's');

	oldy = oldx = -1;
	bool changedX, changedY;
	point auxp;
	for(double i = x1; i <= x2; i += 0.01){
		changedX = changedY = false;

		p.x = p1.x + (int) (i - x1);
		p.y = p1.y + (int) round(mult*func(stretch*i));

		if(p.x != oldx) changedX = true;
		if(p.y != oldy) changedY = true;
		
		if(changedX || changedY){
			oldx = p.x;
			oldy = p.y;
		} else continue;

		if(fill == TOP){
			auxp.x = p.x;
			auxp.y = 27;
			
			if(!changedX) cout << "TOP-Filling same X!\n";
			else while(auxp.y > p.y){
				if(auxp.OOB()) break;
				auxp.addr = addr;
				addr += 2;
				
				auxp.make_string("func", retstring);
				auxp.trailstr = "";

				vec.add(auxp);
				auxp.y--;
			}
		} else if(fill == BOTTOM){
			auxp.x = p.x;
			auxp.y = 0;
			
			if(!changedX) cout << "BOTTOM-Filling same X!\n";
			else while(auxp.y < p.y){
				if(auxp.OOB()) break;
				auxp.addr = addr;
				addr += 2;

				auxp.make_string("func", retstring);
				auxp.trailstr = "";

				vec.add(auxp);
				auxp.y++;
			}
		} else if(fill == RIGHT){
			auxp.x = 39;
			auxp.y = p.y;
			
			if(!changedY) cout << "RIGHT-Filling same Y!\n";
			else while(auxp.x > p.x){
				if(auxp.OOB()) break;
				auxp.addr = addr;
				addr += 2;

				auxp.make_string("func", retstring);
				auxp.trailstr = "";

				vec.add(auxp);
				auxp.x--;
			}
		} else if(fill == LEFT){
			auxp.x = 0;
			auxp.y = p.y;

			if(!changedY) cout << "LEFT-Filling same Y!\n";
			else while(auxp.x < p.x){
				if(auxp.OOB()) break;
				auxp.addr = addr;
				addr += 2;

				auxp.make_string("func", retstring);
				auxp.trailstr = "";

				vec.add(auxp);
				auxp.x++;
			}
		}

		if(p.OOB()) continue;

		p.addr = addr;
		addr += 2;

		p.make_string("func", retstring);
		if(x1 == x2) p.trailstr = "\n";
		else p.trailstr = "";

		vec.add(p);
	}

	return true;
}

enum figure_t {
	DOT,
	FUNC,
	FUNCT,
	FUNCB,
	FUNCR,
	FUNCL,
	REC,
	TRI,
	UTRI,
	DISP,
	END,
	UNDO,
	FAILURE
};

int main(int argc, char *argv[]){
	int init;
	std::string input;
	figure_t figure;
	point p1, p2;
	pointvec_t vec;

	cout << "First address:\n";
	scanf("%d", &init);
	
	bool end_signal = false;
	do{
		cout << "rec/dot/tri/utri/func/funct/funcb/funcr/funcl/disp/undo/end\n";
		cin >> input;

		if(input.compare("dot") == 0) figure = DOT;
		else if(input.compare("func") == 0) figure = FUNC;
		else if(input.compare("funct") == 0) figure = FUNCT;
		else if(input.compare("funcb") == 0) figure = FUNCB;
		else if(input.compare("funcr") == 0) figure = FUNCR;
		else if(input.compare("funcl") == 0) figure = FUNCL;
		else if(input.compare("rec") == 0) figure = REC;
		else if(input.compare("tri") == 0) figure = TRI;
		else if(input.compare("utri") == 0) figure = UTRI;
		else if(input.compare("disp") == 0) figure = DISP;
		else if(input.compare("undo") == 0) figure = UNDO;
		else if(input.compare("end") == 0) figure = END;
		else figure = FAILURE;

		switch(figure){
			case DOT:
			case FUNC:
			case FUNCT:
			case FUNCB:
			case FUNCR:
			case FUNCL:
				vec.merge();
				cout << "1 Point (cartesian):\n";
				p1.read();
				if(p1.OOB()){ cout << "Out of bounds!\n"; continue; }
				break;
			
			case REC:
			case TRI:
			case UTRI:
				vec.merge();
				cout << "2 Points (cartesian, bottom then top)\n";
				p1.read();
				p2.read();
				if(p1.OOB() || p2.OOB()){ cout << "Out of bounds!\n"; continue; }
				break;
			default: break;
		}

		switch(figure){
			case DOT:
				p1.addr = init;
				p1.make_string("dot");
				p1.trailstr = "\n";
				init += 2;
				vec.add(p1);
				break;
			case FUNC:
				if(!make_func(vec, p1, init, NONE)) continue;
				break;
			case FUNCT:
				if(!make_func(vec, p1, init, TOP)) continue;
				break;
			case FUNCB:
				if(!make_func(vec, p1, init, BOTTOM)) continue;
				break;
			case FUNCR:
				if(!make_func(vec, p1, init, RIGHT)) continue;
				break;
			case FUNCL:
				if(!make_func(vec, p1, init, LEFT)) continue;
				break;
			case REC:
				make_rec(vec, p1, p2, init);
				break;
			case TRI:
				make_tri(vec, p1, p2, init, DOWN_BASE);
				break;
			case UTRI:
				make_tri(vec, p1, p2, init, UP_BASE);
				break;
			case DISP:
				vec.display();
				break;
			case END:
				end_signal = true;
				break;
			case UNDO:
				vec.undo(init);
				break;
			default: break;
		}

	} while(!end_signal);
	
	vec.save("output.txt", init);
	return 0;
}

*/
