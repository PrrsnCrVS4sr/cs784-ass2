#include <iostream>

#include <integrator.hpp>
#include <iostream>

using namespace std;

using namespace rt;



bool near_zero(Vector3d _v)
{
	double s = 1e-8;
    return (fabs(_v[0]) < s) && (fabs(_v[1]) < s) && (fabs(_v[2]) < s);
}


Vector3d random_in_unit_sphere() {
    while (true) {
        Vector3d p = random(-1,1);
        if (p.dot(p) < 1)
            return p;
    }
}

Vector3d random_unit_vector() {
    return random_in_unit_sphere().normalized();
}

Vector3d random_cosine()
{
    double U1 = random_double();
    double U2 = random_double();


    double X = cos(2*M_PI*U1)*sqrt(U2);
    double Y = sin(2*M_PI*U1)*sqrt(U2);
    double Z = sqrt(1-U2);

    return Vector3d(X, Y, Z);
}

Vector3d random_in_hemisphere(Vector3d _normal)
{
	 
	Vector3d s = (fabs(_normal.x()) > 0.9) ? Vector3d(0.0,1.0,0.0) : Vector3d(1.0,0.0,0.0);
	Vector3d v = _normal.cross(s);
	Vector3d u = _normal.cross(v);
	Vector3d random_cos = random_cosine();
	Vector3d random_cos_local =( random_cos[0]*u + random_cos[1]*v + random_cos[2]*_normal).normalized();
	return random_cos_local;

}

ray_t get_reflected_ray(ray_t& _ray,Vector3d normal,Vector3d hitpt)
{

	ray_t _rray;


	Vector3d direction = (_ray.direction - 2*(_ray.direction.dot(normal))*normal).normalized();
	
	_rray.origin = hitpt;
	_rray.direction = direction;


	return _rray;

}


ray_t get_transmitted_ray(ray_t& _ray, double eta, 
Vector3d normal ,Vector3d hitpt )
{
	
	ray_t _tray;

	// check normal to get whether the ray is inside to outside or outside to inside

	double dot_res = _ray.direction.dot(normal);
	
	bool inside_out = (dot_res > 0);

	if(!inside_out)
	{
		eta = 1.0/eta;
		
	}
	else
	{
		normal = -normal;
	}


	// check if TIR
	double cos_theta = fmin(-_ray.direction.normalized().dot(normal), 1.0);
	double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

	bool cannot_refract = eta * sin_theta > 1.0;
	Vector3d direction;

	if (cannot_refract)
		direction = (_ray.direction - 2*(_ray.direction.dot(normal))*normal).normalized();
	else
	{

		
		Vector3d r_out_perp =  -normal * sqrt(1-eta*eta*sin_theta*sin_theta);
		Vector3d r_out_parallel = (sin_theta/eta)*((_ray.direction.normalized() -normal).normalized());
		direction = (r_out_perp + r_out_parallel).normalized();
	}
		

	_tray.origin = hitpt;
	_tray.direction = direction;


	return _tray;

}


void sample_from_diffuse_brdf(ray_t &_dray, Vector3d _hitpt, Vector3d _normal)
{
	_dray.origin = _hitpt;
	_dray.direction = random_in_hemisphere(_normal);


}


void sample_from_specular_brdf(ray_t &_sray, ray_t _ray,Vector3d _hitpt, Vector3d _normal)
{
	_sray = get_reflected_ray(_ray, _normal, _hitpt);
	_sray.direction = _sray.direction + random_in_unit_sphere();

}


color_t whitted_integrator_t::radiance(const scene_t* _scn, ray_t& _ray, int d) const
{	

	// end of recursion
	if(d>depth) return color_t(0.0);
	
	bool found_intersection=false;
	std::vector<object_t*>::const_iterator oit;
	hit_t hit, minhit;
	material_t* material;
	
	Vector3d hitpt, normal;

	ray_t reflected_ray, transmitted_ray;
	

	

		

	for (oit=_scn->objs.begin(); oit!=_scn->objs.end(); oit++)
	{
		if ((*oit)->intersect(hit, _ray))
		{
		  _ray.maxt = hit.second;
		  minhit = hit;
		  
		  
		  hitpt = _ray.origin+_ray.maxt*_ray.direction;

		  if ((*oit)->get_name().compare("plane")==0)
		  {
		  	normal = (*oit)->get_normal(_ray.direction);
		  }
		  else if ((*oit)->get_name().compare("sphere")==0)
		  {
		  	normal = (*oit)->get_normal(hitpt);
		  }


		  found_intersection=true;

		}
		
	}
	
	color_t d_col(0.0);

	if(found_intersection)
	{	
		
		// cout<<"here"<<endl;
		material = minhit.first->get_material();
		
		

		std::list<light_t*>::const_iterator lit;
		// if(!material->get_is_transmit())
		for(lit=_scn->lits.begin(); lit!=_scn->lits.end(); lit++)
		{
			d_col += (*lit)->direct(hitpt, normal, material, _scn);

			
		}
		
		if(material->get_is_transmit())
		{	
			// cout<<"here2"<<endl;
			transmitted_ray = get_transmitted_ray(_ray,material->get_eta(),normal,hitpt);
			d_col += (radiance(_scn,transmitted_ray,d+1).array()) * (material->get_transmit().array());
		}


		

		if(material->get_is_reflect())
		{	
			// cout<<"here3"<<endl;
			reflected_ray = get_reflected_ray(_ray,normal,hitpt) ;
			d_col += (radiance(_scn,reflected_ray,d+1).array())*(material->get_reflect().array());
		}
		
		
		
		
	}
	else d_col = _scn->img->get_bgcolor();
	// cout<<d_col<<endl;
	return d_col;
}


color_t montecarlo_integrator_t::radiance(const scene_t* _scn, ray_t& _ray, int d) const
{	

	if(d >= depth) return color_t(0,0,0);
	
	bool found_intersection=false;
	bool found_light= false;
	std::vector<object_t*>::const_iterator oit;
	std::list<light_t*>::const_iterator lit;

	hit_t hit, minhit;
	material_t* material;
	light_t* current_light = NULL;
	Vector3d hitpt, normal;

	ray_t reflected_ray, transmitted_ray, diffuse_ray, specular_ray;
	color_t d_col(0.0);
	color_t e_col(0.0);

	
	

	for (oit=_scn->objs.begin(); oit!=_scn->objs.end(); oit++)
	{
		if ((*oit)->intersect(hit, _ray))
		{
		  _ray.maxt = hit.second;
		  minhit = hit;
		  
		  
		  hitpt = _ray.origin+_ray.maxt*_ray.direction;

		  if ((*oit)->get_name().compare("plane")==0)
		  {
		  	normal = (*oit)->get_normal(_ray.direction);
			// found_light = false;
		  }
		  else if ((*oit)->get_name().compare("sphere")==0)
		  {
		  	normal = (*oit)->get_normal(hitpt);
			// found_light = false;
		  }
		  else if((*oit)->get_name().compare("disk")==0)
		  {
			normal = (*oit)->get_normal(hitpt);
			// if(normal.dot(_ray.direction) < 0)
			// 	e_col = (*oit)->get_color().array();
			// return color_t(0.0);
			// found_light = true;
			// continue;
		  }

		  if((*oit)->get_area_light() != NULL)
		  {	

			current_light = (*oit)->get_area_light();
			found_light = true;
		  }
		  else
		  {
			found_light = false;
		  }


		  found_intersection=true;

		}
		
	}
	if(found_light)
	{
		
		e_col = current_light->direct(hitpt, normal, material, _scn);
		return e_col;
	}
	if(found_intersection)
	{	
		

		material = minhit.first->get_material();

		
		if(material->get_is_transmit())
		{	
			transmitted_ray = get_transmitted_ray(_ray,material->get_eta(),normal,hitpt);
			d_col += (radiance(_scn,transmitted_ray,d+1).array()) * (material->get_transmit().array());
		}

		if(material->get_is_reflect())
		{	
			reflected_ray = get_reflected_ray(_ray,normal,hitpt) ;
			d_col += (radiance(_scn,reflected_ray,d+1).array())*(material->get_reflect().array());
		}
		
		


		// find area_light and compute shadows
		double shadow = 0;
		// int num_samples = 1;
		// for (lit=_scn->lits.begin(); lit!=_scn->lits.end(); lit++)
		// {

		// 	shadow = (*lit)->compute_shadows(num_samples,hitpt,_scn)/num_samples;
			
		// }

		color_t roughness_contrib(0.0);

		sample_from_diffuse_brdf(diffuse_ray, hitpt, normal);
		sample_from_specular_brdf(specular_ray, _ray,hitpt, normal);
     
		color_t diffuse_brdf = (material->get_diffuse().array());
		color_t specular_brdf = (material->get_specular().array());

		roughness_contrib += (radiance(_scn,diffuse_ray,d+1).array()) * (diffuse_brdf.array());

		if(!material->get_is_reflect()) //not a mirror
		{
			roughness_contrib += (radiance(_scn,specular_ray,d+1).array()) * (specular_brdf.array());
		}
		
		d_col  += (1-shadow)*roughness_contrib;

		
		
	}
	else d_col = _scn->img->get_bgcolor();
	// cout<<d_col<<endl;
	return d_col;
}
