include <BOSL2/std.scad>
include <BOSL2/ball_bearings.scad>
include <roller.scad>

d=25;
rounding_r=2;
t=9.8;
bracket_offset=5;
support_height=38;



prismoid(size1=[30, 30],
				size2=[15, 25],
				h=support_height,
				rounding1=10,
				rounding2=5)
	attach(TOP, BOT)
		diff()
			cuboid([15, 25, bracket_offset],
						rounding=5,
						except=[TOP, BOT])
				attach(BOT, BOT, inside=true)
					xcyl(d=25, h=t, rounding=rounding_r);
