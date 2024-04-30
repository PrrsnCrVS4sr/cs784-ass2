
/*
    This file is part of rt.

    rt is a simple ray tracer meant to be used for teaching ray tracing.

    Copyright (c) 2018 by Parag Chaudhuri

    Some parts of rt are derived from Nori by Wenzel Jacob.
    https://github.com/wjakob/nori/

    rt is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    rt is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <material.hpp>
#include <object.hpp>
#include <ray.hpp>
#include <utils.hpp>
namespace rt
{
	/**
	 * \brief The sphere object class.
	 **/
	class disc_t : public object_t
	{
	private:

		/// quadrilateral corner
		Eigen::Vector3d Q;

		/// quadrilateral u and v vectors
		Eigen::Vector3d u;
		Eigen::Vector3d v;

		//radius
		double radius;

		//normal
		Eigen::Vector3d normal;

		/// distance from origin
		double D;

		/// Plane material
		material_t* mat;

		color_t color;

		light_t* area_light;

	public:
		/// Constructor
		disc_t(material_t* _mat);
		/// Constructor
		disc_t(material_t* _mat, Eigen::Vector3d _Q, Eigen::Vector3d _u, Eigen::Vector3d _v, double _r);
		/// Destuctor
		virtual ~disc_t();

		/// Returns the mandatory object name
		std::string get_name(void) const { return std::string("disc"); }

		/**
		* Returns true if the _ray hits this object. The hit information is returned in result. 
		* This is not valid if there is no intersection and the function returns false.
		**/
		bool intersect(hit_t& result, const ray_t& _ray) const;

		/// Returns the normal to the surface at point _p.
		Eigen::Vector3d get_normal(Eigen::Vector3d& _p) const;

		/// Returns the material for the sphere.
		material_t* get_material(void) const;

		

		virtual color_t get_color(void) const;

		light_t* get_area_light(void) const;

		void set_area_light(light_t* light);

		/// Prints information about the sphere. to stream.
		virtual void print(std::ostream &stream) const;

		Vector3d get_samples_on_surface(void) const;
	};
}
