// $Id: interp.cpp,v 1.1 2013-08-04 15:26:19-07 - - $
/*This program was completed using pair programming.
Partner:  Andrew Lien (ahlien@ucsc.edu)
Partner:  Daniel Jou (djou@ucsc.edu)
CMPS 109
Assignment 3
*/

#include <list>
#include <map>
#include <memory>
#include <string>

using namespace std;

#include "interp.h"
#include "object.h"
#include "util.h"

interpreter::interp_map_t interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
   {"newpage", &interpreter::do_newpage},
   {"dump"   , &interpreter::do_dump   },
};

interpreter::factory_map_t interpreter::factory_map {
   {"text"     , &interpreter::make_text     },
   {"ellipse"  , &interpreter::make_ellipse  },
   {"circle"   , &interpreter::make_circle   },
   {"polygon"  , &interpreter::make_polygon  },
   {"rectangle", &interpreter::make_rectangle},
   {"square"   , &interpreter::make_square   },
   {"line"     , &interpreter::make_line     },
};

interpreter::interpreter(const string &filename, ostream &outfile):
   outfile(outfile),
   pagenr(1),
   objmap(),
   infilename(filename),
   page_xoffset (inches(0.25).point_value()),
   page_yoffset (inches(0.25).point_value())
{
   prolog();
   startpage();
}

interpreter::~interpreter() {
   endpage();
   epilog();
   if (traceflags::getflag ('i')) {
      parameters dump {"dump"};
      do_dump (dump);
   }
}

string shift (list<string> &words) {
   if (words.size() == 0) throw runtime_error ("syntax error");
   string front = words.front();
   words.pop_front();
   return front;
}

void interpreter::interpret (parameters &params) {
   TRACE ('i', params);
   string command = shift (params);
   interp_map_t::const_iterator itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   (this->*func) (params);
}


void interpreter::do_define (parameters &params) {
   TRACE ('i', params);
   string name = shift (params);
   objmap.insert (object_map::value_type (name, make_object (params)));
}

void interpreter::do_draw (parameters &params) {
   TRACE ('i', params);
   string name = shift (params);
   object_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      throw runtime_error (name + ": no such object");
   }
   degrees angle = degrees (0);
   if (params.size() == 3) {
      angle = degrees (from_string<double> (params.back()));
      params.pop_back();
   }
   if (params.size() != 2) throw runtime_error ("syntax error");
   xycoords coords (inches (from_string<double> (params.front())),
                    inches (from_string<double> (params.back())));
   itor->second->draw (outfile, coords, angle);
}

void interpreter::do_newpage (parameters &params) {
   if (params.size() != 0) throw runtime_error ("syntax error");
   endpage();
   ++pagenr;
   startpage();
}

void interpreter::do_dump (parameters &params) {
   traceflags::setflags ("*");
   TRACE ('*', params);
   for (object_map::const_iterator itor = objmap.cbegin();
        itor != objmap.cend(); ++itor) {
      cerr << itor->first << ": " << *itor->second << endl;
   }
}


void interpreter::prolog() {
   outfile << "%!PS-Adobe-3.0" << endl;
   outfile << "%%Creator: " << sys_info::get_execname() << endl;
   outfile << "%%CreationDate: " << datestring() << endl;
   outfile << "%%PageOrder: Ascend" << endl;
   outfile << "%%Orientation: Portrait" << endl;
   outfile << "%%SourceFile: " << infilename << endl;
   outfile << "%%EndComments" << endl;
}

void interpreter::startpage() {
   outfile << endl;
   outfile << "%%Page: " << pagenr << " " << pagenr << endl;
   outfile << page_xoffset << " " << page_yoffset
           << " translate" << endl;
   outfile << "/Courier findfont 10 scalefont setfont" << endl;
   outfile << "0 0 moveto (" << infilename << ":"
           << pagenr << ") show" << endl;

}

void interpreter::endpage() {
   outfile << endl;
   outfile << "showpage" << endl;
   outfile << "grestoreall" << endl;
}

void interpreter::epilog() {
   outfile << endl;
   outfile << "%%Trailer" << endl;
   outfile << "%%Pages: " << pagenr << endl;
   outfile << "%%EOF" << endl;

}


object_ptr interpreter::make_object (parameters &command) {
   TRACE ('f', command);
   string type = shift (command);
   factory_map_t::const_iterator itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": no such object");
   }
   factoryfn func = itor->second;
   return (this->*func) (command);
}

object_ptr interpreter::make_text (parameters &command) {
   TRACE ('f', command);
   if(command.size() < 2)
     throw runtime_error ("syntax error");
     
     //pop next string arg from cmd list, conver to doub
   string temp;
   string fontname = "";
   string textdata = "";
   points fontsize = points(12);
   size_t found;
   temp = shift(command);
   found = temp.find_first_not_of("0123456789.");
   if(found != string::npos) fontname = temp;
   else {
      fontsize = points(from_string<double>(temp));
      fontname = shift(command);
   }
   if(!command.empty()) textdata += shift(command);
   while(!command.empty()) textdata += " " + shift(command);
   vector<string> escstrs;
   escstrs.push_back("\\");
   escstrs.push_back("(");
   escstrs.push_back(")");
   vector<string>::const_iterator itor = escstrs.begin();
   vector<string>::const_iterator end = escstrs.end();
   // escape special character for PostScript
   for(; itor != end; ++itor) {
      while((found = textdata.find(*itor, found)) != string::npos) {
         textdata.replace(found, 1, "\\" + *itor);
         found += itor->size() + 1;
      }
   }
   
   return make_shared<text> (fontname, 
                      points(fontsize), string(textdata));
}

object_ptr interpreter::make_ellipse (parameters &command) {
   TRACE ('f', command);
//ellipses has width and height & has optional thickness argument
//so a couple ways to goa bout doing this is 
//either check the length, which is easiest
//and if its length 3, the optional parameter exists and if its not
//then you know it doesnt exist. so the first two numbers
// you assign them to
//width and height, parse them as doubles, then 
//the 3rd number if it does exist
//you conver that to thickness, and if it doesnt exist, 
//you set thickness to default thickness. 
//once you have those three, you just do return new
// ellipse and pass in the three arguments to constructor
 // cout<<"I'm calling make_ellipse" << endl;
  if(command.size() < 2 || command.size() >3) 
    throw runtime_error("syntax error");
    
    //pop front two strings from command list, conver to double
    double height, width;
    try{
      string temp1 = shift(command);
      string temp2 = shift(command);
      height = from_string<double> (temp1);
      width = from_string<double> (temp2);
      /*//test input
         cout<< "The height is " << height <<endl;
   cout<< "The width is " << width << endl;
   cout<< "End test input of make_ellipse "<<endl;*/
   
      }catch(domain_error error){
         throw runtime_error("syntax error");
      }
      double thick;
      if(command.empty()) {thick = 2.0; }
        else{
          try{
            string temp = shift(command);
            thick = from_string<double>(temp);
          }catch(domain_error error){
            throw runtime_error("syntax error");
          }  
      }
  return make_shared<ellipse> (inches(height), 
            inches(width), points(thick));
 //  return new ellipse(height, width, thickness);
}

object_ptr interpreter::make_circle (parameters &command) {
   TRACE ('f', command);
   if(command.empty() || command.size() >2)
     throw runtime_error("syntax error");
     
     //pop diameter from cmd list
     double diameter;
     try{
       string temp = shift(command);
       diameter = from_string<double> (temp);
     }catch(domain_error error){
       throw runtime_error ("syntax error");
     } 
     
     //check if cmd ist is empty ,set thick accordingly
     double thick;
     if(command.empty()) { thick = 2.0; }
     else{
       try{
         string temp = shift(command);
         thick = from_string<double> (temp);
       }catch(domain_error error){
         throw runtime_error("syntax error");
       }
     } /* 
     cout<<"My inches(diameter) is " << inches(diameter) << endl
         <<" My points(thick) is " << points(thick) << endl;*/
   return make_shared<circle> (inches(diameter), points(thick));
}

object_ptr interpreter::make_polygon (parameters &command) {
   TRACE ('f', command);
   coordlist coords;
   double thick;
  // int i = 0;
   while(!command.empty()){
     if(command.size()>=2){
       try{
         string tempx = shift(command);
         string tempy = shift(command);
         
        /* //test input
         cout<< "Tempx is " << tempx << endl;
         cout<< "Tempy is " << tempy << endl;
         cout<< "While loop count is " << i << endl;
         ++i;*/
         double x = from_string<double>(tempx);
         double y = from_string<double>(tempy);
         coords.push_back(xycoords(inches(y),inches(x)));
     }catch(domain_error error){
       throw runtime_error("syntax error");
     } 
   }else if(command.size()==1) {
      try{
        string temp = shift(command);
        thick = from_string<double>(temp);  
      }catch(domain_error error){
         throw runtime_error("syntax error");
      }
    }else{ thick = 2.0; }
   }
   return make_shared<polygon> (coordlist(coords), points(thick));
}//makeshared?


object_ptr interpreter::make_rectangle (parameters &command) {
   TRACE ('f', command);
 if (command.size () < 2 || command.size () > 3)
      throw runtime_error ("syntax error");

   // Pop the first two string arguments from the command list.  Try to
   // convert them to double.  If any errors are caught, throw a syntax
   // error.
   double height, width;
   try {
      string temp1 = shift (command);
      string temp2 = shift (command);
      height = from_string<double> (temp1);
      width = from_string<double> (temp2);
   } catch (domain_error error) {
      throw runtime_error ("syntax error");
   }

   // Check if the command list is empty.  If so, set the thickness to
   // the default of 2.0 points.  If not, pop the thickness string from
   // the command list.  Try to convert the string to a double.  If any
   // errors are caught, throw a syntax error.
   double thick;
   if (command.empty ()) { thick = 2.0; }
   else {
      try {
         string temp = shift (command);
         thick = from_string<double> (temp);
      } catch (domain_error error) {
         throw runtime_error ("syntax error");
      }
   }

   return make_shared<rectangle>
       (inches(height), inches(width), points(thick));
}

object_ptr interpreter::make_square (parameters &command) {
   TRACE ('f', command);
      if (command.empty () || command.size () > 2)
      throw runtime_error ("syntax error");

   // Pop the width string from the command list.  Try to convert the
   // string to a double.  If any errors are caught, throw a syntax
   // error.
   double width;
   try {
      string temp = shift (command);
      width = from_string<double> (temp);
   } catch (domain_error error) {
      throw runtime_error ("syntax error");
   }
   
   // Check if the command list is empty.  If so, set the thickness to
   // the default of 2.0 points.  If not, pop the thickness string from
   // the command list.  Try to convert the string to a double.  If any
   // errors are caught, throw a syntax error.
   double thick;
   if (command.empty ()) { thick = 2.0; }
   else {
      try {
         string temp = shift (command);
         thick = from_string<double> (temp);
      } catch (domain_error error) {
         throw runtime_error ("syntax error");
      }
   }
   return make_shared<square> (inches(width), points(thick));
}

object_ptr interpreter::make_line (parameters &command) {
   TRACE ('f', command);
      if (command.empty () || command.size () > 2)
      throw runtime_error ("syntax error");

   // Pop the length string from the command list.  
   double length;
   try {
      string temp = shift (command);
      length = from_string<double> (temp);
   } catch (domain_error error) {
      throw runtime_error ("syntax error");
   }
   
   // Check if the command list is empty. 
   double thick;
   if (command.empty ()) { thick = 2.0; }
   else {
      try {
         string temp = shift (command);
         thick = from_string<double> (temp);
      } catch (domain_error error) {
         throw runtime_error ("syntax error");
      }
   }

   return make_shared<line> (inches(length), points(thick));
}

