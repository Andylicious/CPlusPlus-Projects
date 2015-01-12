
#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>

using namespace std;

#include "trace.h"
#include "util.h"

//
// inode_t -
//    An inode is either a directory or a plain file.
//

enum inode_t {DIR_INODE, FILE_INODE};

//
// directory -
//    A directory is a list of paired strings (filenames) and inodes.
//    An inode in a directory may be a directory or a file.
//

class inode;
typedef map<string, inode *> directory;

//
// inode_state -
//    A small convenient class to maintain the state of the simulated
//    process:  the root (/), the current directory (.), and the
//    prompt.
//

class inode_state {
   friend class inode;
   friend ostream &operator<< (ostream &out, const inode_state &);
   private:
      inode_state (const inode_state &); // disable copy ctor
      inode_state &operator= (const inode_state &); // disable op=
      inode *root;
      inode *cwd;
      string prompt;
   public:
      inode_state();
      ~inode_state();
      void setprompt(const string &newprompt);
      string getprompt();
      void newFileSystemState();
      inode* getcwd();
      inode* getroot();
      void set_cwd_to_root();
      void set_cwd(inode* node);
};

ostream &operator<< (ostream &out, const inode_state &);




class inode {
   friend ostream &operator<< (ostream &out, inode* );
   private:
      int inode_nr;
      inode_t type;
      union {
         directory *dirents;
         wordvec *data;
      } contents;
      static int next_inode_nr;
      //inode* parent;
   public:
      inode (inode_t init_type);
      inode (const inode &source);
      ~inode();
      inode &operator= (const inode &from);
      int get_inode_nr();
      int size();
      const wordvec &readfile() const;
      void writefile (const wordvec &newdata);
      void remove (const string &filename);
      inode* get_child_dir(const string& childname);
      void set_parent(inode* parent);
      void add_dirent(string& name, inode* addition);
      void add_file(string& name, inode* newfile);
      const string name();
      bool is_dir();
      directory* get_dirents();
      bool delete_child(const string& child_name);
      inode* get_parent();
};

ostream &operator<< (ostream &out, inode* );

#endif

