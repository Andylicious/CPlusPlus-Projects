// $Id: main.cpp,v 1.2 2013-07-30 17:09:20-07 - - $
/*This program was completed using pair programming.
  Partner:  Andrew Lien (ahlien@ucsc.edu)
  Partner:  Daniel Jou (djou@ucsc.edu)
  CMPS 109
  Assignment 4
*/
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

typedef xpair<string,string> str_str_pair;
typedef listmap<string,string> str_str_map;
typedef listmap<string, string>::iterator shortItor;

string trim_spaces(string line){
  string checkWhite (" \t\f\v\n\r");
  size_t leBegin = line.find_first_not_of(checkWhite);
  size_t leEnd = line.find_last_not_of(checkWhite);
  line = line.substr(leBegin, leEnd+1);
  return line;
}
void prFileLine(const string &file, int line, const string &lstr){
   //cout<<"uh why can't this work?"<<endl;
   cout << file <<": " <<line<<": "<< lstr << endl;
}
void parsefile(const string &infilename, istream &infile){
  str_str_map map;
  for(int linenr = 1;; ++linenr){
  try{
    string theLine;
    getline(infile,theLine);
    if(infile.eof()) break;
    if(theLine.size() == 0){ 
      prFileLine(infilename,linenr,"");
      continue;
    }
            
    theLine = trim_spaces(theLine);
    prFileLine(infilename,linenr,theLine);
    size_t equalSign = theLine.find_first_of('=',0);
    string leftOf = theLine.substr(0,equalSign);
    string rightOf = theLine.substr(equalSign+1,theLine.size());
    if(equalSign == string::npos){
    //find theLine
    shortItor itor = map.find(theLine);
    if(theLine[0] == '#'){
      break; //ignore 
    }  
    if(theLine.size() > 0 && itor != map.end()){
      cout << itor->first << "=" << itor->second << endl; //found?
    }else{
      cout << theLine << ":key not found" << endl;//not found
     }  
  }else if(leftOf.size() != 0 && rightOf.size() ==0){ // a=
  //  cout<< "pass 1" <<endl;
    shortItor itor = map.find(leftOf);
    
    if(itor != map.end()) itor.erase();
     }else if(leftOf.size() != 0 && rightOf.size() != 0){
       shortItor itor = map.find(leftOf);
    if(itor == map.end()){
      str_str_pair pair;
      pair.first = leftOf;
      pair.second = rightOf;
      map.insert(pair);
      cout<<leftOf << " = " << rightOf<<endl; //new insert
      }else itor->second = rightOf;
      }else if(leftOf.size() == 0 && rightOf.size() ==0){
        shortItor itor = map.begin();
        shortItor end = map.end();
      for(;itor != end; ++itor){
        cout << itor->first << "=" << itor->second << endl;
      }  
    }else if(leftOf.size() == 0 && rightOf.size() != 0){
       shortItor itor = map.begin();
       shortItor end = map.end();
     for(;itor != end; ++itor){
       if(rightOf == itor->second){
         cout << itor->first << "=" << itor->second << endl;
       }  
     }
   }
 }catch(runtime_error error){
   complain()<<infilename<<":"<<linenr<<": "<<error.what()<<endl;
  }
 }
}

void scan_options (int argc, char **argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char **argv) {
  sys_info::set_execname (argv[0]);
  scan_options (argc, argv);
   if(optind == argc) {
      parsefile("-", cin);
   }else {
      for (int argi = optind; argi < argc; ++argi) {
         const string infilename = argv[argi];
         ifstream infile (infilename.c_str());
         if (infile.fail()) {
            syscall_error(infilename);
            continue;
         }
         parsefile (infilename,infile);
         infile.close();
      }
   }
  // cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

