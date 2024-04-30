#include <sphere.hpp>

using namespace rt;

sphere_t::sphere_t(material_t* _mat):center(0.0,0.0,0.0),radius(1.0),mat(_mat) { area_light = NULL;}
sphere_t::sphere_t(material_t* _mat, Eigen::Vector3d _c, double _r): center(_c), radius(_r), mat(_mat) {area_light = NULL; }

sphere_t::~sphere_t() { }

bool sphere_t::intersect(hit_t& result, const ray_t& _ray) const
{
	Vector3d r2c = center - _ray.origin;
	const double b = r2c.dot(_ray.direction);
	double d = b*b - r2c.dot(r2c) + radius*radius;

	if (d < 0)
		return false;
	else
		d = sqrt(d);

	double t;

	t=b-d;
	if (!is_zero(t))
	{
		result = hit_t(this, t);
	}
	else 
	{
		t = b+d;
		if (!is_zero(t)) 
			result = hit_t(this,t);
		else return false;
	}

	return t >= _ray.mint && t <= _ray.maxt;

}

Eigen::Vector3d sphere_t::get_normal(Eigen::Vector3d& _p) const
{
	Eigen::Vector3d normal = _p - center;
	normal.normalize();

	return normal;
}

material_t* sphere_t::get_material(void) const
{
	return mat;
}

void sphere_t::print(std::ostream &stream) const
{
	Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[ ", " ]");
	
	stream<<"Object Properties: -------------------------------"<<std::endl;
	stream<<"Type: Sphere"<<std::endl;
	stream<<"Center: "<<center.format(CommaInitFmt)<<std::endl;
	stream<<"Radius: "<<radius<<std::endl<<std::endl;
}

color_t sphere_t::get_color(void) const
{
	return color_t(0.0,0.0,0.0);
}


light_t* sphere_t::get_area_light(void) const
{
	return area_light;
}


Vector3d sphere_t::get_samples_on_surface(void) const
{
	double theta = 2 * M_PI * random_double();
	double phi = acos(1 - 2 * random_double());
	double x = sin(phi) * cos(theta);
	double y = sin(phi) * sin(theta);
	double z = cos(phi);

	return Vector3d(x,y,z);
}

void sphere_t::set_area_light(light_t* light)
{
	area_light = light;
}
