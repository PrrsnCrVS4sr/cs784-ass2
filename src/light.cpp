#include <light.hpp>
#include <iostream>


using namespace rt;

light_t::light_t() { }
light_t::~light_t() { }


point_light_t::point_light_t(const Vector3d& _pos, const Vector3d& _col, const double _ka): pos(_pos), col(_col), ka(_ka) 
{ }

point_light_t::~point_light_t()
{ }


double point_light_t::compute_shadows(int num_samples,Vector3d _hitpt, const scene_t* _scn) const
{
	
	std::vector<object_t*>::const_iterator oit;
	hit_t hit;
	//take num_samples samples on the disk
	// u and v are orrthonormal basis
	// use rejection sampling if num_samples is high
	double shadow_count = 0.0;
	while(num_samples--)
	{

		Vector3d light_point = this->pos;

		Vector3d direction = (light_point - _hitpt).normalized();
		ray_t _shadowray(_hitpt, direction);

		bool intersect = false;
		double light_htpt_distance = -100;
		double object_htpt_distance = 0;

		
		//change here
		for (oit=_scn->objs.begin(); oit!=_scn->objs.end(); oit++)
		{
			if ((*oit)->intersect(hit, _shadowray))
			{	
				_shadowray.maxt = hit.second;
				Vector3d obj_hitpt = _shadowray.origin+_shadowray.maxt*_shadowray.direction;

				light_htpt_distance = (_hitpt - light_point).norm();
				object_htpt_distance = (_hitpt - obj_hitpt).norm();
				intersect = true;
			}	
		}

		if(intersect && light_htpt_distance > object_htpt_distance)
		{	
			// return 1.0;
			shadow_count += 1;
			// break;

		}

	}
	return shadow_count;
	

}

color_t point_light_t::direct(const Vector3d& hitpt, const Vector3d& normal, const material_t* mat, const scene_t* scn) const
{	

	std::vector<object_t*>::const_iterator oit;

	Vector3d incident_ray = -(this->pos - hitpt).normalized();
	ray_t _ray(hitpt, -incident_ray);
	hit_t hit;
	color_t ambient_comp = this->ka*(this->col.array());
	

	color_t diffuse_comp = (mat->get_diffuse()*(fmax(-normal.dot(incident_ray),0.0))).array() * this->col.array();

	Vector3d reflected_comp = (incident_ray - 2*(incident_ray.dot(normal))*normal).normalized();
	Vector3d view = (scn->cam->get_eye() - hitpt).normalized();
	color_t specular_comp = (mat->get_specular()*(pow(fmax(view.dot(reflected_comp),0.0)
	, mat->get_shininess()))).array() * this->col.array();

	double shadow = compute_shadows(1,hitpt,scn);
	

	return (1-shadow)*(diffuse_comp + specular_comp) + ambient_comp;
}


object_t* point_light_t::get_object() const
{
	return NULL;
}


void point_light_t::print(std::ostream &stream) const
{
	Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[ ", " ]");

	stream<<"Light Properties: -------------------------------"<<std::endl;
	stream<<"Type: Point Light"<<std::endl;
	stream<<"Position: "<<pos.format(CommaInitFmt)<<std::endl;
	stream<<"Color: "<<col.format(CommaInitFmt)<<std::endl;
	stream<<"Ambient Coefficient: "<<ka<<std::endl<<std::endl;
}




area_light_t::area_light_t(object_t* _obj, const Vector3d& _col): obj(_obj), col(_col)
{ 
	obj->set_area_light(this);
}

area_light_t::~area_light_t()
{ }

color_t area_light_t::direct(const Vector3d& hitpt, const Vector3d& normal, const material_t* mat, const scene_t* scn) const
{	

	return col.array();
}

double area_light_t::compute_shadows(int num_samples, Vector3d _hitpt, const scene_t* _scn) const
{
	std::vector<object_t*>::const_iterator oit;
	hit_t hit;
	//take num_samples samples on the disk
	// u and v are orrthonormal basis
	// use rejection sampling if num_samples is high
	double shadow_count = 0.0;
	while(num_samples--)
	{

		Vector3d random_point = obj->get_samples_on_surface();

		Vector3d direction = (random_point - _hitpt).normalized();
		ray_t _shadowray(_hitpt, direction);

		bool intersect = false;
		double light_htpt_distance = -100;
		double object_htpt_distance = 0;

		
		object_t* obstruction;
		//change here
		for (oit=_scn->objs.begin(); oit!=_scn->objs.end(); oit++)
		{
			if ((*oit)->intersect(hit, _shadowray))
			{	
				if((*oit)->get_area_light()!=NULL)
					break;
				_shadowray.maxt = hit.second;
				Vector3d obj_hitpt = _shadowray.origin+_shadowray.maxt*_shadowray.direction;

				light_htpt_distance = (_hitpt - random_point).norm();
				object_htpt_distance = (_hitpt - obj_hitpt).norm();
				obstruction = (*oit);
				intersect = true;
			}	
		}

		if(intersect && light_htpt_distance > object_htpt_distance)
		{	
			// return 1.0;
			color_t transmission = obstruction->get_material()->get_transmit();
			shadow_count += (1-fmax(fmax(transmission.r(),transmission.g()), transmission.b()));
			// break;

		}

	}
	return shadow_count;

	

}


object_t* area_light_t::get_object() const
{
	return obj;
}


void area_light_t::print(std::ostream &stream) const
{
	Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[ ", " ]");

	stream<<"Light Properties: -------------------------------"<<std::endl;
	stream<<"Type: Area Light"<<std::endl;
	obj->print(std::cout);
	stream<<"Color: "<<col.format(CommaInitFmt)<<std::endl;
}
