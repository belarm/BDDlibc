#include <cairo.h>
#include <cairo-svg.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int main() {
	cairo_surface_t *svg = cairo_svg_surface_create((char *)"test.svg",100,100); // 4" square @ 72dpi
	cairo_svg_surface_restrict_to_version(svg,CAIRO_SVG_VERSION_1_2);
	cairo_t *cr = cairo_create(svg);
	cairo_set_line_width(cr,1);
	for(int x = 0; x < 8; x++) {
		for(int y = 0; y < 8; y++) {
			cairo_arc(cr,x*10+5,y*10+5,1,0,2*M_PI);
			cairo_fill(cr);
			cairo_stroke(cr);
		}
	}
	cairo_set_source_rgba(cr,1,0,0,1);
	cairo_set_line_width(cr,.3);
	cairo_move_to(cr,5,5);
	cairo_line_to(cr,15,15);
	cairo_line_to(cr,15,25);
	cairo_stroke(cr);
	cairo_save(cr);
	cairo_surface_flush(svg);
	cairo_surface_finish(svg);
	cairo_surface_destroy(svg);
	cairo_destroy(cr);

	return 0;
}
