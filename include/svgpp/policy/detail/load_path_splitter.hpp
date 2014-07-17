#pragma once

namespace svgpp { namespace detail
{

template<class Context, class LoadPathPolicy1, class LoadPathPolicy2>
struct load_path_splitter
{
  template<class Coordinate, class AbsoluteOrRelative>
  static void path_move_to(Context & context, Coordinate x, Coordinate y, AbsoluteOrRelative absoluteOrRelative)
  { 
    LoadPathPolicy1::path_move_to(context.first , x, y, absoluteOrRelative); 
    LoadPathPolicy2::path_move_to(context.second, x, y, absoluteOrRelative); 
  }

  template<class Coordinate, class AbsoluteOrRelative>
  static void path_line_to(Context & context, Coordinate x, Coordinate y, AbsoluteOrRelative absoluteOrRelative)
  { 
    LoadPathPolicy1::path_line_to(context.first , x, y, absoluteOrRelative); 
    LoadPathPolicy2::path_line_to(context.second, x, y, absoluteOrRelative); 
  }

  template<class Coordinate, class AbsoluteOrRelative>
  static void path_line_to_ortho(Context & context, Coordinate coord, bool horizontal, AbsoluteOrRelative absoluteOrRelative)
  { 
    LoadPathPolicy1::path_line_to_ortho(context.first , coord, horizontal, absoluteOrRelative); 
    LoadPathPolicy2::path_line_to_ortho(context.second, coord, horizontal, absoluteOrRelative); 
  }

  template<class Coordinate, class AbsoluteOrRelative>
  static void path_cubic_bezier_to(Context & context, Coordinate x1, Coordinate y1, 
                                        Coordinate x2, Coordinate y2, 
                                        Coordinate x, Coordinate y, 
                                        AbsoluteOrRelative absoluteOrRelative)
  { 
    LoadPathPolicy1::path_cubic_bezier_to(context.first , x1, y1, x2, y2, x, y, absoluteOrRelative); 
    LoadPathPolicy2::path_cubic_bezier_to(context.second, x1, y1, x2, y2, x, y, absoluteOrRelative); 
  }

  template<class Coordinate, class AbsoluteOrRelative>
  static void path_cubic_bezier_to(Context & context, 
                                        Coordinate x2, Coordinate y2, 
                                        Coordinate x, Coordinate y, 
                                        AbsoluteOrRelative absoluteOrRelative)
  { 
    LoadPathPolicy1::path_cubic_bezier_to(context.first , x2, y2, x, y, absoluteOrRelative); 
    LoadPathPolicy2::path_cubic_bezier_to(context.second, x2, y2, x, y, absoluteOrRelative); 
  }

  template<class Coordinate, class AbsoluteOrRelative>
  static void path_quadratic_bezier_to(Context & context, 
                                        Coordinate x1, Coordinate y1, 
                                        Coordinate x, Coordinate y, 
                                        AbsoluteOrRelative absoluteOrRelative)
  { 
    LoadPathPolicy1::path_quadratic_bezier_to(context.first , x1, y1, x, y, absoluteOrRelative); 
    LoadPathPolicy2::path_quadratic_bezier_to(context.second, x1, y1, x, y, absoluteOrRelative); 
  }

  template<class Coordinate, class AbsoluteOrRelative>
  static void path_quadratic_bezier_to(Context & context, 
                                        Coordinate x, Coordinate y, 
                                        AbsoluteOrRelative absoluteOrRelative)
  { 
    LoadPathPolicy1::path_quadratic_bezier_to(context.first , x, y, absoluteOrRelative); 
    LoadPathPolicy2::path_quadratic_bezier_to(context.second, x, y, absoluteOrRelative); 
  }

  template<class Coordinate, class AbsoluteOrRelative>
  static void path_elliptical_arc_to(Context & context, 
                                        Coordinate rx, Coordinate ry, Coordinate x_axis_rotation,
                                        bool large_arc_flag, bool sweep_flag, 
                                        Coordinate x, Coordinate y,
                                        AbsoluteOrRelative absoluteOrRelative)
  { 
    LoadPathPolicy1::path_elliptical_arc_to(context.first , rx, ry, x_axis_rotation, large_arc_flag, sweep_flag, x, y, absoluteOrRelative); 
    LoadPathPolicy2::path_elliptical_arc_to(context.second, rx, ry, x_axis_rotation, large_arc_flag, sweep_flag, x, y, absoluteOrRelative); 
  }

  static void path_close_subpath(Context & context)
  { 
    LoadPathPolicy1::path_close_subpath(context.first); 
    LoadPathPolicy2::path_close_subpath(context.second); 
  }

  static void path_exit(Context & context)
  { 
    LoadPathPolicy1::path_exit(context.first); 
    LoadPathPolicy2::path_exit(context.second); 
  }
};

}}