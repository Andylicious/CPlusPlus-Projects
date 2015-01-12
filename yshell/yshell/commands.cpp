// $Id: commands.cpp,v 1.52 2013-07-08 01:34:25-07 - - $
/*This program was completed using pair programming.
Partner:  Andrew Lien (ahlien@ucsc.edu)
Partner:  Daniel Jou (djou@ucsc.edu)
CMPS 109
Assignment 1
*/
#include "commands.h"
#include "trace.h"

commands::commands(): map (commandmap()) {
    map["cat"    ] = fn_cat    ;
    map["cd"     ] = fn_cd     ;
    map["echo"   ] = fn_echo   ;
    map["exit"   ] = fn_exit   ;
    map["ls"     ] = fn_ls     ;
    map["lsr"    ] = fn_lsr    ;
    map["make"   ] = fn_make   ;
    map["mkdir"  ] = fn_mkdir  ;
    map["prompt" ] = fn_prompt ;
    map["pwd"    ] = fn_pwd    ;
    map["rm"     ] = fn_rm     ;
}

::function commands::at (const string& cmd) {
    // Note: value_type is pair<const key_type, mapped_type>
    // So: iterator->first is key_type (string)
    // So: iterator->second is mapped_type (function)
    commandmap::const_iterator result = map.find (cmd);
    if (result == map.end()) {
        throw yshell_exn (cmd + ": no such function");
    }
    return result->second;
}

inode* naviPath(inode_state& state, const wordvec &words,
                     int path_index, int limitBy);
void lsrRecur(inode* current_inode, string pathname);
void rmrRecur(inode* current_inode);
void fn_cat (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   
   if ( words.size() == 1 ){ //if entered "cat" with no arguments
      cout << ": No such file or directory" << endl;
   }else{ //argument
      wordvec_itor itor = words.begin() + 1;
      const wordvec_itor end = words.end();
      for (int i = 1; itor != end; ++itor, ++i){
         inode* target = naviPath(state, words, i, 0);
         if ( target != NULL && !target->is_dir() ){
            cout << target->readfile() << endl;
            //cout <<  << endl;
         } else{
            cout << "cat: " << words[i] << 
            ": No such file or directory" << endl;
         }
      }
   }
}

void fn_cd (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
    string error;
    //easy, if command 'cd' only, 
    //then change current working directory to root
    if(words.size() == 1) {
      state.set_cwd_to_root();
    }
    //when there's an argument, two cases, if no directory
    //or if there is a directory
    else if(words.size() == 2){
    inode* targetPath = naviPath(state,words,1,0);
    if(targetPath == NULL) { //if the directory does not exist
      error +="cd: " + words[1] + 
         ": No such file or directory";
    } else{ //if the directory does exist
        state.set_cwd(targetPath);
        
      }  
    }
   else{ //all cases
   error += "cd:";
   wordvec_itor itor = words.begin() +1;
   while(itor != words.end()) error += " " + *itor++;
     error += ": No such file or directory";
   }
   if(error.size() > 0)
     throw yshell_exn(error);    
}

void fn_echo (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
    string output("");
    //make sure size of word vector > 1
    if(words.size() >1){
    //start after typing "echo"
    //words.begin() = echo, words.begin() +1 words after
      wordvec_itor itor = words.begin() +1;
      const wordvec_itor end = words.end();
      //from first word to last word...
      for(; itor !=end; ++itor){
       //have string itor = output
        output += *itor;
        //concatenate w/ string
        output += " ";
      }
      output.erase(output.end()-1);
    }    
    cout << output << endl;
}
/*
int atoi (const char * str); converts string to integer. 
string::c_str ; C++98 returns C string equivalent
*/
void fn_exit (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
    int exitCode = 0;
    if(words.size() >1){
      exitCode = atoi(words[1].c_str());
      exit_status::set(exitCode);
    }  
    throw ysh_exit_exn ();
}

void fn_ls (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
    if(words.size() == 1){
     //list the directory i'm in
     inode* cwd = state.getcwd();
     //cwd->name() == name of current working directory
     cout << cwd->name() << ":\n" << cwd << "  " << endl;
   }else{
      inode* target;
      wordvec_itor itor = words.begin();
      ++itor;
      wordvec_itor end = words.end();
      for(int i = 1; itor != end; ++itor, ++i){
        target = naviPath(state,words,i,0);
        if(target == NULL)
          cerr << "ls: Invalid file or directory: "
              << words[i] << endl;
         else
           cout << words[i] << ":" << "\n" << target << " " << endl;
         }       
     }     
}
//naviPath is going to take in the state, the words
// first path & it's limit
inode* naviPath(inode_state& state, const wordvec &words,
                     int path_index, int limitBy){
   //check if we're at the root or not                  
   bool start_at_root = false;
   wordvec path = split(words[path_index],"/");
   //if path index is at 0, then we are at the root
   if (words[path_index].at(0)=='/') start_at_root = true;
      
   inode* head;
   // easy part, if you're at root, then head = root
   if ( start_at_root == true) head = state.getroot();
   //otherwise, head is the current working directory
     else head = state.getcwd();
   //find size of path
   int size = path.size()-limitBy;
   for ( int i = 0; i < size; ++i ){
   //if the head is a directory
      if ( head->is_dir() ){
      //traverse down the child directories until head is a file
         head = head->get_child_dir(path[i]);
      } else{ 
         head = NULL;
         //failed to path
      }
      //redundant
      if ( head == NULL )
         return NULL;
   }
   return head;
}
void fn_lsr (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
    string error("");
    inode* head;
    //if just "lsr"
    if(words.size()==1){
      head = state.getcwd(); //get current cwd
      string n;
      lsrRecur(head, n); //perform recursive lsr
    } else{//otherwise, for multiple arguments  
        wordvec_itor itor = words.begin();
        ++itor; //make sure to increment so we don't use lsr
        //edit: might try to use words.begin() + 1 if not work
        wordvec_itor end = words.end();
        for (int i = 1; itor!=end; ++itor, ++i){
          //find out where the head is
          head = naviPath(state,words,i,0);
          if(head != NULL){ //if file/direct exists
            string n;
            lsrRecur(head,n);
          }else{ // if it DNE
             cout<<"lsr: No such file or directory: "
             <<words[i] << endl;          
           }  
        }
      }
}

void lsrRecur(inode* current_inode, string pathname){
  const string name = current_inode->name();
  pathname += current_inode->name();
  cout<<pathname<<":\n" << current_inode<<endl;
  directory* directory = current_inode->get_dirents();
  directory::const_iterator itor = directory->begin();
  ++itor; ++itor;
  directory::const_iterator end = directory->end();
  if(pathname.compare("/") != 0) pathname +="/";
  for(; itor != end; ++itor) {
    if(itor->second->is_dir()){
      lsrRecur(itor->second,pathname);
    }
  }    
}

void fn_make (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
    string error("");
    if(words.size() == 1) {
      error+="make: Please specify a filename";
    } else{
        inode* targetOfParent = naviPath(state,words,1,1);
        if(targetOfParent != NULL){
          bool newf = false;
          wordvec path = split(words[1], "/");
          string directname(path[path.size()-1]);
          inode* targetFile = targetOfParent->get_child_dir(directname);
          //creates a new txtfile if it doesn't already exist
        if(targetFile == NULL){
          targetFile = new inode(FILE_INODE);
          newf = true;
        }
        
        if( !targetFile->is_dir()){
          targetFile->writefile(words);
          if(newf == true){
            string name(words[1]);
            targetOfParent->add_file(directname, targetFile);
            
          }
        }else{
          error += "make: directory already exists with name: " 
            + words[1];
         } 
       } else{
           error +="make: Invalid file or directory";
         }      
     }          
    if ( error.size() > 0 )
      throw yshell_exn(error);
}

void fn_mkdir (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
    string error("");
    if(words.size() == 1 || words.size() >2) {
      error += "mkdir: Invalid directory name";
    }else if(words.size() == 2) {
       inode* target = naviPath(state,words,1,1);
       if(target != NULL){
       //check if directory exists already
       wordvec path = split(words[1],"/");
       if(target->get_child_dir(path[path.size()-1])!=NULL)
         error += "mkdir: Directory already exists";
       else{
       //make a new directory here
       inode* newDirectory = new inode(DIR_INODE);
       target->add_dirent(path[path.size()-1],newDirectory);
       }
     }else{
       error += "mkdir: invalid path";
     }
   }
   if(error.size() >0)
     throw yshell_exn(error);
             
}
//cannot get prompt to work

void fn_prompt (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
  //need to grab all words in the word vector
  //create into single space sep by string
   string newprompt;
   if ( words.size() > 1 ){
      wordvec_itor itor = words.begin() + 1;
      const wordvec_itor end = words.end();
      for (; itor != end; ++itor) {
         newprompt += *itor;
         newprompt += " ";
      }
      newprompt.erase(newprompt.end()-1);
   }
   state.setprompt(newprompt);
}

void fn_pwd (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
    string path("");
    //get the current working directory
    path += state.getcwd()->name();
    //collect parent of working directory
    inode* parent = state.getcwd()->get_parent();
    //while traversing up until we hit root directory
    while(parent->get_inode_nr() !=
       state.getroot()->get_inode_nr() ){
      //append backwards the parent name
      path = parent->name() + "/" + path;
      //increment the parent up
      parent = parent->get_parent();
    }
    if (path.compare("/") != 0)
      path = "/" + path;
    cout << path << endl;    
}

void fn_rm (inode_state &state, const wordvec &words){
   TRACE ('c', state);
   TRACE ('c', words);
   string error("");
   //must have only one argument
   if ( words.size() == 1 || words.size() > 2 ){
      error += "rm: Invalid file or directory";
   }//when rm [file/directory]
   else if ( words.size() == 2){
      inode* target = naviPath(state, words, 1, 0);
      //if target is a file/directory
      if ( target != NULL ){
      //if target is a directory or is a file w/ size less than 2
         if (!target->is_dir() || 
            ( target->is_dir() && target->size() <= 2 )){
        //rm the parent     
            inode* targetOfParent = naviPath(state, words, 1, 1);
            wordvec path = split(words[1],"/");
            //delete child from parent
            if ( targetOfParent->delete_child( path[path.size()-1] )
                == false ){
               error += "rm: Cannot delete '.' or '..'";
            }else 
               TRACE ('c', "Successfully deleted child from parents" );
         }else {
            error += "rm: Cannot delete a non-empty directory";
         }
      }else {
         error += "rm: Invalid file or directory";
      }
   }
   if ( error.size() > 0 )
      throw yshell_exn(error);
}

void fn_rmr (inode_state &state, const wordvec &words){
    TRACE ('c', state);
    TRACE ('c', words);
    
    string error("");
    //must have one other argument after rmr
    if(words.size() == 1 || words.size() > 2) {
      error += "rmr: Invalid file or directory";
    } else if (words.size() == 2) {
      //collect the head & the parent inodes
        inode* targetOfHead = naviPath(state,words,1,0);
        inode* parent = naviPath(state,words,1,1);
      //if the target is a directory
      //simply traverse down until it's at child inode  
        if(targetOfHead->is_dir()){
          rmrRecur(targetOfHead);
        } 
        //if at inode child, delete the child
        if(parent->get_inode_nr() != targetOfHead->get_inode_nr()){
          parent->delete_child(targetOfHead->name());
        }
     }
     if(error.size() > 0)
       throw yshell_exn(error);     
}

// MAKE SURE CURRENT INODE IS DIRECTORY INODE
void rmrRecur(inode* current_inode){
   directory* directory = current_inode->get_dirents();
   directory::iterator itor = directory->begin();
   ++itor; ++itor;
   directory::const_iterator end = directory->end();
   for (; itor != end;) {
      if ( itor->second->is_dir() )
         rmrRecur(itor->second);
      directory->erase(itor++);
   }
}   

int exit_status_message() {
    int exit_status = exit_status::get();
    cout << execname() << ": exit(" << exit_status << ")" << endl;
    return exit_status;
}
