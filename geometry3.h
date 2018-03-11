#pragma once

#ifndef __INCLUDEGEOMETRY
#define __INCLUDEGOEMETRY

const int numobjects = 1; // ** NEW ** number of objects for buffer 
const int numperobj = 3;
const int ncolors = 4;
GLuint buffers[numperobj*numobjects + ncolors + 1]; // ** NEW ** List of buffers for geometric data 
GLuint objects[numobjects]; // ** NEW ** For each object
GLenum PrimType[numobjects];
GLsizei NumElems[numobjects];


#define BUFFER_OFFSET(bytes) ((GLubyte *) NULL + (bytes))
#define NumberOf(array) (sizeof(array)/sizeof(array[0])) 

enum { Vertices, Colors, Elements }; // For arrays for object 
enum { FLOOR1, CUBE }; // For objects, for the floor


const GLfloat floorverts[4][3] = {
	{ 1, 0.5, 0.0 },{ -1, 0.5, 0.0 },{ -0.5, -0.5, 0.0 },{ 0.5, -0.5, 0.0 }
};
const GLfloat floorcol[4][3] = {
	{ 1.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0 }
};
const GLubyte floorinds[1][4] = { { 0, 1, 2, 3 } };
const GLfloat floortex[4][2] = {
	{ 1.0, 1.0 },{ 0.0, 1.0 },{ 0.0, 0.0 },{ 1.0, 0.0 }
};

// CUBES FOR ADDING TO THE SCENE

const GLfloat wd = 0.1;
const GLfloat ht = 0.5;
const GLfloat _cubecol[4][3] = {
	{ 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0, 0.0, 1.0 },{ 1.0, 1.0, 0.0 } };
const GLfloat cubeverts[8][3] = {
	{ -wd, -wd, 0.0 },{ -wd, wd, 0.0 },{ wd, wd, 0.0 },{ wd, -wd, 0.0 },
{ -wd, -wd, ht },{ wd, -wd, ht },{ wd, wd, ht },{ -wd, wd, ht }
};
GLfloat cubecol[8][3];
const GLubyte cubeinds[6][4] = {
	{ 0, 1, 2, 3 }, // BOTTOM 
{ 4, 5, 6, 7 }, // TOP 
{ 0, 4, 7, 1 }, // LEFT 
{ 0, 3, 5, 4 }, // FRONT
{ 3, 2, 6, 5 }, // RIGHT 
{ 1, 7, 6, 2 }  // BACK
};


void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type);
void initobjectnocol(GLuint object, GLfloat * vert, GLint sizevert, GLubyte * inds, GLint sizeind, GLenum type);
void drawobject(GLuint object);
void initcolorscube(void);
void drawcolor(GLuint object, GLuint color);
void inittexture(const char * filename, GLuint program);
void inittexture_BMP(const char * filename, GLuint program);
void inittexture_PNG(const char * filename, GLuint program, int tex_num);
void drawtexture(GLuint object, GLuint texture);

// This function takes in a vertex, color, index and type array 
// And does the initialization for an object.  
// The partner function below it draws the object 

void initobject(GLuint object, GLfloat * vert, GLint sizevert, GLfloat * col, GLint sizecol, GLubyte * inds, GLint sizeind, GLenum type) {
	int offset = object * numperobj;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices + offset]);
	glBufferData(GL_ARRAY_BUFFER, sizevert, vert, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors + offset]);
	glBufferData(GL_ARRAY_BUFFER, sizecol, col, GL_STATIC_DRAW);
	glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements + offset]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds, GL_STATIC_DRAW);
	PrimType[object] = type;
	NumElems[object] = sizeind;
}

// Very basic code to read a ppm file
// And then set up buffers for texture coordinates
void inittexture(const char * filename, GLuint program) {
	int i, j, k;
	FILE * fp;
	GLint err;
	assert(fp = fopen(filename, "rb"));
	fscanf(fp, "%*s %*d %*d %*d%*c");
	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			for (k = 0; k < 3; k++)
				fscanf(fp, "%c", &(woodtexture[i][j][k]));
	fclose(fp);

	// Set up Texture Coordinates
	glGenTextures(1, texNames);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[numobjects*numperobj + ncolors]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floortex), floortex, GL_STATIC_DRAW);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texNames[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE,
		woodtexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Define a sampler.  See page 709 in red book, 7th ed.
	GLint texsampler;
	texsampler = glGetUniformLocation(program, "tex");
	glUniform1i(texsampler, 0); // Could also be GL_TEXTURE0 
	istex = glGetUniformLocation(program, "istex");
}

void inittexture_BMP(const char * imagepath, GLuint program) {
	int i, j, k;
	FILE * fp;
	GLint err;
	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); }

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n"); }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n"); }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	printf("Width is : %d \n", width);
	printf("Height is : %d \n\n", height);
	//printf("imageSize is : %d \n", imageSize);


	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

										 // Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose(file);

	// Set up Texture Coordinates
	glGenTextures(1, texNames);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[numobjects*numperobj + ncolors]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floortex), floortex, GL_STATIC_DRAW);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texNames[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE,
		data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Define a sampler.  See page 709 in red book, 7th ed.
	GLint texsampler;
	texsampler = glGetUniformLocation(program, "tex");
	glUniform1i(texsampler, 0); // Could also be GL_TEXTURE0 
	istex = glGetUniformLocation(program, "istex");
}

void inittexture_PNG(const char * imagepath, GLuint program, int tex_num) {
	FIBITMAP* bitmap = FreeImage_Load(
		FreeImage_GetFileType(imagepath, 0),
		imagepath);
	FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);
	glGenTextures(1, texNames);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[numobjects*numperobj + ncolors]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floortex), floortex, GL_STATIC_DRAW);
	switch (tex_num) {
	case 0:
		glActiveTexture(GL_TEXTURE0);
		break;
	case 1:
		glActiveTexture(GL_TEXTURE1);
		break;
	case 2:
		glActiveTexture(GL_TEXTURE2);
		break;
	default:
		;
	}
	
	glEnable(GL_TEXTURE_2D);
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texNames[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Define a sampler.  See page 709 in red book, 7th ed.
	GLint texsampler;
	switch (tex_num) {
	case 0:
		texsampler = glGetUniformLocation(program, "tex0");
		glUniform1i(texsampler, 0); // Could also be GL_TEXTURE0 
		break;
	case 1:
		texsampler = glGetUniformLocation(program, "tex1");
		glUniform1i(texsampler, 1); // Could also be GL_TEXTURE0 
		break;
	case 2:
		texsampler = glGetUniformLocation(program, "tex2");
		glUniform1i(texsampler, 2); // Could also be GL_TEXTURE0 
		break;
	default:
		;
	}
	
	istex = glGetUniformLocation(program, "istex");
	FreeImage_Unload(pImage);
}



// Simple function to set the color separately.  Takes out colors
void initobjectnocol(GLuint object, GLfloat * vert, GLint sizevert, GLubyte * inds, GLint sizeind, GLenum type) {
	int offset = object * numperobj;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices + offset]);
	glBufferData(GL_ARRAY_BUFFER, sizevert, vert, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements + offset]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeind, inds, GL_STATIC_DRAW);
	PrimType[object] = type;
	NumElems[object] = sizeind;
}

// Simple function to init a bunch of color buffers for the cube 
void initcolorscube(void) {
	int base = numobjects * numperobj;
	for (int i = 0; i < ncolors; i++) {
		for (int j = 0; j < 8; j++)
			for (int k = 0; k < 3; k++)
				cubecol[j][k] = _cubecol[i][k];
		glBindBuffer(GL_ARRAY_BUFFER, buffers[base + i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubecol), cubecol, GL_STATIC_DRAW);
		glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_COLOR_ARRAY);
	}
}
// And a function to draw with them, similar to drawobject but with color
void drawcolor(GLuint object, GLuint color) {
	int offset = object * numperobj;
	int base = numobjects * numperobj;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices + offset]);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[base + color]); // Set color correctly 
	glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements + offset]);
	glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
}

// And a function to draw with textures, similar to drawobject
void drawtexture(GLuint object, GLuint texture) {
	int offset = object * numperobj;
	int base = numobjects * numperobj + ncolors;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices + offset]);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_VERTEX_ARRAY);

	// Even with texturing, so we can blend if needed.
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors + offset]);
	glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_COLOR_ARRAY);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[base]); // Set texcoords
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements + offset]);
	glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

}

void drawobject(GLuint object) {
	int offset = object * numperobj;
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices + offset]);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors + offset]);
	glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements + offset]);
	glDrawElements(PrimType[object], NumElems[object], GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
}


#endif