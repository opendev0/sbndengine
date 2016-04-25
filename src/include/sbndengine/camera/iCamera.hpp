#ifndef __I_CAMERA_HPP__
#define __I_CAMERA_HPP__

#include "libmath/CGlSlMath.hpp"

/**
 * camera interface which has to be implemented by the camera classes
 */
class iCamera
{
public:
	/**
	 * the view matrix of this camera computed by computeMatrices()
	 */
	CMatrix4<float> view_matrix;

	/**
	 * the projection matrix of this camera computed by computeMatrices()
	 */
	CMatrix4<float> projection_matrix;

	iCamera()
	{
		view_matrix.loadIdentity();
		projection_matrix.loadIdentity();
	}

	/**
	 * setup the projection matrix with a frustum
	 *
	 * TODO: maybe this function should be implemented by the camera implementations.
	 */
	inline void frustum(	float left,
							float right,
							float bottom,
							float top,
							float nearval,
							float farval
					)
	{
		projection_matrix = GLSL::frustum(left, right, bottom, top, nearval, farval);
	}


	/**
	 * return the current camera position
	 */
	virtual const CVector<3,float> getPosition() const = 0;


	/**
	 * this method has to be implemented by the implementations.
	 *
	 * if this method is called, the camera class is assumed to setup
	 * the view_matrix and projection_matrix.
	 */
	virtual void computeMatrices() = 0;
};

#endif //__I_OBJECT_HPP__
