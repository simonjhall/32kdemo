#ifndef _SOFTWARE_CUBE_H_
#define _SOFTWARE_CUBE_H_

#include "fixed_point.h"

struct IplImage;

//#define FP

struct Point
{
	int x, y;
};

namespace RnD
{

class Cube
{
public:
	Cube(float size = 20.0f, float x_scale = 1.0f, float y_scale = 1.0f, float z_scale = 2.0f);
	~Cube();
	
	void	TransformSingleVertex(float* v, float* vt) const;
	void	TransformCube(bool force_transform = false, bool force_calc_combined = false);
	void	RenderCube(IplImage *image, short col, bool force_render = false, bool check_winding = true, int winding_mult = 1, bool shade = true);
	
	void	SetProjectionMatrix(fixed_point *gl_cpara);
	void	SetProjectionMatrix(float *gl_cpara);
	//void	SetProjectionMatrix(double *gl_cpara);
	
	void	SetModelviewMatrix(fixed_point *gl_para);
	void	SetModelviewMatrix(float *gl_para);
	//void	SetModelviewMatrix(double *gl_para);
	
	void	SetViewport(int originX, int originY, int width, int height);
	
	void    SetProjectionGivesNDC( bool bState );
	
	void	SetLightPosition(float *v);
	void	SetLightPosition(float x, float y, float z, float w = 0.0f);
	
	void	SetFaceColour(int face, unsigned char r, unsigned char g, unsigned char b);
	
	void	Scale(float x_scale, float y_scale, float z_scale);
	void	Translate(float x_trans, float y_trans, float z_trans);

	void	CalcCombinedMatrix(void);
	
	void	matmultf(float* a, float* b, fixed_point* out);
	void	matmultf(fixed_point* a, fixed_point* b, fixed_point* out);
	void	matmultf(float* a, float* b, float* out);
	void	vectmatmultf(const fixed_point* v, const fixed_point* m, fixed_point* out) const;
	float	dotproductf(float* v1, float* v2);
private:

	//the co-ordinates in the local object space of each of the cube's vertices
	fixed_point	cube[8][4];
	
	//the co-ordinates in screen space of each of the vertices
	int		transformed_cube[8][2];
	bool	isTransformed;		//set to false if the cube needs to be transformed
	bool	isBehind;
	
	//the indices of the vertices required to make each face
	//int		cube_indices[6][4];
	
	//the normals (of unit length) for each of the faces
/*	float	face_normals[6][4];
	float	transformed_normals[6][4];*/
	
	//the RGB colours of each face
	//int		face_colours[6][3];
	
	#ifdef FP
	float	gl_cpara[16];		//the projection matrix (in OpenGL format)
	float	gl_para[16];		//the modelview matrix (in OpenGL format)
	#else
	fixed_point	gl_cpara[16];		//the projection matrix (in OpenGL format)
	fixed_point gl_para[16];		//the modelview matrix (in OpenGL format)
	#endif
	
	fixed_point	combined[16];		//the product of the projection and modelview matricies
	bool	isMatrixCached;		//false if the combined matrix needs to be recalculated
	
	//float	light_vector[4];
	
	//the viewport
	unsigned int		m_originX;
	unsigned int		m_originY;
	unsigned int		m_screenWidth;
	unsigned int		m_screenHeight;
	
	bool	isViewPortSet;
	bool	projectionInNdc; // if true projection matrix gives (-1,1) coords, if false (0,width) / (0,height)
};

}

#endif /*CUBE_H_*/
