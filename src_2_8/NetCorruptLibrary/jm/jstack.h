/*
 *
 * Copyright (c) 1998-9
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 */

  /*
  *   FILE     jstack.h
  *   VERSION  2.25
  */

#ifndef JSTACH_H
#define JSTACK_H

#ifndef JM_CFG_H
#include <jm/jm_cfg.h>
#endif
#ifndef RE_RAW_H
#include <jm/re_raw.h>
#endif

JM_NAMESPACE(JM)

#ifdef __BORLANDC__
   #if __BORLANDC__ == 0x530
    #pragma option push -a4 -b
   #elif __BORLANDC__ > 0x530
    #pragma option push -a8 -b
   #endif
#endif

//
// class jstack
// simplified stack optimised for push/peek/pop
// operations, we could use std::stack<std::vector<T>> instead...
//
template <class T, class Allocator JM_DEF_ALLOC_PARAM(T) >
class jstack
{
public:
   typedef JM_MAYBE_TYPENAME REBIND_TYPE(unsigned char, Allocator) alloc_type;
   typedef typename alloc_type::size_type size_type;
   typedef T value_type;
private:
   struct node
   {
      node* next;
      T* start;  // first item
      T* end;    // last item
      T* last;   // end of storage
   };
   
   //
   // empty base member optimisation:
   struct data : public alloc_type
   {
      padding buf[(sizeof(T) * 16 + sizeof(padding) - 1) / sizeof(padding)];
      data(const Allocator& a) : alloc_type(a){}
   };

   data alloc_inst;
   mutable node* m_stack;
   mutable node* unused;
   node base;
   size_type block_size;

   void RE_CALL pop_aux()const;
   void RE_CALL push_aux();

public:
#ifndef JM_AIX
   jstack(size_type n = 64, const Allocator& a = Allocator());
#else
   template <class T, class Allocator>
   jstack<T, Allocator>::jstack(size_type n, const Allocator& a)
       : alloc_inst(a)
   {
     unused = 0;
     block_size = n;
     m_stack = &base;
     base.last = (T*)(alloc_inst.buf);
     base.end = base.start = base.last + 16;
     base.next = 0;
   }
#endif

   ~jstack();

   node* RE_CALL get_node()
   {
      node* new_stack = (node*)alloc_inst.allocate(sizeof(node) + sizeof(T) * block_size);
      new_stack->last = (T*)(new_stack+1);
      new_stack->start = new_stack->end = new_stack->last + block_size;
      new_stack->next = 0;
      return new_stack;
   }

   bool RE_CALL empty()
   {
      return (m_stack->start == m_stack->end) && (m_stack->next == 0);
   }

   bool RE_CALL good()
   {
      return (m_stack->start != m_stack->end) || (m_stack->next != 0);
   }

   T& RE_CALL peek()
   {
      if(m_stack->start == m_stack->end)
         pop_aux();
      return *m_stack->end;
   }

   const T& RE_CALL peek()const
   {
      if(m_stack->start == m_stack->end)
         pop_aux();
      return *m_stack->end;
   }

   void RE_CALL pop()
   {
      if(m_stack->start == m_stack->end)
         pop_aux();
      jm_destroy(m_stack->end);
      ++(m_stack->end);
   }

   void RE_CALL pop(T& t)
   {
      if(m_stack->start == m_stack->end)
         pop_aux();
      t = *m_stack->end;
      jm_destroy(m_stack->end);
      ++(m_stack->end);
   }

   void RE_CALL push(const T& t)
   {
      if(m_stack->end == m_stack->last)
         push_aux();
      --(m_stack->end);
      jm_construct(m_stack->end, t);
   }

};
#ifndef JM_AIX
template <class T, class Allocator>
jstack<T, Allocator>::jstack(size_type n, const Allocator& a)
    : alloc_inst(a)
{
  unused = 0;
  block_size = n;
  m_stack = &base;
  base.last = ((T*)(alloc_inst.buf));
  base.end = base.start = base.last + 16;
  base.next = 0;
}
#endif
template <class T, class Allocator>
void RE_CALL jstack<T, Allocator>::push_aux()
{
   // make sure we have spare space on TOS:
   register node* new_node;
   if(unused)
   {
      new_node = unused;
      unused = new_node->next;
      new_node->next = m_stack;
      m_stack = new_node;
   }
   else
   {
      new_node = get_node();
      new_node->next = m_stack;
      m_stack = new_node;
   }
}

template <class T, class Allocator>
void RE_CALL jstack<T, Allocator>::pop_aux()const
{
   // make sure that we have a valid item
   // on TOS:
   jm_assert(m_stack->next);
   register node* p = m_stack;
   m_stack = p->next;
   p->next = unused;
   unused = p;
}

template <class T, class Allocator>
jstack<T, Allocator>::~jstack()
{
   node* condemned;
   while(good())
      pop();
   while(unused)
   {
      condemned = unused;
      unused = unused->next;
      alloc_inst.deallocate((unsigned char*)condemned, sizeof(node) + sizeof(T) * block_size);
   }
   while(m_stack != &base)
   {
      condemned = m_stack;
      m_stack = m_stack->next;
      alloc_inst.deallocate((unsigned char*)condemned, sizeof(node) + sizeof(T) * block_size);
   }
}

#ifdef __BORLANDC__
 #if __BORLANDC__ > 0x520
  #pragma option pop
 #endif
#endif

JM_END_NAMESPACE

#endif








