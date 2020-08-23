hpp{{
#include <cairo/cairo.h>
}}

/**
 * Cairo graphics context.
 */
class Context {
  hpp{{
  cairo_t* cr;
  }}

  function destroy() {
    cpp{{
    cairo_destroy(this_()->cr);
    }}
  }
  
  /*
   * Paths
   * -----
   */
  function newPath() {
    cpp{{
    cairo_new_path(this_()->cr);
    }}
  }
  
  function closePath() {
    cpp{{
    cairo_close_path(this_()->cr);
    }}
  }

  function arc(xc:Real, yc:Real, radius:Real, angle1:Real, angle2:Real) {
    cpp{{
    cairo_arc(this_()->cr, xc, yc, radius, angle1, angle2);
    }}
  }

  function arcNegative(xc:Real, yc:Real, radius:Real, angle1:Real, angle2:Real) {
    cpp{{
    cairo_arc_negative(this_()->cr, xc, yc, radius, angle1, angle2);
    }}
  }

  function curveTo(x1:Real, y1:Real, x2:Real, y2:Real, x3:Real, y3:Real) {
    cpp{{
    cairo_curve_to(this_()->cr, x1, y1, x2, y2, x3, y3);
    }}
  }

  function lineTo(x:Real, y:Real) {
    cpp{{
    cairo_line_to(this_()->cr, x, y);
    }}
  }

  function moveTo(x:Real, y:Real) {
    cpp{{
    cairo_move_to(this_()->cr, x, y);
    }}
  }

  function rectangle(x:Real, y:Real, width:Real, height:Real) {
    cpp{{
    cairo_rectangle(this_()->cr, x, y, width, height);
    }}
  }

  function relCurveTo(dx1:Real, dy1:Real, dx2:Real, dy2:Real, dx3:Real, dy3:Real) {
    cpp{{
    cairo_curve_to(this_()->cr, dx1, dy1, dx2, dy2, dx3, dy3);
    }}
  }

  function relLineTo(dx:Real, dy:Real) {
    cpp{{
    cairo_line_to(this_()->cr, dx, dy);
    }}
  }

  function relMoveTo(dx:Real, dy:Real) {
    cpp{{
    cairo_move_to(this_()->cr, dx, dy);
    }}
  }
  
  function stroke() {
    cpp{{
    cairo_stroke(this_()->cr);
    }}
  }

  function strokePreserve() {
    cpp{{
    cairo_stroke_preserve(this_()->cr);
    }}
  }

  function fill() {
    cpp{{
    cairo_fill(this_()->cr);
    }}
  }

  function fillPreserve() {
    cpp{{
    cairo_fill_preserve(this_()->cr);
    }}
  }

  function paint() {
    cpp{{
    cairo_paint(this_()->cr);
    }}
  }
  
  /*
   * Transformations
   * ---------------
   */
  function translate(tx:Real, ty:Real) {
    cpp{{
    cairo_translate(this_()->cr, tx, ty);
    }}
  }

  function scale(sx:Real, sy:Real) {
    cpp{{
    cairo_scale(this_()->cr, sx, sy);
    }}
  }
  
  function rotate(angle:Real) {
    cpp{{
    cairo_rotate(this_()->cr, angle);
    }}
  }
  
  function deviceToUserDistance(ux:Real, uy:Real) -> (Real, Real) {
    ux1:Real <- ux;
    uy1:Real <- uy;
    cpp{{
    cairo_device_to_user_distance(this_()->cr, &ux1, &uy1);
    }}
    return (ux1, uy1);
  }
  
  /*
   * Sources
   * -------
   */
  function setSourceRGB(red:Real, green:Real, blue:Real) {
    cpp{{
    cairo_set_source_rgb(this_()->cr, red, green, blue);
    }}
  }

  function setSourceRGBA(red:Real, green:Real, blue:Real, alpha:Real) {
    cpp{{
    cairo_set_source_rgba(this_()->cr, red, green, blue, alpha);
    }}
  }
  
  function setSource(pattern:Pattern) {
    cpp{{
    cairo_set_source(this_()->cr, pattern->pattern);
    }}
  }
  
  function setLineWidth(width:Real) {
    cpp{{
    cairo_set_line_width(this_()->cr, width);
    }}
  }

  /*
   * Text
   * ----
   */
  function showText(utf8:String) {
    cpp{{
    cairo_show_text(this_()->cr, utf8.c_str());
    }}
  }

  function setFontSize(size:Real) {
    cpp{{
    cairo_set_font_size(this_()->cr, size);
    }}
  }
  
  /*
   * Groups
   * ------
   */
  function pushGroup() {
    cpp{{
    cairo_push_group(this_()->cr);
    }}
  }
  
  function popGroupToSource() {
    cpp{{
    cairo_pop_group_to_source(this_()->cr);
    }}
  }
}

function create(surface:Surface) -> Context {
  cr:Context;
  cpp{{
  cr->cr = cairo_create(surface->surface);
  }}
  return cr;
}