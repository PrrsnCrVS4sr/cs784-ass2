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

#include <color.hpp>
#include <material.hpp>
#include <scene.hpp>
#include <utils.hpp>

namespace rt
{
	/// Forward Declaration.
	class scene_t;
	class object_t;

	/**
	 * \brief  This is the abstract base class for lights.
	 * The light is responsible for returning its direct illumination estimate to a point.
	 **/
	class light_t
	{
	private:

	public:

		/// Constructor
		light_t();

		/// Destructon
		virtual ~light_t();

		/** 
		* Returns the direct illumination estimate for the point hitpt, where the surface normal is normal, material is mat.
		* Scene is passed so that the camera position, and the objects can be used for computing the specular component
		* of direct illumination and the shadow computations.
		**/ 
		virtual color_t direct(const Vector3d& hitpt, const Vector3d& normal, const material_t* mat, const scene_t* scn) const = 0;

		/// Prints information about the light to the stream.
		virtual void print(std::ostream &stream) const = 0;


		virtual object_t* get_object() const = 0;

		virtual double compute_shadows(int num_samples, Vector3d _hitpt, const scene_t* _scn) const = 0;
	};

	/** 
	 * \brief This is the class for a positional point light source.
	 **/
	class point_light_t : public light_t
	{
	private:
		/// Position of the light
		Vector3d pos;
		/// Color of the light. This can be thought of as radiance emitted by the light source.
		Vector3d col;
		/// An ambient coefficient. Modulate col with ka to get ambient component of illumination.
		double ka;

	public:
		/// Constructor
		point_light_t(const Vector3d& _pos, const Vector3d& _col, const double _ka);
		/// Destructor
		virtual ~point_light_t();

		/** 
		* Returns the direct illumination estimate for the point hitpt, where the surface normal is normal, material is mat.
		* Scene is passed so that the camera position, and the objects can be used for computing the specular component
		* of direct illumination and the shadow computations.
		**/ 
		virtual color_t direct(const Vector3d& hitpt, const Vector3d& normal, const material_t* mat, const scene_t* scn) const;

		/// Prints information about the light to the stream.
		virtual void print(std::ostream &stream) const;
		object_t* get_object() const;

		double compute_shadows(int num_samples, Vector3d _hitpt, const scene_t* _scn) const;
	};




	class area_light_t : public light_t
	{
	private:
		/// Position of the light
		object_t* obj;
		/// Color of the light. This can be thought of as radiance emitted by the light source.
		Vector3d col;
	

	public:
		/// Constructor
		area_light_t(object_t* object, const Vector3d& _col);
		/// Destructor
		virtual ~area_light_t();

		/** 
		* Returns the direct illumination estimate for the point hitpt, where the surface normal is normal, material is mat.
		* Scene is passed so that the camera position, and the objects can be used for computing the specular component
		* of direct illumination and the shadow computations.
		**/ 
		virtual color_t direct(const Vector3d& hitpt, const Vector3d& normal, const material_t* mat, const scene_t* scn) const;


		double compute_shadows(int num_samples,  Vector3d _hitpt, const scene_t* _scn) const;

		object_t* get_object() const;

		/// Prints information about the light to the stream.
		virtual void print(std::ostream &stream) const;
	};
}
