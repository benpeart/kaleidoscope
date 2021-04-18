stroke = 2;
notch_width = 21;
notch_depth = 1;
distance_apart = 33 + 1/3;
wall_depth = 20;
triangle_height = (distance_apart * sqrt(3)) / 2;
num_rings = 10;
radius = 0.0000001;
////////////////////////////////////////////////////////////////

//draws an equal triangle with the point facing to the right >
module triangleOne(x, y) 
{
    hull()
    {
        //bottom left point
        translate([x + stroke / 2.0, y + stroke])                                           circle(radius);
        //top left point
        translate([x + stroke / 2.0, y + (distance_apart - stroke)])                        circle(radius);
        //middle right point
        translate([x + triangle_height - stroke, y + distance_apart - distance_apart / 2])  circle(radius);     
    } 
}

//draws an equal triangle with the point facing to the left <
module triangleTwo(x,y)
{
    hull()
    {   
        //bottom right point
        translate([x - stroke / 2, y + stroke])                                             circle(radius);
        //top right point
        translate([x - stroke / 2, y + distance_apart - stroke])                            circle(radius);
        //middle left point
        translate([x - triangle_height + (stroke), y + distance_apart / 2])                 circle(radius);     
    } 
}

//iterative approach to drawing the large hexagon and subrtacting triangles from it
module honeycomb()
{
    //x = 0;
    //y = distance_apart * num_rings;
    
    linear_extrude(height=wall_depth) difference()
    {
        // FIX:
        // 
        // The hexagon doesn't account for stroke, and thus only half of the stroke
        // is left on the end making the final wall thinner than the interior walls. 
        
        //draws the large hexagon
        polygon(points=[[0 - (triangle_height * num_rings), 0 - ((num_rings / 2) * distance_apart)],[0 - (triangle_height * num_rings), 0 + ((num_rings / 2) * distance_apart)],[0, distance_apart * num_rings],[0 + (triangle_height * num_rings), 0 + ((num_rings / 2) * distance_apart)],[0 + (triangle_height * num_rings), 0 - ((num_rings / 2) * distance_apart)],[0, 0 - (distance_apart * num_rings)]],faces=[[0,1,0],[1,2],[2,3],[3,4],[4,5],[5,0]]);
        
        //punches out each of the triangles for the 'honeycomb' pattern
        for (i = [0:1:num_rings - 1]) //i = 0; i < 4; i++
        {
            triangleOne(0, distance_apart * i);
            triangleTwo(0, distance_apart * i);
            triangleOne(0, distance_apart * (-i - 1));
            triangleTwo(0, distance_apart * (-i - 1));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * 2, (distance_apart * i));
            triangleTwo(triangle_height * 2, (distance_apart * i));
            triangleOne(triangle_height * 2, (distance_apart * (-i - 1)));
            triangleTwo(triangle_height * 2, (distance_apart * (-i - 1)));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * 3, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height * 3, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height * 3, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height * 3, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * 4, (distance_apart * i));
            triangleTwo(triangle_height * 4, (distance_apart * i));
            triangleOne(triangle_height * 4, (distance_apart * (-i - 1)));
            triangleTwo(triangle_height * 4, (distance_apart * (-i - 1)));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * 5, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height * 5, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height * 5, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height * 5, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * 6, (distance_apart * i));
            triangleTwo(triangle_height * 6, (distance_apart * i));
            triangleOne(triangle_height * 6, (distance_apart * (-i - 1)));
            triangleTwo(triangle_height * 6, (distance_apart * (-i - 1)));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * 7, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height * 7, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height * 7, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height * 7, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * 8, (distance_apart * i));
            triangleTwo(triangle_height * 8, (distance_apart * i));
            triangleOne(triangle_height * 8, (distance_apart * (-i - 1)));
            triangleTwo(triangle_height * 8, (distance_apart * (-i - 1)));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * 9, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height * 9, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height * 9, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height * 9, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleTwo(triangle_height * 10, (distance_apart * i));
            triangleTwo(triangle_height * 10, (distance_apart * (-i - 1)));
        }
        
        ///////////////////////////////////////////////////////////////////////////
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * -1, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height * -1, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height * -1, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height * -1, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1]) //i = 0; i < 4; i++
        {
            triangleOne(triangle_height * -2, distance_apart * i);
            triangleTwo(triangle_height * -2, distance_apart * i);
            triangleOne(triangle_height * -2, distance_apart * (-i - 1));
            triangleTwo(triangle_height * -2, distance_apart * (-i - 1));
        }
          
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * -3, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height * -3, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height * -3, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height * -3, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * -4, (distance_apart * i));
            triangleTwo(triangle_height * -4, (distance_apart * i));
            triangleOne(triangle_height * -4, (distance_apart * (-i - 1)));
            triangleTwo(triangle_height * -4, (distance_apart * (-i - 1)));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * -5, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height * -5, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height * -5, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height * -5, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * -6, (distance_apart * i));
            triangleTwo(triangle_height * -6, (distance_apart * i));
            triangleOne(triangle_height * -6, (distance_apart * (-i - 1)));
            triangleTwo(triangle_height * -6, (distance_apart * (-i - 1)));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * -7, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height * -7, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height * -7, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height * -7, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * -8, (distance_apart * i));
            triangleTwo(triangle_height * -8, (distance_apart * i));
            triangleOne(triangle_height * -8, (distance_apart * (-i - 1)));
            triangleTwo(triangle_height * -8, (distance_apart * (-i - 1)));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * -9, (distance_apart * i) - (distance_apart / 2));
            triangleTwo(triangle_height * -9, (distance_apart * i) - (distance_apart / 2));
            triangleOne(triangle_height * -9, (distance_apart * (-i - 1)) - (distance_apart / 2));
            triangleTwo(triangle_height * -9, (distance_apart * (-i - 1)) - (distance_apart / 2));
        }
        
        for (i = [0:1:num_rings - 1])
        {
            triangleOne(triangle_height * -10, (distance_apart * i));
            triangleTwo(triangle_height * -10, (distance_apart * i));
            triangleOne(triangle_height * -10, (distance_apart * (-i - 1)));
            triangleTwo(triangle_height * -10, (distance_apart * (-i - 1)));
        }  
   
        
    }
    for (i = [num_rings / 2 - 1:-1:(-2 * num_rings) + (num_rings / 2)]) // draws the square
        {
            orgin = [triangle_height * -10, distance_apart * i  + (triangle_height - notch_width) / 2, wall_depth + 4, ];
            translate(orgin) rotate(30)square([distance_apart * (num_rings * 2 + 1), notch_width]); //the + 1 is needed to finish the etch
        }
}

//series of walls that make 4 triangles
module walls()
{
    wall_orgin = [0,0,0];
    
    linear_extrude(wall_height) union()
    {
        //outside triangle        
        square([wall_length * 2, wall_depth]);
        rotate(60,0,0) square ([wall_length * 2, wall_depth]);
        translate(wall_orgin) translate([distance_apart, triangle_height * 2]) rotate(-60,0,0) square ([wall_length * 2, wall_depth]);
        
        //inside triangle
        translate(wall_orgin) translate([distance_apart / 2, triangle_height, 0])square([wall_length, wall_depth]);
        translate(wall_orgin) translate([distance_apart, 0, 0]) rotate(60,0,0) square ([wall_length, wall_depth]);
        translate(wall_orgin) translate([distance_apart / 2, triangle_height, 0]) rotate(-60,0,0) square ([wall_length, wall_depth]);
    }
}

//walls();
honeycomb();
