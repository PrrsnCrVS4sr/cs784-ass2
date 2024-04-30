
#include <plane.hpp>
#include <iostream>
using namespace std;

using namespace rt;

plane_t::plane_t(material_t* _mat):Q(0.0,0.0,0.0),u(1.0, 0.0, 0.0), v(0.0,1.0,0.0),mat(_mat) 
{ 
	normal = u.cross(v).normalized();
	D = normal.dot(Q);
	area_light = NULL;
}
plane_t::plane_t(material_t* _mat, Eigen::Vector3d _Q, Eigen::Vector3d _u, Eigen::Vector3d _v): Q(_Q), u(_u), v(_v), mat(_mat) 
{ 
	normal = u.cross(v).normalized();
	D = normal.dot(Q);
	area_light = NULL;
}
plane_t::~plane_t() { }

bool plane_t::intersect(hit_t& result, const ray_t& _ray) const
{	
	
	double denom = normal.dot(_ray.direction);
	

	
	// No hit if the ray is parallel to the plane.
	if (fabs(denom) < 1e-4)
	{	
	
		return false;
	}
		


	double t = (D - normal.dot(_ray.origin)) / denom;

	if(!(t >= _ray.mint && t <= _ray.maxt))
		return false;


	Eigen::Vector3d intersection = (_ray.origin + t*_ray.direction);
	Eigen::Vector3d planar_hitpt_vector = intersection - Q;



	double a = planar_hitpt_vector.dot(u)/u.norm();
	double b = planar_hitpt_vector.dot(v)/v.norm();

	if(a < 0 || b < 0 || a > u.norm() || b > v.norm()) return false;

	
	
		

	result = hit_t(this, t);
	
	
	return true;

}

Eigen::Vector3d plane_t::get_normal(Eigen::Vector3d& _dir) const
{
	double dot = normal.dot(_dir);
	
	if (dot > 0)
		return -normal;
	else
		return normal;
}

material_t* plane_t::get_material(void) const
{
	return mat;
}

void plane_t::print(std::ostream &stream) const
{
	Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[ ", " ]");
	
	stream<<"Object Properties: -------------------------------"<<std::endl;
	stream<<"Type: Plane"<<std::endl;
	stream<<"Center: "<<Q.format(CommaInitFmt)<<std::endl;
	stream<<"Length: "<<u.norm()<<std::endl<<std::endl;
	stream<<"Breadth: "<<v.norm()<<std::endl<<std::endl;
	stream<<"Normal: "<<normal.format(CommaInitFmt)<<std::endl<<std::endl;
}

color_t plane_t::get_color(void) const
{
	return color_t(0.0,0.0,0.0);
}


light_t* plane_t::get_area_light(void) const
{
	return area_light;
}


Vector3d plane_t::get_samples_on_surface(void) const
{
	double a = random_double();
	double b = random_double();
	return a*u +b*v + Q;
	
}

void plane_t::set_area_light(light_t* light) 
{
	area_light = light;
}