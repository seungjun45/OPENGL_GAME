# version 120 
//in vec3 position;
//in vec2 texCoord;

//out vec2 colorCoord;

varying vec4 color ; 
varying vec3 mynormal ; 
varying vec4 myvertex ; 

void main() {
	gl_TexCoord[0] = gl_MultiTexCoord0 ; 
	gl_TexCoord[1] = gl_MultiTexCoord1 ; 
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex ; 
    color = gl_Color ; 
    myvertex = gl_Vertex ; 
	//colorCoord=texCoord;
}
