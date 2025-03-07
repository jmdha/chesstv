// Set cylinder smoothing
$fn=16;

// Upper radius of hole
u_dim=2.04;
// Lower radius of hole
l_dim=1.54;

// Offset of hole
h_offset=1;
// Height of upper part of hole
u_height=1.2;
// Height of lower part of hole
l_height=4.2;

// Size of each square, should be higher than u_dim
s_size=6;
s_size_r=s_size/2;

// The number of holes
count=8;

// Padding around holes
padding=6;

size=count*s_size+2*padding;
height=h_offset+u_height+l_height;

// display board
union() {
difference() {
cube(size=[size, size, height / 2]);
translate([padding,padding,0])
for (dx=[0:1:count - 1]) {
    for (dy=[0:1:count - 1]) {
        translate([dx * s_size, dy * s_size, 0])
        union() {
            translate([s_size_r, s_size_r, h_offset])
            cylinder(h=height, r=l_dim);
            translate([s_size_r, s_size_r, h_offset+l_height])
            cylinder(h=height, r=u_dim);
        }
     }
}
}
translate([padding, padding - 2, 0])
difference() {
// plate
cube([size - 2 * padding, 2, size]);
//// esp
translate([10, -1, height + 7.4])
cube([2.5, 20, 38]);
translate([35, -1, height + 7.4])
cube([2.5, 20, 38]);
}
}

