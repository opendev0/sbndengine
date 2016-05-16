/**
 * \brief This file is primarily used to activate/deactivate
 * specific code parts belonging to worksheets
 */


#ifndef WORKSHEETS_PRECOMPILER_HPP
#define WORKSHEETS_PRECOMPILER_HPP

#define WORKSHEET_1		1	// explicit euler
#define WORKSHEET_2		1	// collisions (sphere-sphere, sphere-plane), interpenetration
#define WORKSHEET_3a	1	// quaternions
#define WORKSHEET_3		1	// springs, ropes, linear momentum
#define WORKSHEET_4		1	// collisions (sphere-box, plane-box), create your own scene
#define WORKSHEET_5		1	// collision (box-box)
#define WORKSHEET_6		0	// angular momentum
#define WORKSHEET_7		0	// friction

#define WORKSHEET_X		1	// other helper functions

#define WORKSHEET_ANGULAR_DAMPING		0	// use angular damping

#define SHADERS         1   // Deactivate this if your GPU/driver does not support shaders

#define EPSILON			1.0f/1000

#endif
