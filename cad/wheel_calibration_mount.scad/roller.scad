include <BOSL2/std.scad>
include <BOSL2/ball_bearings.scad>

bi = ball_bearing_info(trade_size="608ZZ", struct=true);
id = struct_val(bi, "outer_diam");
w = struct_val(bi, "width");
thickness = 4;
od = id + 2 * thickness;
groove_depth = 2;

triangle = [
	[od / 2, 0],
	[od / 2, w],
	[od / 2 - groove_depth, w / 2]
];

module roller(anchor=CENTER, spin=0, orient=UP) {
	attachable(anchor, spin, orient) {
		diff()
			cyl(d=od, h=w)
			tag("remove")
				cyl(d=id, h=w + 1)
				down(w / 2)
				rotate_sweep(shape=triangle, angle=360);
		children();
	}
}

// if ($preview == false)
// 	roller();
