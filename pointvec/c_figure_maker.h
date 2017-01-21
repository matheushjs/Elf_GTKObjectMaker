/*
 * This file should be the only FigureMaker-related file included in all C files
 */

#ifndef C_FIGURE_MAKER_H
#define C_FIGURE_MAKER_H

typedef char VgaCharMatrix[40][30][5];
typedef void CFigureMaker;

#ifdef __cplusplus
extern "C"{
#endif



CFigureMaker *c_figure_maker_new(int addr);
void c_figure_maker_destroy(CFigureMaker *fig);
void c_figure_maker_add_point(CFigureMaker *fig, int x, int y, const char vga_char[5], const char *comment);

//Adds a rectangle to the temp vector of points.
//Starts at coordinates (x,y) and expands to have 'width' and 'height' dimensions.
//All points will have the given vga_char.
void c_figure_maker_add_rectangle(CFigureMaker *fig, int x, int y, const char vga_char[5],
		const char *comment, int width, int height);

//Adds a down (base pointing downside)/up(pointing upside) triangle,
//starts at (x,y) coordinates,
//extends for dimensions 'width' and 'height'.
//All points will have the given vga_char.
void c_figure_maker_add_down_triangle(CFigureMaker *fig, int x, int y, const char vga_char[5],
		const char *comment, int width, int height);
void c_figure_maker_add_up_triangle(CFigureMaker *fig, int x, int y, const char vga_char[5],
		const char *comment, int width, int height);

void c_figure_maker_merge(CFigureMaker *fig);
void c_figure_maker_undo(CFigureMaker *fig);

VgaCharMatrix *c_figure_maker_get_matrix(const CFigureMaker *fig);
void c_figure_maker_destroy_matrix(VgaCharMatrix *m);
void c_figure_maker_save_to_file(const CFigureMaker *fig, FILE *fp);
void c_figure_maker_display(const CFigureMaker *fig);



#ifdef __cplusplus
}
#endif

#endif
