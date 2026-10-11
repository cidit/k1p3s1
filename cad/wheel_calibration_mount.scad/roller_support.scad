
include <BOSL2/std.scad>
include <BOSL2/ball_bearings.scad>
include <roller.scad>

bi = ball_bearing_info(trade_size="608ZZ", struct=true);
d = struct_val(bi, "inner_diam");
w = struct_val(bi, "width");

cuboid([45, 5, 30], rounding=5, edges=[LEFT+TOP, RIGHT+TOP]) {
  attach(FRONT, BOT, align=[TOP + RIGHT, TOP + LEFT], inset=2)
    cyl(d=d + 1, h=3)
      attach(TOP, BOT)
        cyl(d=d+0.15, h=w) *#ball_bearing("608ZZ") roller();
  align(FRONT, align=[BOT+LEFT, BOT+RIGHT])
    wedge([10, 20, 5], spin=180);
}
