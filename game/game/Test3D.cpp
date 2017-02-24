#include <math.h>
#ifdef _WIN32
#include <stdio.h>
#endif

#include "Test3D.h"

#include "common.h"
#include "screen.h"
#include "pad.h"
#include "text.h"

#include "fixed_point.h"

#define M_PI 3.141592653589f
#define SCALE (int)(2.0f * 3.141592653589f * (float)(1 << FP_SCALE_SHIFT))
#define NUM_STEPS 2048

static_assert(SCALE <= NUM_STEPS, "scale too small");

fixed_point sine[NUM_STEPS];
fixed_point cosine[NUM_STEPS];

void build_table(void)
{
	for (int count = 0; count < NUM_STEPS; count++)
	{
		fixed_point f;
		f.value = count;
		float v = f.get_float();

		sine[count].set_value_by_float(sinf(v));
		cosine[count].set_value_by_float(cosf(v));
	}
}

fixed_point 
#ifndef _WIN32
//__attribute__((noinline))
#endif
fp_sin(fixed_point f)
{
	int index = f.value;
	while (index < 0)
		index += SCALE;

	while (index > SCALE)
		index -= SCALE;

	return sine[index];
}

fixed_point
#ifndef _WIN32
//__attribute__((noinline))
#endif
fp_cos(fixed_point f)
{
	int index = f.value;
	while (index < 0)
		index += SCALE;

	while (index > SCALE)
		index -= SCALE;

	return cosine[index];
}

#ifdef _WIN32
void print_table(void)
{
	for (float f = -4 * M_PI; f < 4 * M_PI; f += 0.1f)
	{
		fixed_point v;
		v.set_value_by_float(f);

		float s = sinf(f);
		float c = cosf(f);
		fixed_point fs = fp_sin(v);
		fixed_point fc = fp_cos(v);

		printf("%.3f:\t%.3f\t%.3f\t%.3f\t%.3f\n", f, s, c, fs.get_float(), fc.get_float());
	}
}
#endif

#ifdef FP
void glhFrustumf2(float *matrix, fixed_point left, fixed_point right, fixed_point bottom, fixed_point top,
	fixed_point znear, fixed_point zfar)
{
	fixed_point temp, temp2, temp3, temp4;
	temp = znear * fixed_point(2U);
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	matrix[0] = (temp / temp2).get_float();
	matrix[1] = 0U;
	matrix[2] = 0U;
	matrix[3] = 0U;
	matrix[4] = 0U;
	matrix[5] = (temp / temp3).get_float();
	matrix[6] = 0U;
	matrix[7] = 0U;
	matrix[8] = ((right + left) / temp2).get_float();
	matrix[9] = ((top + bottom) / temp3).get_float();
	matrix[10] = ((-zfar - znear) / temp4).get_float();
	matrix[11] = -1;
	matrix[12] = 0U;
	matrix[13] = 0U;
	matrix[14] = ((-temp * zfar) / temp4).get_float();
	matrix[15] = 0U;
}

void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
	float znear, float zfar)
{
	fixed_point ymax, xmax;
	ymax = znear * tanf(fovyInDegrees * M_PI / 360.0f);
	//ymin = -ymax;
	//xmin = -ymax * aspectRatio;
	xmax = ymax * aspectRatio;
	glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void rotationX(float *matrix, float theta)
{
	matrix[0] = 1;				matrix[1] = 0;				matrix[2] = 0;				matrix[3] = 0;
	matrix[4] = 0;				matrix[5] = cosf(theta);	matrix[6] = -sinf(theta);	matrix[7] = 0;
	matrix[8] = 0;				matrix[9] = sinf(theta);	matrix[10] = cosf(theta);	matrix[11] = 0;
	matrix[12] = 0;				matrix[13] = 0;				matrix[14] = 0;				matrix[15] = 1;
}

void rotationY(float *matrix, float theta)
{
	matrix[0] = cosf(theta);	matrix[1] = 0;				matrix[2] = sinf(theta);	matrix[3] = 0;
	matrix[4] = 0;				matrix[5] = 1;				matrix[6] = 0;				matrix[7] = 0;
	matrix[8] = -sinf(theta);	matrix[9] = 0;				matrix[10] = cosf(theta);	matrix[11] = 0;
	matrix[12] = 0;				matrix[13] = 0;				matrix[14] = 0;				matrix[15] = 1;
}


void translate(float *matrix, float x, float y, float z)
{
	matrix[0] = 1;				matrix[1] = 0;				matrix[2] = 0;				matrix[3] = 0;
	matrix[4] = 0;				matrix[5] = 1;				matrix[6] = 0;				matrix[7] = 0;
	matrix[8] = 0;				matrix[9] = 0;				matrix[10] = 1;				matrix[11] = 0;
	matrix[12] = x;				matrix[13] = y;				matrix[14] = z;				matrix[15] = 1;
}
#else
void glhFrustumf2(fixed_point *matrix, fixed_point left, fixed_point right, fixed_point bottom, fixed_point top,
	fixed_point znear, fixed_point zfar)
{
	fixed_point temp, temp2, temp3, temp4;
	temp = znear * fixed_point(2U);
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	matrix[0] = (temp / temp2);
	matrix[1] = 0U;
	matrix[2] = 0U;
	matrix[3] = 0U;
	matrix[4] = 0U;
	matrix[5] = (temp / temp3);
	matrix[6] = 0U;
	matrix[7] = 0U;
	matrix[8] = ((right + left) / temp2);
	matrix[9] = ((top + bottom) / temp3);
	matrix[10] = ((-zfar - znear) / temp4);
	matrix[11].set_value_by_signed_int(-1);
	matrix[12] = 0U;
	matrix[13] = 0U;
	matrix[14] = ((-temp * zfar) / temp4);
	matrix[15] = 0U;
}

void glhPerspectivef2(fixed_point *matrix, float fovyInDegrees, float aspectRatio,
	float znear, float zfar)
{
	fixed_point ymax, xmax;
	ymax = znear * tanf(fovyInDegrees * M_PI / 360.0f);
	//ymin = -ymax;
	//xmin = -ymax * aspectRatio;
	xmax = ymax * aspectRatio;
	glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

//https://open.gl/transformations
void rotationX(fixed_point *matrix, fixed_point theta)
{
	matrix[0] = 1U;				matrix[1] = 0U;				matrix[2] = 0U;				matrix[3] = 0U;
	matrix[4] = 0U;				matrix[5] = fp_cos(theta);	matrix[6] = -fp_sin(theta);	matrix[7] = 0U;
	matrix[8] = 0U;				matrix[9] = fp_sin(theta);	matrix[10] = fp_cos(theta);	matrix[11] = 0U;
	matrix[12] = 0U;			matrix[13] = 0U;			matrix[14] = 0U;			matrix[15] = 1U;
}

void rotationY(fixed_point *matrix, fixed_point theta)
{
	matrix[0] = fp_cos(theta);	matrix[1] = 0U;				matrix[2] = fp_sin(theta);	matrix[3] = 0U;
	matrix[4] = 0U;				matrix[5] = 1U;				matrix[6] = 0U;				matrix[7] = 0U;
	matrix[8] = -fp_sin(theta);	matrix[9] = 0U;				matrix[10] = fp_cos(theta);	matrix[11] = 0U;
	matrix[12] = 0U;			matrix[13] = 0U;			matrix[14] = 0U;			matrix[15] = 1U;
}

void translate(fixed_point *matrix, float x, float y, float z)
{
	matrix[0] = 1U;				matrix[1] = 0U;				matrix[2] = 0U;				matrix[3] = 0U;
	matrix[4] = 0U;				matrix[5] = 1U;				matrix[6] = 0U;				matrix[7] = 0U;
	matrix[8] = 0U;				matrix[9] = 0U;				matrix[10] = 1U;			matrix[11] = 0U;
	matrix[12].set_value_by_float(x);	matrix[13].set_value_by_float(y);		matrix[14].set_value_by_float(z);		matrix[15] = 1U;
}
#endif

Test3D::Test3D()
	: m_cube(3, 1, 1, 1),
	m_angleX(0), m_angleY(-7)
{
	build_table();

	#ifdef _WIN32
	print_table();
	#endif

	#ifdef FP
	float proj[16];
	float trans[16];
	float rotation[16];
	float modelview[16];

	float rot = M_PI / 5;
	#else
	fixed_point proj[16];
	fixed_point trans[16];
	fixed_point rotation[16];
	fixed_point modelview[16];

	fixed_point rot;
	rot.set_value_by_float(M_PI / 5);
	#endif
	glhPerspectivef2(proj, 90, 1.0f, 0.1f, 50.0f);

	translate(trans, 0, 0, -10);

	rotationX(rotation, rot);

	m_cube.matmultf(rotation, trans, modelview);

	m_cube.SetViewport(0, 0, 256, 256);
	m_cube.SetProjectionGivesNDC(true);
	m_cube.SetProjectionMatrix(proj);
	m_cube.SetModelviewMatrix(modelview);
	m_cube.TransformCube();
}

void Test3D::Update(unsigned char pad, unsigned long frame)
{
	#ifdef FP
	float trans[16];
	float rotation1[16], rotation2[16];
	float modelview[16], temp[16];
	#else
	fixed_point trans[16];
	fixed_point rotation1[16], rotation2[16];
	fixed_point modelview[16], temp[16];
	#endif
	translate(trans, 0, 0, -10);

	volatile fixed_point s = fp_sin(m_angleY);
	volatile fixed_point c = fp_cos(m_angleY);

	rotationX(rotation1, m_angleX);
	rotationY(rotation2, m_angleY);

	if (pad & PAD_UP_SH)
		m_angleX += 0.05f;
	else if (pad & PAD_DOWN_SH)
		m_angleX -= 0.05f;
	if (pad & PAD_LEFT_SH)
		m_angleY += 0.05f;
	else if (pad & PAD_RIGHT_SH)
		m_angleY -= 0.05f;

	//m_cube.matmultf(rotation, trans, modelview);
	m_cube.matmultf(rotation1, trans, temp);
	m_cube.matmultf(rotation2, temp, modelview);

	m_cube.SetModelviewMatrix(modelview);
	m_cube.TransformCube();
}

void Test3D::Render(void)
{
	m_cube.RenderCube(0, 1, false, false);
}

void Test3D::Clear(void)
{
	m_cube.RenderCube(0, 0, false, false);
}

