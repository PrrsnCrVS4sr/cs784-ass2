
#pragma once

#include <tinyxml2.hpp>

#include <camera.hpp>
#include <image.hpp>
#include <integrator.hpp>
#include <light.hpp>
#include <material.hpp>
#include <object.hpp>
#include <plane.hpp>
#include <sphere.hpp>
#include <disc.hpp>
#include <utils.hpp>

using namespace tinyxml2;

namespace rt
{
 /// Forward Declarations   
 class light_t;
 class integrator_t;
 class object_t;

 /**
  * \brief This is the scence class. It represents the entire scene that rt reads from the scene file.
  **/
 class scene_t
 {

   private:
    /// Name of the scene file
    std::string scenefile;
    /// The xml scene file. We use tinyxml2 to parse these file
    tinyxml2::XMLDocument scenexml;

    /// Parses an xml property with the name _property and returns the value for an attribute named _type.
    std::string parse_property(XMLElement* _elm, std::string _property, std::string _type);
    /// Parses an xml attribute named _type and returns its value. 
    std::string parse_parameter(XMLElement* _elm, std::string _type);
    /// Returns a pointed to a material called name from the material list. If not found, throws exception.
    material_t* find_material(std::string name, const std::list<material_t*> matlist);

    

    ///  Parse an int property.
    int parse_int(XMLElement* _elm, std::string property);
    // Returns the int property value.
    int parse_int(XMLElement* _elm);
    /// Converts a string value to int
    int parse_int(std::string _val);

    ///  Parse an double property.
    double parse_double(XMLElement* _elm, std::string property);
    // Returns the double property value.
    double parse_double(XMLElement* _elm);
    /// Converts a string value to double
    double parse_double(std::string _val);

    ///  Parse an bool property.
    bool parse_bool(XMLElement* _elm, std::string property);
    // Returns the bool property value.
    bool parse_bool(XMLElement* _elm);
    /// Converts a string value to bool
    bool parse_bool(std::string _val);

    /// Parse an angle property. An angle can have units deg or rad.
    double parse_angle(XMLElement* _elm, std::string _property);
    /// Returns the angle property value.
    double parse_angle(XMLElement* _elm);
    /// Converts a string angle value to double.
    double parse_angle(std::string _val);

    ///  Parse an color property.
    color_t parse_color(XMLElement* _elm, std::string _property);
    /// Returns the color property value.
    color_t parse_color(XMLElement* _elm);
    /// Converts a string color to color_t
    color_t parse_color(std::string _val);

    ///  Parse an vector3 property. We use Eigen::Vector3d as our vector.
    Eigen::Vector3d parse_vector3(XMLElement* _elm, std::string _property);
    // Returns the vector3 property value.
    Eigen::Vector3d parse_vector3(XMLElement* _elm);
    /// Converts a string value to Vector3d
    Eigen::Vector3d parse_vector3(std::string _val);



    /// The top level parser for the scenefile. 
    bool parse_scenefile(void);

    /// Parse a camera.
    camera_t* parse_camera(XMLElement* _elm);

    /// Parse objects. All objects in the scene must appear in one block. They are all added to a vector of objects.
    int parse_objects(XMLElement* _elm, const std::list<material_t*>& matlist);

    // Parse object
    object_t* parse_object(XMLElement* _elm, const std::list<material_t*>& matlist);

    /// Parse a sphere object.
    object_t* parse_object_sphere(XMLElement* _elm, const std::list<material_t*>& matlist);
    
    /// Parse a plane object
    object_t* parse_object_plane(XMLElement* _elm, const std::list<material_t*>& matlist);

	/// Parse an disc obkect
	object_t* parse_object_disc(XMLElement* _elm, const std::list<material_t*>& matlist);

    /// Parses materials. All materials in the scene must appear in one block. They are all added to a list of materials.
    int parse_materials(XMLElement* _elm);
    /// Parse a solid surface material.
    material_t* parse_simplemat(XMLElement* _elm);

    /// Parses lights. All the lights in the scene must appeat in one block. They are all added to a list of lights.
    int parse_lights(XMLElement* _elm, const std::list<material_t*>& matlist);
    /// Parse a point light.
    light_t* parse_pointlight(XMLElement* _elm);

    light_t* parse_arealight(XMLElement* _elm, const std::list<material_t*>& matlist);
    /// Parse the image.
    image_t* parse_image(XMLElement* _elm);

    /// Parse the integrator.
    integrator_t* parse_integrator(XMLElement* _elm);
    /// Parse a whitted integrator.
    integrator_t* parse_whitted_integrator(XMLElement *_elm);
	/// Parce a montecarlo integrator
	integrator_t* parse_montecarlo_integrator(XMLElement *_elm);

    ///Constructor. Does not do anything. This cannot be used, hence defined as private.
    scene_t() {;}


   public:
    /// The camera
    camera_t* cam;

    /// Vector of objects
    std::vector<object_t*> objs;

    /// List of materials
    std::list<material_t*> mats;

    /// List of lights
    std::list<light_t*> lits;

    /// The integrator
    integrator_t *intg;

    /// The image
    image_t* img;

    /// Number of objects, materials and lights in the scene.
    int numobjs, nummats, numlits;

    /// Constructor.
    scene_t(std::string _scenefile)
    {
     scenefile = _scenefile;
     parse_scenefile();
    }

    /// Destructor
    ~scene_t()
    {
        objs.clear();
        mats.clear();
        lits.clear();
 
        delete img;
    }

 };
}
