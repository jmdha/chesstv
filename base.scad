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

// Size of small beam around base
beam=2;

// Size of each square, should be higher than u_dim
s_size=6;
s_size_r=s_size/2;

// The number of holes
count=8;

// Padding around holes
padding=2;

size=count*s_size+2*padding;
height=h_offset+u_height+l_height;

// display board
difference() {
// base plate
cube(size=[size, size, height]);
// led holes
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
// cover holes
translate([padding, padding, h_offset])
for (dx=[0:1:1]) {
    for (dy=[0:1:1]) {
        translate([dx*count*s_size, dy*count*s_size])
        cylinder(h=height, r=1);
     }
}
}

// extension
translate([size + 10, 0, 0])
union() {
// baseplate
difference() {
// plate
cube([size, size, 2]);
//// esp
translate([4, 8, -1])
cube([2, 38, 20]);
translate([28, 8, -1])
cube([2, 38, 20]);
// cutouts
//// shift registers
translate([28, 0, 0])
for (dy=[0:1:1]) {
    translate([8, 4 * (1 + dy) + 20 * dy, -1])
    cube([0.8, 20, 20]);
    translate([20, 4 * (1 + dy) + 20 * dy, -1])
    cube([0.8, 20, 20]);
}

}
// legs
//translate([padding, padding, 0])
//for (dx=[0:1:1]) {
//    for (dy=[0:1:1]) {
//        translate([dx*count*s_size, dy*count*s_size])
//        cylinder(h=2*height, r=0.9);
//     }
//}
}
