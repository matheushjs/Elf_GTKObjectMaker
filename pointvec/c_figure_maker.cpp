/*
 * This is a C++ file supposed to provide C-compatible interface to use the FigureMaker class.
 */

#include <figure_maker.h>
#include <c_figure_maker.h> //Necessary for declaring the C linkage.

CFigureMaker *c_figure_maker_new(int addr){
	return (CFigureMaker *) new FigureMaker(addr);
}

void c_figure_maker_destroy(CFigureMaker *fig){
	delete (FigureMaker *) fig;
}

void c_figure_maker_add_point(CFigureMaker *fig, int x, int y, const char vga_char[5], const char *comment){
	Point pt(x, y);
	if(comment) pt.append_to_comment(comment);
	pt.set_vga_char(vga_char);
	((FigureMaker*) fig)->add(pt);
}

void c_figure_maker_add_rectangle(CFigureMaker *fig, int x, int y, const char vga_char[5],
		const char *comment, int width, int height){
	Point pt(x, y);
	if(comment) pt.append_to_comment(comment);
	pt.set_vga_char(vga_char);
	((FigureMaker*) fig)->add_rectangle(pt, width, height);
}

void c_figure_maker_add_down_triangle(CFigureMaker *fig, int x, int y, const char vga_char[5],
		const char *comment, int width, int height){
	Point pt(x, y);
	if(comment) pt.append_to_comment(comment);
	pt.set_vga_char(vga_char);
	((FigureMaker*) fig)->add_down_triangle(pt, width, height);
}

void c_figure_maker_add_up_triangle(CFigureMaker *fig, int x, int y, const char vga_char[5],
		const char *comment, int width, int height){
	Point pt(x, y);
	if(comment) pt.append_to_comment(comment);
	pt.set_vga_char(vga_char);
	((FigureMaker*) fig)->add_up_triangle(pt, width, height);
}

void c_figure_maker_merge(CFigureMaker *fig){
	((FigureMaker *) fig)->merge();
}

void c_figure_maker_undo(CFigureMaker *fig){
	((FigureMaker *) fig)->undo();
}

VgaCharMatrix *c_figure_maker_get_matrix(const CFigureMaker *fig){
	return &((FigureMaker *) fig)->get_vga_char_matrix();
}

void c_figure_maker_destroy_matrix(VgaCharMatrix *m){
	FigureMaker().destroy_vga_char_matrix(*m);
}

void c_figure_maker_save_to_file(const CFigureMaker *fig, FILE *fp){
	((FigureMaker *) fig)->save_to_file(fp);
}

void c_figure_maker_display(const CFigureMaker *fig){
	((FigureMaker *) fig)->display();
}
