fudge=0.004;

// honeycomb dimensions
stroke = 2;
notch_width = 21;
notch_depth = 1;
distance_apart = 33 + 1/3;
wall_depth = 20;
triangle_height = (distance_apart * sqrt(3)) / 2;
triangle_zaxis_offset=-fudge/2;
num_rings = 10;
radius = distance_apart * num_rings;

// diffuser dimensions
diffuser_thickness=0.4;

// backplate dimensions
backplate_thickness=3.175;
backplate_height=20;
outerwall_thickness=3.175;
chin_length=4;

// knobs
knob_radius=25;
knob_height=10;
knob_stem_radius=2;
knob_stem_height=2;

//diffuser();
honeycomb();
sides();
translate([0, 0, -wall_depth-backplate_height]) linear_extrude(height=backplate_height) backplate();
knobs();

module knobs()
{
    color("silver") rotate(30) translate([radius-2*knob_radius, -radius/2-knob_radius, -wall_depth]) 
    union ()
    {
        translate([0, 0, knob_stem_height]) cylinder(h=knob_height, r=knob_radius, $fn=6);
        cylinder(h=knob_stem_height, r=knob_stem_radius, $fn=6);
    }

    color("silver") rotate(30) translate([radius-2*knob_radius, radius/2+knob_radius, -wall_depth]) 
    union ()
    {
        translate([0, 0, knob_stem_height]) cylinder(h=knob_height, r=knob_radius, $fn=6);
        cylinder(h=knob_stem_height, r=knob_stem_radius, $fn=6);
    }
}

module diffuser()
{
    // draw a hexagon
    color("white") rotate(30) cylinder(h=diffuser_thickness, r=radius, $fn=6);
}

module honeycomb()
{
    color("white") mirror([0, 0, 180]) difference()
    {
        // draw the hexagon
        rotate(30) cylinder(h=wall_depth, r=radius, $fn=6);
        
        // remove notches for the led strips
        for (i = [num_rings / 2 - 1:-1:(-2 * num_rings) + (num_rings / 2)]) // draws the square
        {
            orgin = [triangle_height * -10, distance_apart * i  + (triangle_height - notch_width) / 2, wall_depth - notch_depth];
                                                                               //the + 1 is needed to finish the etch
            translate(orgin) rotate(30)cube([distance_apart * (num_rings * 2 + 1), notch_width,notch_depth]); 
        }
        
        // remove the triangles for the lights
        for (y = [num_rings:-1:-num_rings]) 
        {
            for (x = [-num_rings:2:num_rings])
            {
                translate([x * triangle_height + (triangle_height * (2/3)), y * distance_apart, triangle_zaxis_offset]) rotate(60) 
                    cylinder(h=wall_depth+fudge, r=(triangle_height* 2/3) - stroke/2, $fn=3);
                translate([x * triangle_height - (triangle_height * (2/3)), y * distance_apart, triangle_zaxis_offset]) rotate(0)
                    cylinder(h=wall_depth+fudge, r=(triangle_height* 2/3) - stroke/2, $fn=3);
                
                translate([(x+1) * triangle_height + (triangle_height * (2/3)), (y) * distance_apart + distance_apart / 2, triangle_zaxis_offset]) rotate(60) 
                    cylinder(h=wall_depth+fudge, r=(triangle_height* 2/3) - stroke/2, $fn=3);
                translate([(x+1) * triangle_height - (triangle_height * (2/3)), (y) * distance_apart + distance_apart / 2, triangle_zaxis_offset]) rotate(0)
                    cylinder(h=wall_depth+fudge, r=(triangle_height* 2/3) - stroke/2, $fn=3);        
            }
        }

        /*
        // shrink the size for testing purposes
        translate([0,0,-1]) rotate(60) cube([1000,1000,100]);
        translate([0,0,-1]) rotate(120) cube([1000,1000,100]);
        translate([0,0,-1]) rotate(180) cube([1000,1000,100]);
        translate([0,0,-1]) rotate(-0) cube([1000,1000,100]);
        translate([0,0,-1]) rotate(-30) cube([1000,1000,100]);
        translate([0,-8 * distance_apart,-1]) rotate(30) cube([1000,1000,100]);
        translate([0,-6 * distance_apart,-1]) rotate(-30) cube([1000,1000,100]);
        */

    }
}

module sides()
{
    // walls and bezel
    rotate(30) translate([0, 0, -wall_depth]) 
    difference()
    {
        cylinder(h=wall_depth+outerwall_thickness+diffuser_thickness, r=radius+outerwall_thickness, $fn=6);
        cylinder(h=wall_depth+diffuser_thickness, r=radius, $fn=6);
        translate([0, 0, wall_depth-diffuser_thickness]) cylinder(h=wall_depth+diffuser_thickness, r=radius-outerwall_thickness, $fn=6);
    }
}

module backplate()
{
    // 'magic' correction - not sure why it is needed
    chin_width_correction = 0.5;
    color("brown") translate([0, 0, -wall_depth-backplate_thickness]) 
    rotate(30) union ()
    {
        circle(r=radius+outerwall_thickness, $fn=6);
        translate([radius-triangle_height*num_rings+chin_length, -triangle_height*num_rings-outerwall_thickness+chin_width_correction, 0]) square([triangle_height*num_rings, triangle_height*num_rings*2+outerwall_thickness*2-chin_width_correction*2]);
    }
}

