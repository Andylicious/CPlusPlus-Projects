// $Id: listmap.cpp,v 1.3 2013-07-30 18:43:59-07 - - $
/*This program was completed using pair programming.
  Partner:  Andrew Lien (ahlien@ucsc.edu)
  Partner:  Daniel Jou (djou@ucsc.edu)
  CMPS 109
  Assignment 4
*/
#include "listmap.h"
#include "trace.h"
#include "xless.h"
xless<string>compare; 
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (const value_type &pair):
            pair(pair), prev(NULL), next(NULL) {
}

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::listmap (): head(NULL), tail (NULL) {
}

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap () {
   TRACE ('l', (void*) this);
   //cout<< "does delete even work?" << endl;
   listmap<Key,Value,Less>::iterator beginning = this->begin();
   listmap<Key,Value,Less>::iterator ending = this->end();
//   int i = 0;
   for(; beginning!=ending; ++beginning){
//   cout<<"Iteration: " << i <<endl;
     beginning.erase();
   }
}

template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::insert (const xpair<Key,Value> &pair) {
    node *tmp = new node (pair);
   if (empty ()) {
      head = tail = tmp;
   }else {  
      //here we will add based on lexicographical order
      //we have a case for insert before HEAD and a case
      //for insert before elements between HEAD and TAIL
      for(node *curr=head; curr!=NULL; curr=curr->next) {
         int cmp = compare (curr->pair.first, pair.first);
         if(cmp==1 && curr==head){
            tmp->next = head;
            tmp->prev = NULL;
            head->prev = tmp;
            head = tmp;
            return;
         }else if(cmp==1){
            tmp->next = curr;
            node *curr_prev = curr->prev;
            curr->prev = tmp;
            tmp->prev = curr_prev;
            curr_prev->next = tmp;
            return;
         }
      }
      //item was greater than those elments in list
      //so we default add it to the back
      tail->next = tmp;
      tmp->prev = tail;
      tail = tmp;
   }

}

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type &that) {
     auto itor = this->begin();
   auto end  = this->end();
   
   for(; itor != end; ++itor){
      if(that==itor->first) return iterator(this,itor.where); 
   }
   return iterator(NULL,NULL);
}

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::begin () {
   return iterator (this, head);
}

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::end () {
   return iterator (this, NULL);
}

template <typename Key, typename Value, class Less>
bool listmap<Key,Value,Less>::empty () const {
   return head == NULL;
}


template <typename Key, typename Value, class Less>
xpair<Key,Value> &listmap<Key,Value,Less>::iterator::operator* () {
   TRACE ('l', where->pair);
   return where->pair;
}

template <typename Key, typename Value, class Less>
xpair<Key,Value> *listmap<Key,Value,Less>::iterator::operator-> () {
   TRACE ('l', where->pair);
   return &(where->pair);
}

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator &
listmap<Key,Value,Less>::iterator::operator++ () {
   TRACE ('l', "First: " << map << ", " << where);
   TRACE ('l', "Second: " << map->head << ", " << map->tail);
   where = where->next;
   return *this;
}

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator &
listmap<Key,Value,Less>::iterator::operator-- () {
   where = where->prev;
   return *this;
}

template <typename Key, typename Value, class Less>
bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator &that) const {
   return this->where == that.where;
}

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::iterator::iterator (listmap *map,
            node *where): map (map), where (where){
}

template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::iterator::erase () {
   TRACE ('l', "map = " << map << ", where = " << where << endl);
   //node four cases; check for tail, check for head, 
   //check for mid, check for single
   //can i use a switch statement..?
   
   /* check for tail */
   if(where == map->tail){
     node *tmp = where->prev;
     tmp->next = NULL;
     delete where;
     map->tail = tmp;
     where = tmp;
   /*check for head*/  
   }else if(where == map->head){
     node *tmp = where->next;
     tmp->prev = NULL;
     delete where;
     map->head = tmp;
     where = tmp;
   /*utilize case 1 and case 2 restrictions*/  
   }else if(where == map->head && where == map->tail){
     delete where;
     map->head = NULL;
     map->tail = NULL;
     where = NULL;
   }else{ /*every single other case there is known to man*/
     node *previous = where->prev;
     node *theNext = where->next;
     previous->next = theNext;
     theNext->prev = previous;
     delete where;
     where = previous; //current position is to the left of deleted   
    } 
}

#include "templates.tcc"
