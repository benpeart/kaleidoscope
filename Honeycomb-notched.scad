stroke = 2;
notch_width = 21;
notch_depth = 1;
distance_apart = 33 + 1/3;
wall_depth = 20;
triangle_height = (distance_apart * sqrt(3)) / 2;
num_rings = 10;
radius = distance_apart * num_rings;

difference()
{
    rotate(30) cylinder(h=wall_depth, r=radius, $fn=6);
    
    for (i = [num_rings / 2 - 1:-1:(-2 * num_rings) + (num_rings / 2)]) // draws the square
    {
        orgin = [triangle_height * -10, distance_apart * i  + (triangle_height - notch_width) / 2, wall_depth - notch_depth];
                                                                           //the + 1 is needed to finish the etch
        translate(orgin) rotate(30)cube([distance_apart * (num_rings * 2 + 1), notch_width,notch_depth]); 
    }
    
    for (y = [num_rings:-1:-num_rings]) 
    {
        for (x = [-num_rings:2:num_rings])
        {          
            translate([x * triangle_height + (triangle_height * (2/3)), y * distance_apart, -2]) rotate(60) cylinder (h=wall_depth+4, r=(triangle_height* 2/3) - stroke/2, $fn=3);
            translate([x * triangle_height - (triangle_height * (2/3)), y * distance_apart, -2]) rotate(0) cylinder (h=wall_depth+4, r=(triangle_height* 2/3) - stroke/2, $fn=3);
            
            translate([(x+1) * triangle_height + (triangle_height * (2/3)), (y) * distance_apart + distance_apart / 2, -2]) rotate(60) cylinder (h=wall_depth+4, r=(triangle_height* 2/3) - stroke/2, $fn=3);
            translate([(x+1) * triangle_height - (triangle_height * (2/3)), (y) * distance_apart + distance_apart / 2, -2]) rotate(0) cylinder (h=wall_depth+4, r=(triangle_height* 2/3) - stroke/2, $fn=3);        
        }
    }
    /*
    translate([0,0,-1]) rotate(60) cube([1000,1000,100]);
    translate([0,0,-1]) rotate(120) cube([1000,1000,100]);
    translate([0,0,-1]) rotate(180) cube([1000,1000,100]);
    translate([0,0,-1]) rotate(-0) cube([1000,1000,100]);
    translate([0,0,-1]) rotate(-30) cube([1000,1000,100]);
    translate([0,-7 * distance_apart,-1]) rotate(30) cube([1000,1000,100]);
    translate([0,-3 * distance_apart,-1]) rotate(-30) cube([1000,1000,100]);
    */
}
