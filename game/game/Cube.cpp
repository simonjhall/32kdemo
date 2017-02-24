/*
 * It's Simon's Magic (software-rendered) Cube Class!
 * 
 * This'll drawn a multi-coloured 3D cube onto an image, using OpenCV to do the actual drawing.
 * The cube's position in screen space is calculated by taking the product of the projection
 * and modelview matrices and then multiplying each vertex of the cube with that phat matrix.
 * 
 * This will give, for each vertex, a position in ideal screen space (from -1.0 to 1.0), so it
 * then transforms it to the actual screen co-ords (once you give it the screen width and height)
 * 
 * To draw the faces, some kind of z-testing is required in order to sort the quads from near to far.
 * This is done by using backface-culling rather than a Z-buffer, as this'd need per-fragment Z info
 * in order to do it properly.
 * 
 * As a result, this only really works with a cube as you can be sure you can only see a max
 * of three sides.
 * 
 * The cube's faces are Lambert-shaded, and so are a function of the position of the cube relative to
 * the light source. There's no scope for an alpha channel.
 */  

#include <string.h>
#include <stdio.h>

#include "Cube.h"

#include "common.h"
#include "screen.h"


namespace RnD
{

//set up the cube
Cube::Cube(float scale, float x_scale, float y_scale, float z_scale)
{
	//let's clear everything first
	memset(this, 0, sizeof(Cube));

	projectionInNdc = false;
	
	//set up the eight vertices of the cube
	cube[0][0] = scale * x_scale;
	cube[0][1] = scale * y_scale;
	cube[0][2] = scale * -z_scale;
	cube[0][3] = 1.0f;

	cube[1][0] = scale * x_scale;
	cube[1][1] = scale * -y_scale;
	cube[1][2] = scale * -z_scale;
	cube[1][3] = 1.0f;

	cube[2][0] = scale * -x_scale;
	cube[2][1] = scale * -y_scale;
	cube[2][2] = scale * -z_scale;
	cube[2][3] = 1.0f;

	cube[3][0] = scale * -x_scale;
	cube[3][1] = scale * y_scale;
	cube[3][2] = scale * -z_scale;
	cube[3][3] = 1.0f;

///
	cube[4][0] = scale * x_scale;
	cube[4][1] = scale * y_scale;
	cube[4][2] = scale * z_scale;
	cube[4][3] = 1.0f;

	cube[5][0] = scale * x_scale;
	cube[5][1] = scale * -y_scale;
	cube[5][2] = scale * z_scale;
	cube[5][3] = 1.0f;

	cube[6][0] = scale * -x_scale;
	cube[6][1] = scale * -y_scale;
	cube[6][2] = scale * z_scale;
	cube[6][3] = 1.0f;

	cube[7][0] = scale * -x_scale;
	cube[7][1] = scale * y_scale;
	cube[7][2] = scale * z_scale;
	cube[7][3] = 1.0f;

//set up the faces of the cube by pointing them at the right vertices
	//bottom
	/*cube_indices[0][0] = 3;
	cube_indices[0][1] = 2;
	cube_indices[0][2] = 1;
	cube_indices[0][3] = 0;

	//top
	cube_indices[1][0] = 4;
	cube_indices[1][1] = 5;
	cube_indices[1][2] = 6;
	cube_indices[1][3] = 7;

	//left
	cube_indices[2][0] = 0;
	cube_indices[2][1] = 1;
	cube_indices[2][2] = 5;
	cube_indices[2][3] = 4;

	//right
	cube_indices[3][0] = 2;
	cube_indices[3][1] = 3;
	cube_indices[3][2] = 7;
	cube_indices[3][3] = 6;

	//front
	cube_indices[4][0] = 0;
	cube_indices[4][1] = 4;
	cube_indices[4][2] = 7;
	cube_indices[4][3] = 3;

	//back
	cube_indices[5][0] = 1;
	cube_indices[5][1] = 2;
	cube_indices[5][2] = 6;
	cube_indices[5][3] = 5;
	*/
//set a normal for each face
/*	face_normals[0][0] = 0.0f;
	face_normals[0][1] = 0.0f;
	face_normals[0][2] = -1.0f;
	face_normals[0][3] = 0.0f;

	face_normals[1][0] = 0.0f;
	face_normals[1][1] = 0.0f;
	face_normals[1][2] = 1.0f;
	face_normals[1][3] = 0.0f;

	face_normals[2][0] = 1.0f;
	face_normals[2][1] = 0.0f;
	face_normals[2][2] = 0.0f;
	face_normals[2][3] = 0.0f;

	face_normals[3][0] = -1.0f;
	face_normals[3][1] = 0.0f;
	face_normals[3][2] = 0.0f;
	face_normals[3][3] = 0.0f;

	face_normals[4][0] = 0.0f;
	face_normals[4][1] = 1.0f;
	face_normals[4][2] = 0.0f;
	face_normals[4][3] = 0.0f;

	face_normals[5][0] = 0.0f;
	face_normals[5][1] = -1.0f;
	face_normals[5][2] = 0.0f;
	face_normals[5][3] = 0.0f;

//and let's give each one a pretty colour, in RGB space
	face_colours[0][0] = 255;
	face_colours[0][1] = 255;
	face_colours[0][2] = 255;

	face_colours[1][0] = 255;
	face_colours[1][1] = 0;
	face_colours[1][2] = 0;

	face_colours[2][0] = 0;
	face_colours[2][1] = 255;
	face_colours[2][2] = 0;
	
	face_colours[3][0] = 0;
	face_colours[3][1] = 0;
	face_colours[3][2] = 255;

	face_colours[4][0] = 255;
	face_colours[4][1] = 255;
	face_colours[4][2] = 0;

	face_colours[5][0] = 0;
	face_colours[5][1] = 255;
	face_colours[5][2] = 255;
	
//set the default light
	light_vector[0] = 0.0f;
	light_vector[1] = 0.0f;
	light_vector[2] = 1.0f;
	light_vector[3] = 0.0f;*/
}

Cube::~Cube()
{
}

//multiply matrix a by b and store result in out
//the matrices need to be 4x4, stored as an array of 16 floats
void Cube::matmultf(float* a, float* b, fixed_point* out)
{
	float temp[4][4];       // temporary matrix for storing result
	int i, j;               // row and column counters

	for (j = 0; j < 4; j++)         // transform by columns first
		for (i = 0; i < 4; i++)     // then by rows
			temp[i][j] = a[i * 4 + 0] * b[0 * 4 + j] + a[i * 4 + 1] * b[1 * 4 + j] +
			a[i * 4 + 2] * b[2 * 4 + j] + a[i * 4 + 3] * b[3 * 4 + j];

	for (i = 0; i < 4; i++)         // copy result matrix into matrix a
		for (j = 0; j < 4; j++)
			out[i * 4 + j] = temp[i][j];
}

void Cube::matmultf(fixed_point* a, fixed_point* b, fixed_point* out)
{
	fixed_point temp[4][4];       // temporary matrix for storing result
	int i, j;               // row and column counters

	for (j = 0; j < 4; j++)         // transform by columns first
		for (i = 0; i < 4; i++)     // then by rows
			temp[i][j] = a[i * 4 + 0] * b[0 * 4 + j] + a[i * 4 + 1] * b[1 * 4 + j] +
			a[i * 4 + 2] * b[2 * 4 + j] + a[i * 4 + 3] * b[3 * 4 + j];

	for (i = 0; i < 4; i++)         // copy result matrix into matrix a
		for (j = 0; j < 4; j++)
			out[i * 4 + j] = temp[i][j];
}



void Cube::matmultf(float* a, float* b, float* out)
{
	float temp[4][4];       // temporary matrix for storing result
	int i, j;               // row and column counters

	for (j = 0; j < 4; j++)         // transform by columns first
		for (i = 0; i < 4; i++)     // then by rows
			temp[i][j] = a[i * 4 + 0] * b[0 * 4 + j] + a[i * 4 + 1] * b[1 * 4 + j] +
			a[i * 4 + 2] * b[2 * 4 + j] + a[i * 4 + 3] * b[3 * 4 + j];

	for (i = 0; i < 4; i++)         // copy result matrix into matrix a
		for (j = 0; j < 4; j++)
			out[i * 4 + j] = temp[i][j];
}

//mutliply vector v by matrix m, with result stored in out
//v is a 4 floats, m is 16 floats, out is 4 floats
void Cube::vectmatmultf(const fixed_point* v, const fixed_point* m, fixed_point* out) const
{
	out[0] =  v[0] * m[0 * 4 + 0]
			+ v[1] * m[1 * 4 + 0]
			+ v[2] * m[2 * 4 + 0]
			+ v[3] * m[3 * 4 + 0];

	out[1] =  v[0] * m[0 * 4 + 1]
			+ v[1] * m[1 * 4 + 1]
			+ v[2] * m[2 * 4 + 1]
			+ v[3] * m[3 * 4 + 1];

	out[2] =  v[0] * m[0 * 4 + 2]
			+ v[1] * m[1 * 4 + 2]
			+ v[2] * m[2 * 4 + 2]
			+ v[3] * m[3 * 4 + 2];

	out[3] =  v[0] * m[0 * 4 + 3]
			+ v[1] * m[1 * 4 + 3]
			+ v[2] * m[2 * 4 + 3]
			+ v[3] * m[3 * 4 + 3];
}

//return the dot product of the two four-element float vectors
float Cube::dotproductf(float* v1, float* v2)
{
	return	  v1[0] * v2[0]
			+ v1[1] * v2[1]
			+ v1[2] * v2[2]
			+ v1[3] * v2[3];
}

void Cube::CalcCombinedMatrix(void)
{
	matmultf(gl_para, gl_cpara, combined);
	isMatrixCached = true;
}

void Cube::SetProjectionGivesNDC( bool bState )
{
	projectionInNdc = bState;
	isTransformed = false;
	isMatrixCached = false;
}

//take a copy of the projection matrix
void Cube::SetProjectionMatrix(float *gl_cpara)
{
	memcpy(this->gl_cpara, gl_cpara, 16 * sizeof(float));

	isTransformed = false;
	isMatrixCached = false;
}

void Cube::SetProjectionMatrix(fixed_point *gl_cpara)
{
	memcpy(this->gl_cpara, gl_cpara, 16 * sizeof(fixed_point));

	isTransformed = false;
	isMatrixCached = false;
}


//take a copy of the modelview matrix
void Cube::SetModelviewMatrix(float *gl_para)
{
	memcpy(this->gl_para, gl_para, 16 * sizeof(float));
	
	isTransformed = false;
	isMatrixCached = false;
}


void Cube::SetModelviewMatrix(fixed_point *gl_para)
{
	memcpy(this->gl_para, gl_para, 16 * sizeof(fixed_point));

	isTransformed = false;
	isMatrixCached = false;
}

//set the viewport position and it's size
void Cube::SetViewport(int originX, int originY, int width, int height)
{
	m_originX = originX;
	m_originY = originY;
	
	m_screenWidth = width;
	m_screenHeight = height;
	
	isViewPortSet = true;
	isTransformed = false;
}

//set the light position via a vector
/*void Cube::SetLightPosition(float *v)
{
	memcpy(light_vector, v, sizeof(float) * 4);
}

//and the long version
void Cube::SetLightPosition(float x, float y, float z, float w)
{
	light_vector[0] = x;
	light_vector[1] = y;
	light_vector[2] = z;
	light_vector[3] = w;
}

void Cube::SetFaceColour(int face, unsigned char r, unsigned char g, unsigned char b)
{
	ASSERT(face < 6);
	
	face_colours[face][0] = r;
	face_colours[face][1] = g;
	face_colours[face][2] = b;
}*/

/*void Cube::Translate(float x, float y, float z)
{
	for (int count = 0; count < 8; count++)
	{
		cube[count][0] += x;
		cube[count][1] += y;
		cube[count][2] += z;
	}
}

void Cube::Scale(float x, float y, float z)
{
	for (int count = 0; count < 8; count++)
	{
		cube[count][0] *= x;
		cube[count][1] *= y;
		cube[count][2] *= z;
	}
}*/

// Transform single vertex by composite matrix. It is safe for v and vt to point to the same vector.
/*void Cube::TransformSingleVertex(float* v, float* vt) const
{
	float transformed_vertex[4];
	vectmatmultf(v, combined, transformed_vertex);
	vt[0] = transformed_vertex[0]/transformed_vertex[3];
	vt[1] = transformed_vertex[1]/transformed_vertex[3];
	vt[2] = transformed_vertex[2]/transformed_vertex[3];
	vt[3] = 1.f;
}*/

//transform the cube's vertices and it's normals into screen space
void Cube::TransformCube(bool force_transform, bool force_calc_combined)
{
	ASSERT(isViewPortSet);						//you need to set the viewport first
	
	if (!isTransformed || force_transform)
	{
		fixed_point transformed_vertex[4];
		
		if (!isMatrixCached || force_calc_combined)		//make the fat matrix
			CalcCombinedMatrix();
		
		isBehind = false;

		if (projectionInNdc)
			for (int vertex = 0; vertex < 8; vertex++)
			{
				vectmatmultf(cube[vertex], combined, transformed_vertex);		//transform that vertex

				transformed_vertex[0] = (transformed_vertex[0] / transformed_vertex[3] + fixed_point(1U)) / fixed_point(2U)		//make it 0.0-1.0
					* fixed_point(m_screenWidth) + fixed_point(m_originX);							//adjust for viewport

				transformed_vertex[1] = fixed_point(m_screenHeight) - ((transformed_vertex[1] / transformed_vertex[3] + fixed_point(1U)) / fixed_point(2U)
					* fixed_point(m_screenHeight)) + fixed_point(m_originY);

				transformed_cube[vertex][0] = transformed_vertex[0].value >> FP_SCALE_SHIFT;
				transformed_cube[vertex][1] = transformed_vertex[1].value >> FP_SCALE_SHIFT;

				if (transformed_vertex[2] < 0.5f)
					isBehind = true;
			}
		else
			ASSERT(!"not ndc");
			/*for (int vertex = 0; vertex < 8; vertex++)
			{
				vectmatmultf(cube[vertex], combined, transformed_vertex);		//transform that vertex
				
				transformed_vertex[0] = transformed_vertex[0] / transformed_vertex[3];
				transformed_vertex[1] = transformed_vertex[1] / transformed_vertex[3];

				transformed_cube[vertex][0] = (int)(transformed_vertex[0]+.5f);
				transformed_cube[vertex][1] = (int)(transformed_vertex[1]+.5f);

				//fixme, not checking if behind the camera or not
			}*/
		
		//transform and store the face normals
		/*for (int quad = 0; quad < 6; quad++)
			vectmatmultf(face_normals[quad], gl_para, transformed_normals[quad]);*/
		
		isTransformed = true;
	}
}

//render the cube to the image
void Cube::RenderCube(IplImage *image, short col, bool force_render, bool check_winding, int winding_mult, bool shade)
{
	if ((isTransformed || force_render) && isBehind == false)
	{
		draw_fast_line(transformed_cube[0][0], transformed_cube[0][1],
			transformed_cube[1][0], transformed_cube[1][1],
			col);
		draw_fast_line(transformed_cube[0][0], transformed_cube[0][1],
			transformed_cube[3][0], transformed_cube[3][1],
			col);
		draw_fast_line(transformed_cube[0][0], transformed_cube[0][1],
			transformed_cube[4][0], transformed_cube[4][1],
			col);

		draw_fast_line(transformed_cube[1][0], transformed_cube[1][1],
			transformed_cube[2][0], transformed_cube[2][1],
			col);
		draw_fast_line(transformed_cube[1][0], transformed_cube[1][1],
			transformed_cube[5][0], transformed_cube[5][1],
			col);

		draw_fast_line(transformed_cube[2][0], transformed_cube[2][1],
			transformed_cube[3][0], transformed_cube[3][1],
			col);
		draw_fast_line(transformed_cube[2][0], transformed_cube[2][1],
			transformed_cube[6][0], transformed_cube[6][1],
			col);

		draw_fast_line(transformed_cube[3][0], transformed_cube[3][1],
			transformed_cube[7][0], transformed_cube[7][1],
			col);

		draw_fast_line(transformed_cube[4][0], transformed_cube[4][1],
			transformed_cube[5][0], transformed_cube[5][1],
			col);
		draw_fast_line(transformed_cube[4][0], transformed_cube[4][1],
			transformed_cube[7][0], transformed_cube[7][1],
			col);

		draw_fast_line(transformed_cube[5][0], transformed_cube[5][1],
			transformed_cube[6][0], transformed_cube[6][1],
			col);

		draw_fast_line(transformed_cube[6][0], transformed_cube[6][1],
			transformed_cube[7][0], transformed_cube[7][1],
			col);
	}

	#if 0
	if ((isTransformed || force_render) && isBehind == false)
	{
		for (int quad = 0; quad < 6; quad++)
		{
			#if 0
			volatile int a = transformed_cube[cube_indices[quad][1]][0] - transformed_cube[cube_indices[quad][0]][0];
			volatile int b = transformed_cube[cube_indices[quad][3]][1] - transformed_cube[cube_indices[quad][0]][1];
			volatile int c = transformed_cube[cube_indices[quad][3]][0] - transformed_cube[cube_indices[quad][0]][0];
			volatile int d = transformed_cube[cube_indices[quad][1]][1] - transformed_cube[cube_indices[quad][0]][1];
			/*int winding = (transformed_cube[cube_indices[quad][1]][0] - transformed_cube[cube_indices[quad][0]][0])
							* (transformed_cube[cube_indices[quad][3]][1] - transformed_cube[cube_indices[quad][0]][1])
							- (transformed_cube[cube_indices[quad][3]][0] - transformed_cube[cube_indices[quad][0]][0])
							* (transformed_cube[cube_indices[quad][1]][1] - transformed_cube[cube_indices[quad][0]][1]);*/
			int winding = a * b - c * d;

			winding = winding * winding_mult;
			
			//check which way the face is pointing - only draw it if it's facing the camera
			if (!check_winding || (winding > 0))
				#endif
			{
				Point pts[4];
				
				pts[0].x = transformed_cube[cube_indices[quad][0]][0]; pts[0].y = transformed_cube[cube_indices[quad][0]][1];
				pts[1].x = transformed_cube[cube_indices[quad][1]][0]; pts[1].y = transformed_cube[cube_indices[quad][1]][1];
				pts[2].x = transformed_cube[cube_indices[quad][2]][0]; pts[2].y = transformed_cube[cube_indices[quad][2]][1];
				pts[3].x = transformed_cube[cube_indices[quad][3]][0]; pts[3].y = transformed_cube[cube_indices[quad][3]][1];			
				
				//OpenCV should check that the lines to be drawn lie within the image,
				//but for some reason this fails when built with the PowerPC compiler

				//sjh: is this still necessary? I think this may have been fixed by the changes to pointer aliasing compiler options
#ifdef PS3
				bool check = true;
				for (int pointcheck = 0; pointcheck < 4; pointcheck++)
				{
					if ((pts[pointcheck].x < 0) || (pts[pointcheck].x >= image->width))
					{
						check = false;
						break;
					}
					if ((pts[pointcheck].y < 0) || (pts[pointcheck].y >= image->height))
					{
						check = false;
						break;
					}
				}
				if (check)
					#endif

					/*for (int count = 0; count < 4; count++)
						plot_pixel(pts[count].x, pts[count].y, 1);*/
					
					for (int count = 0; count < 4; count++)
						draw_fast_line(pts[count].x, pts[count].y, pts[(count + 1) & 3].x, pts[(count + 1) & 3].y, col);
					#if 0
				{
					//draw the face
					if (shade)
						cvFillConvexPoly(image, pts, 4,
							CV_RGB(face_colours[quad][0] *winding_mult* -dotproductf(light_vector, transformed_normals[quad]),
								face_colours[quad][1] *winding_mult* -dotproductf(light_vector, transformed_normals[quad]),
								face_colours[quad][2] *winding_mult* -dotproductf(light_vector, transformed_normals[quad])));
					else
						cvFillConvexPoly(image, pts, 4,
							CV_RGB((double)face_colours[quad][0],
								(double)face_colours[quad][1],
								(double)face_colours[quad][2]));

					//draw the edges of the cube
					cvLine(image, pts[0], pts[1], CV_RGB(0, 0, 0), 1, CV_AA);
					cvLine(image, pts[1], pts[2], CV_RGB(0, 0, 0), 1, CV_AA);
					cvLine(image, pts[2], pts[3], CV_RGB(0, 0, 0), 1, CV_AA);
					cvLine(image, pts[3], pts[0], CV_RGB(0, 0, 0), 1, CV_AA);
				}
				#endif
			}
		}
	}
	#endif
}

}
