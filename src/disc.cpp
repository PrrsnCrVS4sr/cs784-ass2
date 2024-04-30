

#include <disc.hpp>
#include <iostream>
using namespace std;

using namespace rt;

disc_t::disc_t(material_t* _mat):Q(0.0,0.0,0.0),u(1.0, 0.0, 0.0), v(0.0,1.0,0.0),mat(_mat) 
{ 
	normal = u.cross(v).normalized();
	D = normal.dot(Q);
	area_light = NULL;
}
disc_t::disc_t(material_t* _mat, Eigen::Vector3d _Q, Eigen::Vector3d _u, Eigen::Vector3d _v, double _r): Q(_Q), u(_u), v(_v), mat(_mat), radius(_r)
{ 	

	normal = u.cross(v).normalized();
	v = normal.cross(u).normalized();
	u = u.normalized();
	D = normal.dot(Q);
	area_light = NULL;
}
disc_t::~disc_t() { }

bool disc_t::intersect(hit_t& result, const ray_t& _ray) const
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

	double dist = (Q-intersection).norm();

	if(dist > radius)	return false;	

	result = hit_t(this, t);
	
	
	return true;

}

Eigen::Vector3d disc_t::get_normal(Eigen::Vector3d& _dir) const
{
	double dot = normal.dot(_dir);
	
	if (dot > 0)
		return -normal;
	else
		return normal;
}

material_t* disc_t::get_material(void) const
{
	return mat;
}




light_t* disc_t::get_area_light(void) const
{
	return area_light;
}

void disc_t::print(std::ostream &stream) const
{
	Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[ ", " ]");
	
	stream<<"Object Properties: -------------------------------"<<std::endl;
	stream<<"Type: Disc"<<std::endl;
	stream<<"Center: "<<Q.format(CommaInitFmt)<<std::endl;
	stream<<"Radius: "<<radius<<std::endl;
	stream<<"Normal: "<<normal.format(CommaInitFmt)<<std::endl<<std::endl;
}

color_t disc_t::get_color(void) const
{
	return color_t(0.0,0.0,0.0);
}


Vector3d disc_t::get_samples_on_surface(void) const
{
	double U1 = ((double)rand())/RAND_MAX;
	double U2 = ((double)rand())/RAND_MAX;
	return (radius*sqrt(U1)*cos(2*M_PI*U2))*u + (radius*sqrt(U1)*sin(2*M_PI*U2))*v + Q;
}


void disc_t::set_area_light(light_t* light) 
{
	area_light = light;
}
