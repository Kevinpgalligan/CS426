#include "colors.inc"
#include "textures.inc"        
#include "metals.inc"
#include "woods.inc"
               
background {
    colour Cyan
}             

camera {
    location <2, 6, -10>
    look_at <0, 0, 0>
}

light_source {
    <0, 10, -2>
    colour White
    fade_distance 3
    fade_power 2
}

plane {
    <0, 1, 0>, -3  
    texture {
        pigment {
            checker colour White, colour Black
        }  
    }
}

#declare hemisphere = difference  {
    sphere {
        <0, 0, 0>, 1
    }
    
    box {
        <-1, -1, -1>, <1, 0, 1>
    }
} 

#declare twisty_part = merge { 
    cylinder {
         <0, 1, 0>, <0, 0, 0>, 0.6
         open
    }

    torus {
         0.6, 0.05
    }
    
    torus {
        0.6, 0.05
        translate y * 0.5
    } 
    
    torus {
        0.6, 0.05
        translate y * 1.0
    }
    
    object {
        hemisphere
        scale 0.6
        rotate <180, 0, 0>
    }
    
    texture {
        T_Brass_1A
    }
}

#declare glass_part = merge {
    sphere {
        <0, 0, 0>, 1 
    }
    
    cylinder {
        <0, 0, 0>, <0, -1.5, 0>, 0.59
        open  
    } 
      
    pigment{
        rgbf <1.0, 1.0, 1.0, 0.6>
    }
}

#declare lightbulb = union {
    object { glass_part translate y * 2.2  }
    object { twisty_part }
    light_source {
        <0, 0, 0>, Yellow
        translate y * 2.2  
        fade_distance 10
        fade_power 2 * sin(clock * 6 * pi) + 1
    }
}

box {
    <-1, -1, -1>, <1, 1, 1>
    texture {
        T_Wood1
    }
}     

object {
    lightbulb
    translate y * -1.3
    translate x * 3.5 
    rotate <0, 360 * clock, 0>
}

