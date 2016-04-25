#ifndef __I_CAMERA_LOOK_AT_HPP__
#define __I_CAMERA_LOOK_AT_HPP__

#include "iCamera.hpp"
#include "libmath/CGlSlMath.hpp"

/**
 * this implements a simple "lookat" cammera
 */
class cCameraLookAt : public iCamera
{
	CVector<3,float> position;
public:
	/**
	 * setup the camera to look at a some point
	 * \param eye		the current view position
	 * \param center	the distant center point to look at
	 * \param up		the up vector of the current view position to create a unique view
	 */
	inline void lookAt(	const GLSL::vec3 &eye,
						const GLSL::vec3 &center,
						const GLSL::vec3 &up
				)
	{
		view_matrix = GLSL::lookAt(eye, center, up);
		position = center;
	}

	inline void computeMatrices()
	{

	}

	/**
	 * return the current camera position
	 */
	inline const CVector<3,float> getPosition()	const
	{
		return position;
	}
};

#endif
