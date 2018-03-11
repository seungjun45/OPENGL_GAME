# version 120 



varying vec4 color ;
varying vec4 myvertex ;
//in vec2 colorCoord;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;




void main (void) 
{       
    //gl_FragColor = color ;  // colors defined by "glColor3f(r,g,b)" in main.cpp
	//gl_FragColor =texture2D(tex0, gl_TexCoord[0].st) ;  //  texture mapping!!


	// Merging two sources!!

	if(myvertex.z == 0 && 15.0f <= myvertex.y && myvertex.y <= 16.0f ) { // vertex in texturing domain
		gl_FragColor = texture2D(tex0, gl_TexCoord[0].st);
	}
	else if(myvertex.z == 3.0f){
		gl_FragColor = texture2D(tex1, gl_TexCoord[1].st);
	}
	else{
		gl_FragColor = color ;
	}
}
