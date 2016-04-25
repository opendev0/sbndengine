/*
 * Copyright 2010 Martin Schreiber
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GM_CINTERSECTIONS_HPP__
#define GM_CINTERSECTIONS_HPP__

/**
 * this file contains various intersection tests for different kinds of primitives
 */


/**
 * intersection tests for point and plane in normal form
 */
template <typename T>
class CIntersectionsPointPlaneNormal
{
public:
	static inline T distance(
			const CVector<3,T> &point,
			const CPlaneNormal<T> &plane
			)
	{
		return (plane.normal.dotProd(point) - plane.d) / plane.normal.dotProd(plane.normal);
	}

	static inline void closestPoint(
			const CVector<3,T> &point,
			const CPlaneNormal<T> &plane,
			CVector<3,T> &closest_point
			)
	{
		T dist = plane.normal.dotProd(point) - plane.d;
		closest_point = point - plane.normal*dist;
	}
};

template <typename T>
class CIntersectionsPointLinePP
{
public:
	/**
	 * return squared distance
	 */
	static inline T distance2(
			const CVector<3,T> &point,
			const CLinePP<T> &line
			)
	{
		CVector<3,T> d1 = line.v1 - line.v0;
		CVector<3,T> d2 = point - line.v0;
		CVector<3,T> d3 = point - line.v1;

		T a = d2.dotProd(d1);
		T b = d1.dotProd(d1);

		return CMath<T>::abs(d2.dotProd(d2) - a*a / b);
	}

	static inline T distance(
			const CVector<3,T> &point,
			const CLinePP<T> &line
			)
	{
		return CMath<T>::sqrt(distance2(point, line));
	}

	static inline T getT(
			const CVector<3,T> &point,
			const CLinePP<T> &line
			)
	{
		CVector<3,T> dir = line.v1 - line.v0;
		return dir.dotProd(point - line.v0) / dir.dotProd(dir);
	}

	static inline void closestPoint(
			const CVector<3,T> &point,
			const CLinePP<T> &line,
			CVector<3,T> &closest_point
			)
	{
		CVector<3,T> dir = line.v1 - line.v0;
		T t = dir.dotProd(point - line.v0) / dir.dotProd(dir);
		closest_point = line.v0 + dir*t;
	}
};


template <typename T>
class CIntersectionsPointPlaneAAOrigin
{

#if WORKSHEET_2

#endif
};



template <typename T>
class CIntersectionsPointBoxAAOrigin
{
#if WORKSHEET_4

#endif
};

template <typename T>
class CIntersectionsPointBoxAA
{

#if WORKSHEET_X
public:
	/**
	 * return distance
	 */
	static inline T distance(
			const CVector<3,T> &point,
			const CBoxAA<T> &aabox
			)
	{
		CVector<3,T> tmp_point;
		clampOuterPointToSurface(point, aabox, tmp_point);
		return point.dist(tmp_point);
	}

	/**
	 * return squared distance
	 */
	static inline T distance2(
			const CVector<3,T> &point,
			const CBoxAA<T> &aabox
			)
	{
		return point.dist2(clampOuterPointToSurface(point, aabox));
	}

	static inline void clampOuterPointToSurface(
			const CVector<3,T> &point,
			const CBoxAA<T> &aabox,
			CVector<3,T> &clamped_point
			)
	{
		clamped_point = point;

		if (clamped_point.data[0] > aabox.max.data[0])	clamped_point.data[0] = aabox.max.data[0];
		else if (clamped_point.data[0] < aabox.min.data[0])	clamped_point.data[0] = aabox.min.data[0];

		if (clamped_point.data[1] > aabox.max.data[1])	clamped_point.data[1] = aabox.max.data[1];
		else if (clamped_point.data[1] < aabox.min.data[1])	clamped_point.data[1] = aabox.min.data[1];

		if (clamped_point.data[2] > aabox.max.data[2])	clamped_point.data[2] = aabox.max.data[2];
		else if (clamped_point.data[2] < aabox.min.data[2])	clamped_point.data[2] = aabox.min.data[2];
	}
#endif
};


/**
 * intersections of a line given by the start position (P) and the direction (D) and
 * a triangle specified by 3 vertices (PPP)
 */
template <typename T>
class CIntersectionsLineVDTrianglePPP
{
#if WORKSHEET_X
public:
	/**
	 * compute the intersection of a ray with a triangle
	 *
	 * \param ray_start_position	the start 3d coordinate of the ray
	 * \param ray_direction			the 3d direction of the ray
	 * \param triangle_vertex0		the 1st vertex of the triangle
	 * \param triangle_vertex1		the 2nd vertex of the triangle
	 * \param triangle_vertex2		the 3rd vertex of the triangle
	 * \param intersection_u		the u component of the intersection
	 * \param intersection_v		the v component of the intersection
	 * \param intersection_distance	the distance to the intersection point, if ray_direction is normalized
	 */
	static bool getIntersectionParameters(
			const CVector<3,T> &ray_start_position,
			const CVector<3,T> &ray_direction,

			const CVector<3,T> &triangle_vertex0,
			const CVector<3,T> &triangle_vertex1,
			const CVector<3,T> &triangle_vertex2,

			T &intersection_u,
			T &intersection_v,
			T &intersection_distance
		)
	{
		CVector<3,T> e1 = triangle_vertex1 - triangle_vertex0;
		CVector<3,T> e2 = triangle_vertex2 - triangle_vertex0;

		CVector<3,T> ret;	// [u,v,t]

		CVector<3,T> p = ray_direction % e2;
		T tmp = p.dotProd(e1);

		/*
		// in the case that the ray hits the back surface, don't compute intersection
		if ((e1 % e2).dotProd(object_direction) > 0)
			continue
		// this can be also checked by the following line with already computed values!
		*/
		if (tmp < 0)	return false;

		CVector<3,T> t;

		if (tmp > 0)
		{
			t = ray_start_position - triangle_vertex0;
		}
		else
		{
			t = triangle_vertex0 - ray_start_position;
			tmp = -tmp;
		}

		if (tmp <= 0.0)	return false;

		// U
		T u = p.dotProd(t);
		if (u > tmp || u < 0.0)	return false;

		// V
		CVector<3,T> q = t % e1;
		T v = q.dotProd(ray_direction);
		if (u + v > tmp || v < 0.0)	return false;

		// T
		T tt = q.dotProd(e2);

		if (tt < 0)	return false;

		T inv_tmp = 1.0f/tmp;
		tt *= inv_tmp;

		intersection_u = u*inv_tmp;
		intersection_v = v*inv_tmp;
		intersection_distance = tt;

		return true;
	}
#endif
};


/**
 * intersection between 2 lines given by its vertices (PP,PP)
 */
template <typename T>
class CIntersectionsLinePPLinePP
{
#if WORKSHEET_4

#endif
};



/**
 * intersection between 2 lines given by one vertex and a direction on each line (PD,PD)
 */
template <typename T>
class CIntersectionsLinePDLinePD
{
#if WORKSHEET_4

#endif
};


/**
 * intersection between a line segments and the borders of an axis aligned plane
 *
 * the difference between a line and a line segment is, that no intersection
 * point is given back if the closest intersection points are not on the segments
 */
template <typename T>
class CIntersectionsLineSegmentPVPlaneAAOrigin
{
#if WORKSHEET_4

#endif
};


/**
 * intersection between a sphere and a axis aligned box at origin
 */
template <typename T>
class CIntersectionsSphereBoxAAOrigin
{
#if WORKSHEET_4

#endif
};


/**
 * intersection between a sphere and a size limited axis aligned plane
 * with its center at origin
 */
template <typename T>
class CIntersectionsSpherePlaneAAOrigin
{
#if WORKSHEET_2

#endif
};

#endif
