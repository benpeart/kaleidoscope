fudge=0.004;

// honeycomb dimensions
stroke = 2;
notch_width = 21;
notch_depth = 1;
distance_apart = 33 + 1/3;
wall_height = 38.1; // 1 1/2"
triangle_height = (distance_apart * sqrt(3)) / 2;
triangle_zaxis_offset=-fudge/2;
num_rings = 10;
radius = distance_apart * num_rings;

honeycomb();

module honeycomb()
{
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
        translate([0,-8 * distance_apart,-1]) rotate(30) cube([1000,1000,100]);
        translate([0,-6 * distance_apart,-1]) rotate(-30) cube([1000,1000,100]);
        */

    }
}
