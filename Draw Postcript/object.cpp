// $Id: object.cpp,v 1.1 2013-08-04 15:26:21-07 - - $
/*This program was completed using pair programming.
Partner:  Andrew Lien (ahlien@ucsc.edu)
Partner:  Daniel Jou (djou@ucsc.edu)
CMPS 109
Assignment 3
*/

#include <typeinfo>

using namespace std;

#include "object.h"
#include "util.h"

#define NEW_TRACE(OBJ) TRACE ('c', "new: " << endl << *this)

object::object() {
   NEW_TRACE (*this);
}

text::text (const string &font, const points &size, const string &data):
      fontname(font), fontsize(size), textdata(data) {
   NEW_TRACE (*this);
}

shape::shape (const points &thick): thick(thick) {
   NEW_TRACE (*this);
}

ellipse::ellipse (const inches &width, const inches &height,
                  const points &thick):
      shape(thick), dimension ({width, height}) {
   NEW_TRACE (*this);
  
}

circle::circle (const inches &diameter, const points &thick):
      ellipse (diameter, diameter, thick) {
   NEW_TRACE (*this);
}

polygon::polygon (const coordlist &coords, const points &thick):
      shape(thick), coordinates(coords) {
   NEW_TRACE (*this);
}

rectangle::rectangle (const inches &width, const inches &height,
                      const points &thick):
      polygon (make_list(height,width),thick) {
   NEW_TRACE (*this);
}

square::square (const inches &width, const points &thick):
      rectangle (width, width, thick) {
   NEW_TRACE (*this);
}

line::line (const inches &length, const points &thick):
      polygon (coordlist ({{length, 0}}), thick) {
   NEW_TRACE (*this);
}


#define DELETE_TRACE(OBJ) TRACE ('c', "delete: " << endl << *this)
object   ::~object()    { DELETE_TRACE (*this); }
text     ::~text()      { DELETE_TRACE (*this); }
shape    ::~shape()     { DELETE_TRACE (*this); }
ellipse  ::~ellipse()   { DELETE_TRACE (*this); }
circle   ::~circle()    { DELETE_TRACE (*this); }
polygon  ::~polygon()   { DELETE_TRACE (*this); }
rectangle::~rectangle() { DELETE_TRACE (*this); }
square   ::~square()    { DELETE_TRACE (*this); }
line     ::~line()      { DELETE_TRACE (*this); }

#define DRAW_TRACE(COORDS,ANGLE,OBJ) \
        TRACE ('d', "draw: " << COORDS << " " << ANGLE << endl << OBJ)

void text::draw (ostream &out, const xycoords &coords,
                 const degrees &angle) {
   DRAW_TRACE (coords, angle, *this);
 //  out << "Generate Postscript code here." << endl;
   string indent = "   ";
   
   out<<"gsave" <<endl
       << indent << "/" << fontname << " findfont" << endl
       << indent << (double)fontsize << " scalefont setfont" <<endl
       << indent << (double)(coords.first) << " "
                 << (double)(coords.second) << " translate" << endl
       << indent << (double)angle <<" rotate"<<endl
       << indent << "0 0 moveto" << endl
       << indent << "(" << textdata << ")" << endl
       << indent << "show" << endl
       << "grestore" << endl;         
}

void ellipse::draw (ostream &out, const xycoords &coords,
                    const degrees &angle) {
   DRAW_TRACE (coords, angle, *this);
   //he changed it so now height and width are one variable stored
   //together as a pair. which is dimension.second and dimension.first
   inches height = dimension.second;
   inches width = dimension.first;  
   
   /*test input
   cout<< "The height is " << height <<endl;
   cout<< "The width is " << width << endl;*/
   
  // out << "Generate Postscript code here." << endl;
   double hght = height;
   double wdth = width;
   double x = coords.first;
   double y = coords.second;
   double ang = angle;
   double xscale, yscale, radius;
   double thck = thick;
   
   if(hght < wdth) {
      xscale = 1;
      yscale = hght / wdth;
      radius = wdth / 2;
   } else {
      xscale = wdth / hght;
      yscale = 1;
      radius = hght / 2;
   }
   
   out << "gsave\n   newpath\n   "
         << "/save matrix currentmatrix def\n   "
         << x << " " << y << " translate\n   "
         << ang << " rotate\n   "
         << yscale << " " << setprecision(15) <<xscale << " scale\n   "
         << "0 0 " << radius << " 0 360 arc\n   "
         << "save setmatrix\n   "
         << thck << " setlinewidth\n   "
         << "stroke\ngrestore\n";
   
}

void polygon::draw (ostream &out, const xycoords &coords,
                    const degrees &angle) {
   DRAW_TRACE (coords, angle, *this);

   string indent = "   ";

   out << "gsave" << endl
       << indent << "newpath" << endl
       << indent << double (coords.first) << " "
                 << double (coords.second) << " translate" << endl
       << indent << double (angle) << " rotate" << endl
       << indent << "0 0 moveto" << endl;

   coordlist::const_iterator itor = coordinates.begin ();
   for (; itor != coordinates.end (); ++itor) {
      out << indent << double (itor->second) << " "
                    << double (itor->first) << " rlineto" << endl;
   }

   out << indent << "closepath" << endl
       << indent << double (thick) << " setlinewidth" << endl
       << indent << "stroke" << endl
       << "grestore" << endl;

}


void object::show (ostream &out) const {
   out << this << "->" << demangle(*this) << ": ";
}

void text::show (ostream &out) const {
   object::show (out);
   out << fontname << "[" << fontsize << "] \"" << textdata << "\"";
}

void shape::show (ostream &out) const {
   object::show (out);
   out << thick << ": ";
}

void ellipse::show (ostream &out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream &out) const {
   shape::show (out);
   out << "{" << coordinates << "}";
}
coordlist rectangle::make_list (
            const inches &height, const inches &width) {
   coordlist coordlist;
   inches zero = inches(0);
   coordlist.push_back(xycoords(zero, height));
   coordlist.push_back(xycoords(width, zero));
   coordlist.push_back(xycoords(zero, -height));
   return coordlist;
}

coordlist line::make_list (const inches &length) {
   coordlist coordlist;
   coordlist.push_back(xycoords(length, inches(0)));
   return coordlist;
}
ostream &operator<< (ostream &out, const object &obj) {
   obj.show (out);
   return out;
}

