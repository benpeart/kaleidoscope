fudge=0.004;

// honeycomb dimensions
stroke = 3.2;   // this needs to be double the wall width you actually want when printing, not sure why...
                // the printer can print in increments of 0.4 mm so we make the outside walls 0.8mm and the inside
                // walls 1.6mm.
notch_width = 21;
notch_depth = 1;
distance_apart = 33 + 1/3;
wall_height = 38.1; // 1 1/2"
triangle_height = (distance_apart * sqrt(3)) / 2;
triangle_zaxis_offset=-fudge/2;
num_rings = 10;
radius = distance_apart * num_rings;

// diffuser dimensions
diffuser_thickness = 3.175; // 1/8 inch

// backboard dimensions
backboard_thickness = 6.35; // 1/4 inch

// side wall dimensions
side_wall_thickness = 19.05;// 3/4 inch
side_wall_height = backboard_thickness + wall_height + (2 * diffuser_thickness);

// chin dimensions
chin_width = (radius + side_wall_thickness) * sin(60) * 2 - (4 * side_wall_thickness);
chin_depth = side_wall_thickness + wall_height;
chin_wall_height = 31.75; // 1 1/4 inch

// knob dimensions
knob_height = 15;
knob_radius = 35;


//
// Draw all the component pieces
//
color("yellow") backboard();
color("blue") honeycomb_sidewall();
//color("black") honeycomb();
color("gray", 0.25) diffuser();

color("brown") chin_sidewall();
//color("green") translate([-559 - side_wall_thickness, -770, backboard_thickness]) rotate(90) resize([72.8, 80.4, 25]) import("OBJ_PCB_ESP32 Kaleidoscope.stl");
color("brown", 0.5) chin_top();

color("silver") translate([chin_width / 4 + 50, radius - chin_depth + 50, chin_wall_height + backboard_thickness]) rotate(15) cylinder(h=knob_height, r=knob_radius, $fn=6);
color("silver") translate([-(chin_width / 4 + 50), radius - chin_depth + 50, chin_wall_height + backboard_thickness]) rotate(45) cylinder(h=knob_height, r=knob_radius, $fn=6);


module backboard()
{
    union()
    {
        // draw the hexagon
        rotate(30)
        cylinder(h=backboard_thickness, r=(radius + diffuser_thickness), $fn=6);
        
        // draw the chin
        translate([side_wall_thickness - chin_width / 2 - diffuser_thickness, chin_depth + side_wall_thickness, 0])
        cube([chin_width - (2 * side_wall_thickness) + (2 *  + diffuser_thickness), radius - (2 * side_wall_thickness) + diffuser_thickness, backboard_thickness]);
    }
}

module diffuser()
{
    // draw the diffuser hexagon
    rotate(30)
    translate([0, 0, backboard_thickness + wall_height]) 
    cylinder(h=diffuser_thickness, r=(radius + diffuser_thickness), $fn=6);
}

module honeycomb_sidewall()
{
    rotate(30)
    difference()
    {
        // draw the expanded hexagon
        cylinder(h=side_wall_height, r=(radius + side_wall_thickness - diffuser_thickness), $fn=6);

        // now remove the face of the honeycomb leaving a lip to hold the contents (diffuser + honeycomb + backboard)
        translate([0, 0, -fudge / 2]) 
        cylinder(h=side_wall_height + fudge, r=radius - diffuser_thickness, $fn=6);

        // now remove bottom of the honeycomb to leave room for the diffuser, honeycomb and backboard
        translate([0, 0, -diffuser_thickness]) 
        cylinder(h=side_wall_height, r=radius, $fn=6);
        
        // remove the chin in two pieces to leave a support
        rotate(-30)
        translate([-chin_width / 2, chin_depth, -fudge/2]) 
        cube([chin_width / 2 - 10, radius, chin_wall_height + diffuser_thickness + fudge]);
        rotate(-30)
        translate([10, chin_depth, -fudge/2]) 
        cube([chin_width / 2 - 10, radius, chin_wall_height + diffuser_thickness + fudge]);
    }
}

module chin_sidewall()
{
    difference()
    {
        // draw the chin
        translate([-chin_width / 2, chin_depth, 0]) 
        cube([chin_width, radius, chin_wall_height]);
        
        // remove the chin insides
        translate([-chin_width / 2 + side_wall_thickness, chin_depth, - fudge / 2]) 
        cube([chin_width - (2 * side_wall_thickness), radius - side_wall_thickness, chin_wall_height + fudge]);

        // remove the expanded hexagon
        rotate(30) translate([0, fudge / 2, 0]) 
        cylinder(h=side_wall_height + fudge, r=radius, $fn=6);
        
        // remove a notch for the backboard
        backboard();
    }
}

module chin_top()
{
    difference()
    {
        // draw a lid for the chin
        translate([-chin_width / 2, chin_depth, chin_wall_height]) 
        cube([chin_width, radius, diffuser_thickness]);
        
        // remove the expanded hexagon
        rotate(30) translate([0, fudge / 2, 0]) 
        cylinder(h=side_wall_height + fudge, r=radius, $fn=6);
    }
}

module honeycomb()
{
    translate([0, 0, backboard_thickness]) 
    difference()
    {
        // draw the hexagon
        rotate(30) cylinder(h=wall_height, r=radius, $fn=6);
        
        // remove notches for the led strips
        for (i = [num_rings / 2 - 1:-1:(-2 * num_rings) + (num_rings / 2)]) // draws the square
        {
            orgin = [triangle_height * -10, distance_apart * i  + (triangle_height - notch_width) / 2, wall_height - notch_depth];
                                                                               //the + 1 is needed to finish the etch
            translate(orgin) rotate(30)cube([distance_apart * (num_rings * 2 + 1), notch_width,notch_depth]); 
        }
        
        // remove the triangles for the lights
        for (y = [num_rings:-1:-num_rings]) 
        {
            for (x = [-num_rings:2:num_rings])
            {
                translate([x * triangle_height + (triangle_height * (2/3)), y * distance_apart, triangle_zaxis_offset]) rotate(60) 
                    cylinder(h=wall_height+fudge, r=(triangle_height* 2/3) - stroke/2, $fn=3);
                translate([x * triangle_height - (triangle_height * (2/3)), y * distance_apart, triangle_zaxis_offset]) rotate(0)
                    cylinder(h=wall_height+fudge, r=(triangle_height* 2/3) - stroke/2, $fn=3);
                
                translate([(x+1) * triangle_height + (triangle_height * (2/3)), (y) * distance_apart + distance_apart / 2, triangle_zaxis_offset]) rotate(60) 
                    cylinder(h=wall_height+fudge, r=(triangle_height* 2/3) - stroke/2, $fn=3);
                translate([(x+1) * triangle_height - (triangle_height * (2/3)), (y) * distance_apart + distance_apart / 2, triangle_zaxis_offset]) rotate(0)
                    cylinder(h=wall_height+fudge, r=(triangle_height* 2/3) - stroke/2, $fn=3);        
            }
        }
        // shrink the size for testing purposes
        translate([0,0,-1]) rotate(60) cube([1000,1000,100]);
        translate([0,0,-1]) rotate(120) cube([1000,1000,100]);
        translate([0,0,-1]) rotate(180) cube([1000,1000,100]);
        translate([0,0,-1]) rotate(-0) cube([1000,1000,100]);
        translate([0,0,-1]) rotate(-30) cube([1000,1000,100]);
/*
        translate([0,-4 * distance_apart,-1]) rotate(30) cube([1000,1000,100]);
        translate([0,-4 * distance_apart,-1]) rotate(-30) cube([1000,1000,100]);
*/
    }
}

