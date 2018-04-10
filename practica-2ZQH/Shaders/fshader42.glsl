/*****************************
 * File: fshader42.glsl
 *       A simple fragment shader
 *****************************/

 //#version 150  // YJC: Comment/un-comment this line to resolve compilation errors
                 //      due to different settings of the default GLSL version
#version 120

varying  vec4 color;
// vec4 fColor;

void main() 
{ 
    gl_FragColor = color;
} 

