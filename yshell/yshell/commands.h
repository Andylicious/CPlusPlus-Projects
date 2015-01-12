
#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <map>

using namespace std;
//only write includes when they are absolutely needed
#include "inode.h"
#include "trace.h"
#include "util.h"

//
// A couple of convenient typedefs to avoid verbosity.
//

typedef void (*function) (inode_state &state, const wordvec &words);
                         //wordvec is declared somewhere else
                         //both of these are coming by reference
         //suppose i have f(foo p){
         //
         //}
         //int g(foo &q) {
         //
         //}
         //foo x, f(x) the entire structure copies over to function p
         //g(x) the parameter q is just a pointer to the object
         //it's fine to pass in primitives by value but when you 
         //pass in structures by value, copy constructor called

typedef map<string, ::function> commandmap;
//suppose i say commandmap cm;
//string key;
//y = cm[key] or y=cm.at(key)
//z = cm.find(key);
//3 ways to interrogate a map in c++, 
//usually what you want to do in general
//z->second is going to be the name of
// the function, the value type in a map is a pair




//
// commands -
//    A class to hold and dispatch each of the command functions.
//    Each command "foo" is interpreted by a function fn_foo.
// ctor -
//    The default ctor initializes the map.
// operator[] -
//    Given a string, returns a function associated with it,
//    or 0 if not found.
//


class commands {
private://redundant because default is private
    commands (const inode &) = delete; // copy ctor
    commands &operator= (const inode &) = delete; // operator=
    commandmap map;
//it's a map from a string to a function
// generally you want to make fields of a class private

public:
    commands();
//this is the default constructor,
// executed to create an object and initialize it
    ::function at (const string &cmd);
//this is a member function, in java a 
//method, and it takes a string as an argument
//because string is an object, make it by refeference, 
//and in allow to pass constants in, make it a constant reference
//if i have a map;; cmd.at() it's going to use mackey's version

};


//
// execution functions -
//    See the man page for a description of each of these functions.
//

void fn_cat    (inode_state &state, const wordvec &words);
void fn_cd     (inode_state &state, const wordvec &words);
void fn_echo   (inode_state &state, const wordvec &words);
void fn_exit   (inode_state &state, const wordvec &words);
void fn_ls     (inode_state &state, const wordvec &words);
void fn_lsr    (inode_state &state, const wordvec &words);
void fn_make   (inode_state &state, const wordvec &words);
void fn_mkdir  (inode_state &state, const wordvec &words);
void fn_prompt (inode_state &state, const wordvec &words);
void fn_pwd    (inode_state &state, const wordvec &words);
void fn_rm     (inode_state &state, const wordvec &words);
void fn_rmr    (inode_state &state, const wordvec &words);
//each function takes in the inode state, and the word value 


//
// exit_status_message -
//    Prints an exit message and returns the exit status, as recorded
//    by any of the functions.
//

int exit_status_message();
class ysh_exit_exn: public exception {};
//declare an exception 
//in general, in c++ you can throw pretty much anything as opposed
//to java where in java the only thing you can throw is a class that
//extends throwable, and the only 
//class that does that are errors & runtime exceptions
//in C++, if n is an intenger, you can say throw n
#endif
