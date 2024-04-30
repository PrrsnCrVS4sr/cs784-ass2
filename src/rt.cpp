#include <resolver.hpp>

#include <rt.hpp>


using namespace rt;

void rt::render(const scene_t* scn)
{
	unsigned int w=scn->img->get_width();
	unsigned int h=scn->img->get_height();

	// std::list<light_t*>::const_iterator lit;
	// for(lit= scn->lits.begin(); lit!= scn->lits.end(); lit++)
	// {	

	// 	object_t* obj = (*lit)->get_object();
	// 	if(obj != NULL)
	// 		scn->objs.push_back(obj);	
	// }

	for (unsigned int i=0; i<w; i++)
	{	
		//std::clog << "\rScanlines remaining: " << (w - i) << ' ' << std::flush;
		for (unsigned int j=0; j<h; j++)
		{	
			ray_t ray;
			int d=0;
			color_t aggr_col(0.0);

			for(int k = 0; k < scn->img->num_samples; k++)
			{
				Eigen::Vector2f psample=scn->img->sample_pixel(scn->img->num_samples==1?i:i + rand()%(3)-1,
				scn->img->num_samples==1?j:j+ rand()%(3)-1);
				color_t col = scn->cam->sample_ray(ray, psample);
				col *= scn->intg->radiance(scn, ray, d);
				aggr_col += col;
			}
			
			
			// scn->cam->print(std::cout);
			

			scn->img->set_pixel(i,j,aggr_col);
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2) 
		{
        	std::cerr << "Syntax: " << argv[0] << " <scene.xml>" << std::endl;
        	return -1;
		}

	filesystem::path path(argv[1]);

	try
	{
		if (path.extension() == "xml") 
		{
			std::string scene_filename(argv[1]);
  			rt::scene_t scn(scene_filename);

  			rt::render(&scn);

			std::string img_filename = scene_filename;
    		size_t lastdot = img_filename.find_last_of(".");
    		if (lastdot != std::string::npos)
     			img_filename.erase(lastdot, std::string::npos);
   			img_filename += ".ppm";
  	
  			scn.img->write(img_filename);
  		}
  		else
  		{
  			std::cerr<<"Error: Unknown file type."<<std::endl;
  			return -1;
  		}
  } 
  catch (const std::exception &e)
  {
  	std::cerr<<"Error: "<<e.what()<<std::endl;
  	return -1;
  }
  	
  return 0;
}
