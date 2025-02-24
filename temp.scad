difference() {
    cube(size = [10, 22, 2], center = true);
    translate([4, 0, 0])
        cube([0.8, 20, 22], center = true);
    translate([-4, 0, 0])
        cube([0.8, 20, 22], center = true);
}