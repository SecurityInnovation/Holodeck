
#include <jm/jm_cfg.h>
#include <jm/re_raw.h>

#ifdef JM_MSC_VER
#include <crtdbg.h>
#endif

#ifndef JM_STDC
#error
#endif

#ifdef JM_DEBUG

char b1[32] = {0,};
char guard_pattern[32] 
= { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, };
char b2[32] = {0,};

static const int guard_size = 32;

bool check_pattern(void* p)
{
   return p ? memcmp(p, guard_pattern, guard_size) : 0;
}

inline unsigned maxi(unsigned i, unsigned j)
{
   return i < j ? j : i;
}

unsigned int allocated = 0;

struct init
{
   init();
   ~init();
};

init::init()
{ 
#ifdef JM_MSC_VER
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#endif
}

init::~init() 
{ 
}


init i;

void* get_mem(size_t n)
{
   ++allocated;
   char* p = (char*)malloc(n + guard_size * 2 + maxi(sizeof(int) + sizeof(void*), JM::padding_size) + JM::padding_size);
   char* base = p;
   p = (char*)((JM_STDC::ptrdiff_t)(p + JM::padding_mask) & ~JM::padding_mask);
   memcpy(p + maxi(sizeof(int) + sizeof(void*), JM::padding_size), guard_pattern, guard_size);
   memcpy(p + maxi(sizeof(int) + sizeof(void*), JM::padding_size) + guard_size + n, guard_pattern, guard_size);
   *(int*)p = n;
   *(void**)(p + sizeof(int)) = base;
   return p + guard_size + maxi(sizeof(int) + sizeof(void*), JM::padding_size);
}

void free_mem(void* b)
{
   if(b)
   {
      char* p = (char*)b;
      p -= (guard_size + maxi(sizeof(int) + sizeof(void*), JM::padding_size));
      if(check_pattern(p + maxi(sizeof(int) + sizeof(void*), JM::padding_size)) || check_pattern(p + maxi(sizeof(int) + sizeof(void*), JM::padding_size) + guard_size + *(int*)p))
      {
         cerr << "Error: freed memory has been written past end..." << endl;
      }
      free(*(void**)(p + sizeof(int)));
      --allocated;
   }
}

void* operator new(size_t n)
{
   return get_mem(n);
}

void* operator new[](size_t n)
{
   return get_mem(n);
}

void operator delete(void* p)
{
   free_mem(p);
}

void operator delete[](void* p)
{
   free_mem(p);
}

#include <set>

JM_NAMESPACE(JM)

std::set<debug_guard*, std::less<debug_guard*> >* patterns = 0;

int pattern_count = 0;

void check_patterns(const char* f, int l)
{
   if(pattern_count)
   {
      std::set<debug_guard*, std::less<debug_guard*> >::iterator i, j;
      i = patterns->begin();
      j = patterns->end();
      while(i != j)
      {
         if(check_pattern((void*)(*i)->g1) || check_pattern((*i)->g2) || check_pattern((void*)(*i)->pc) || check_pattern((*i)->pnc))
         {
            cerr << "Error: static memory corruption at " << hex << *i << dec << " in " << (*i)->file << "@" << (*i)->line << " called from " << f << "@" << l << endl;
         }
         ++i;
      }
   }
}

debug_guard::debug_guard(const char* f, int l, const char* p1, char* p2)
{
   if(++pattern_count == 1)
      patterns = new std::set<debug_guard*, std::less<debug_guard*> >;
   file = f;
   line = l;
   memcpy(g1, guard_pattern, guard_size);
   memcpy(g2, guard_pattern, guard_size);
   if(p1)
   {
      pc = p1;
   }
   else
      pc = 0;
   if(p2)
   {
      pnc = p2;
      memcpy(pnc, guard_pattern, guard_size);
   }
   else
      pnc = 0;
   patterns->insert(this);
}

debug_guard::~debug_guard()
{
   check_patterns(file, line);
   if(check_pattern(g1) || check_pattern(g2))
   {
      cerr << "Error: memory corruption " << file << "@" << line << endl;
   }
   patterns->erase(this);
   if(--pattern_count == 0)
   {
      delete patterns;
      patterns = 0;
   }
}

JM_END_NAMESPACE



#endif
