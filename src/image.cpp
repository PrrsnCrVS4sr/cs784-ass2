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

#include <image.hpp>
#include <iostream>
using namespace std;
using namespace rt;

image_t::image_t(int _w, int _h, color_t _bgc,int _nsamples):width(_w),height(_h),bgcolor(_bgc),num_samples(_nsamples)
{
	aspect = double(width)/double(height);
	data = new char[width*height*3]; 
}

image_t::~image_t()
{ 
	delete data;
}

int image_t::get_width(void) const {return width; }
int image_t::get_height(void) const {return height; }
double image_t::get_aspect(void) const {return aspect; }

color_t image_t::get_bgcolor(void) const {return bgcolor; }

Eigen::Vector2f image_t::sample_pixel(unsigned int _x, unsigned int _y) const
{
	return Eigen::Vector2f(double(_x)/width, double(_y)/height);
}

color_t image_t::get_pixel(unsigned int _x, unsigned int _y) const
{
	int pos=(_y)*width*3+(_x)*3;
	double r=double(data[pos])/255.0;
	double g=double(data[pos+1])/255.0;
	double b=double(data[pos+2])/255.0;
	
	return color_t(r,g,b);
}

void image_t::set_pixel(unsigned int _x, unsigned int _y, color_t _col)
{
	int pos=(_y)*width*3+(_x)*3;
	char r = to_char((double)_col.r()/num_samples);
	char g = to_char((double)_col.g()/num_samples);
	char b = to_char((double)_col.b()/num_samples);

	data[pos]=r; data[pos+1]=g; data[pos+2]=b;
}

void image_t::write(std::string filename)
{
	std::ofstream out(filename.c_str(), std::ios::binary|std::ios::out);
	out<<"P6"<<std::endl<<width<<" "<<height<<" "<<255<<std::endl;
	out.write((const char*)data,width*height*3);
	out.close();
}

void image_t::print(std::ostream &stream)
{
	Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[ ", " ]");
	
	stream<<"Image Properties: -------------------------------"<<std::endl;
	stream<<"BG Color: "<<bgcolor.format(CommaInitFmt)<<std::endl;
	stream<<"Width: "<<width<<std::endl;
	stream<<"Height:"<<height<<std::endl;
	stream<<"aspect:"<<aspect<<std::endl<<std::endl;
}