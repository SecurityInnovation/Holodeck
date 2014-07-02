
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
  *   FILE     re_nlsw.cpp
  *   VERSION  2.25
  */

#include <jm/jm_cfg.h>

#ifndef JM_NO_WCSTRING
#include <jm/re_nls.h>
#include <jm/re_thrd.h>
#include <jm/re_str.h>
#include <jm/re_mss.h>
#include <jm/regex.h>
#include <jm/re_lst.h>
#include <jm/re_cls.h>
#include <jm/re_coll.h>

#include <locale.h>
#include <stdio.h>
#include <ctype.h>

JM_NAMESPACE(JM)

#ifndef RE_LOCALE_CPP

JM_IX_DECL unsigned short re_unicode_classes[] = {
   char_class_cntrl,        // ''  0
   char_class_cntrl,        // ''  1
   char_class_cntrl,        // ''  2
   char_class_cntrl,        // ''  3
   char_class_cntrl,        // ''  4
   char_class_cntrl,        // ''  5
   char_class_cntrl,        // ''  6
   char_class_cntrl,        // ''  7
   char_class_cntrl,        // ''  8
   char_class_cntrl | char_class_space | char_class_blank,        // ''  9
   char_class_cntrl | char_class_space,        // ''  10
   char_class_cntrl | char_class_space,        // ''  11
   char_class_cntrl | char_class_space,        // ''  12
   char_class_cntrl | char_class_space,        // ''  13
   char_class_cntrl,        // ''  14
   char_class_cntrl,        // ''  15
   char_class_cntrl,        // ''  16
   char_class_cntrl,        // ''  17
   char_class_cntrl,        // ''  18
   char_class_cntrl,        // ''  19
   char_class_cntrl,        // ''  20
   char_class_cntrl,        // ''  21
   char_class_cntrl,        // ''  22
   char_class_cntrl,        // ''  23
   char_class_cntrl,        // ''  24
   char_class_cntrl,        // ''  25
   char_class_cntrl,        // ''  26
   char_class_cntrl,        // ''  27
   char_class_cntrl,        // ''  28
   char_class_cntrl,        // ''  29
   char_class_cntrl,        // ''  30
   char_class_cntrl,        // ''  31
   char_class_space | char_class_blank,        // ' '  32
   char_class_punct,        // '!'  33
   char_class_punct,        // '"'  34
   char_class_punct,        // '#'  35
   char_class_punct,        // '$'  36
   char_class_punct,        // '%'  37
   char_class_punct,        // '&'  38
   char_class_punct,        // '''  39
   char_class_punct,        // '('  40
   char_class_punct,        // ')'  41
   char_class_punct,        // '*'  42
   char_class_punct,        // '+'  43
   char_class_punct,        // ','  44
   char_class_punct,        // '-'  45
   char_class_punct,        // '.'  46
   char_class_punct,        // '/'  47
   char_class_digit |   char_class_xdigit,        // '0'  48
   char_class_digit |   char_class_xdigit,        // '1'  49
   char_class_digit |   char_class_xdigit,        // '2'  50
   char_class_digit |   char_class_xdigit,        // '3'  51
   char_class_digit |   char_class_xdigit,        // '4'  52
   char_class_digit |   char_class_xdigit,        // '5'  53
   char_class_digit |   char_class_xdigit,        // '6'  54
   char_class_digit |   char_class_xdigit,        // '7'  55
   char_class_digit |   char_class_xdigit,        // '8'  56
   char_class_digit |   char_class_xdigit,        // '9'  57
   char_class_punct,        // ':'  58
   char_class_punct,        // ';'  59
   char_class_punct,        // '<'  60
   char_class_punct,        // '='  61
   char_class_punct,        // '>'  62
   char_class_punct,        // '?'  63
   char_class_punct,        // '@'  64
   char_class_alpha |   char_class_upper | char_class_xdigit,        // 'A'  65
   char_class_alpha |   char_class_upper | char_class_xdigit,        // 'B'  66
   char_class_alpha |   char_class_upper | char_class_xdigit,        // 'C'  67
   char_class_alpha |   char_class_upper | char_class_xdigit,        // 'D'  68
   char_class_alpha |   char_class_upper | char_class_xdigit,        // 'E'  69
   char_class_alpha |   char_class_upper | char_class_xdigit,        // 'F'  70
   char_class_alpha |   char_class_upper,        // 'G'  71
   char_class_alpha |   char_class_upper,        // 'H'  72
   char_class_alpha |   char_class_upper,        // 'I'  73
   char_class_alpha |   char_class_upper,        // 'J'  74
   char_class_alpha |   char_class_upper,        // 'K'  75
   char_class_alpha |   char_class_upper,        // 'L'  76
   char_class_alpha |   char_class_upper,        // 'M'  77
   char_class_alpha |   char_class_upper,        // 'N'  78
   char_class_alpha |   char_class_upper,        // 'O'  79
   char_class_alpha |   char_class_upper,        // 'P'  80
   char_class_alpha |   char_class_upper,        // 'Q'  81
   char_class_alpha |   char_class_upper,        // 'R'  82
   char_class_alpha |   char_class_upper,        // 'S'  83
   char_class_alpha |   char_class_upper,        // 'T'  84
   char_class_alpha |   char_class_upper,        // 'U'  85
   char_class_alpha |   char_class_upper,        // 'V'  86
   char_class_alpha |   char_class_upper,        // 'W'  87
   char_class_alpha |   char_class_upper,        // 'X'  88
   char_class_alpha |   char_class_upper,        // 'Y'  89
   char_class_alpha |   char_class_upper,        // 'Z'  90
   char_class_punct,        // '['  91
   char_class_punct,        // '\'  92
   char_class_punct,        // ']'  93
   char_class_punct,        // '^'  94
   char_class_punct | char_class_underscore,        // '_'  95
   char_class_punct,        // '`'  96
   char_class_alpha |  char_class_lower |  char_class_xdigit,        // 'a'  97
   char_class_alpha |  char_class_lower |  char_class_xdigit,        // 'b'  98
   char_class_alpha |  char_class_lower |  char_class_xdigit,        // 'c'  99
   char_class_alpha |  char_class_lower |  char_class_xdigit,        // 'd'  100
   char_class_alpha |  char_class_lower |  char_class_xdigit,        // 'e'  101
   char_class_alpha |  char_class_lower |  char_class_xdigit,        // 'f'  102
   char_class_alpha |  char_class_lower,        // 'g'  103
   char_class_alpha |  char_class_lower,        // 'h'  104
   char_class_alpha |  char_class_lower,        // 'i'  105
   char_class_alpha |  char_class_lower,        // 'j'  106
   char_class_alpha |  char_class_lower,        // 'k'  107
   char_class_alpha |  char_class_lower,        // 'l'  108
   char_class_alpha |  char_class_lower,        // 'm'  109
   char_class_alpha |  char_class_lower,        // 'n'  110
   char_class_alpha |  char_class_lower,        // 'o'  111
   char_class_alpha |  char_class_lower,        // 'p'  112
   char_class_alpha |  char_class_lower,        // 'q'  113
   char_class_alpha |  char_class_lower,        // 'r'  114
   char_class_alpha |  char_class_lower,        // 's'  115
   char_class_alpha |  char_class_lower,        // 't'  116
   char_class_alpha |  char_class_lower,        // 'u'  117
   char_class_alpha |  char_class_lower,        // 'v'  118
   char_class_alpha |  char_class_lower,        // 'w'  119
   char_class_alpha |  char_class_lower,        // 'x'  120
   char_class_alpha |  char_class_lower,        // 'y'  121
   char_class_alpha |  char_class_lower,        // 'z'  122
   char_class_punct,        // '{'  123
   char_class_punct,        // '|'  124
   char_class_punct,        // '}'  125
   char_class_punct,        // '~'  126

   char_class_cntrl,        // ''  127
   char_class_cntrl,        // 'Ä'  128
   char_class_cntrl,        // 'Å'  129
   char_class_cntrl,        // 'Ç'  130
   char_class_cntrl,        // 'É'  131
   char_class_cntrl,        // 'Ñ'  132
   char_class_cntrl,        // 'Ö'  133
   char_class_cntrl,        // 'Ü'  134
   char_class_cntrl,        // 'á'  135
   char_class_cntrl,        // 'à'  136
   char_class_cntrl,        // 'â'  137
   char_class_cntrl,        // 'ä'  138
   char_class_cntrl,        // 'ã'  139
   char_class_cntrl,        // 'å'  140
   char_class_cntrl,        // 'ç'  141
   char_class_cntrl,        // 'é'  142
   char_class_cntrl,        // 'è'  143
   char_class_cntrl,        // 'ê'  144
   char_class_cntrl,        // 'ë'  145
   char_class_cntrl,        // 'í'  146
   char_class_cntrl,        // 'ì'  147
   char_class_cntrl,        // 'î'  148
   char_class_cntrl,        // 'ï'  149
   char_class_cntrl,        // 'ñ'  150
   char_class_cntrl,        // 'ó'  151
   char_class_cntrl,        // 'ò'  152
   char_class_cntrl,        // 'ô'  153
   char_class_cntrl,        // 'ö'  154
   char_class_cntrl,        // 'õ'  155
   char_class_cntrl,        // 'ú'  156
   char_class_cntrl,        // 'ù'  157
   char_class_cntrl,        // 'û'  158
   char_class_cntrl,        // 'ü'  159
   char_class_space | char_class_blank,        // '†'  160
   char_class_punct,        // '°'  161
   char_class_punct,        // '¢'  162
   char_class_punct,        // '£'  163
   char_class_punct,        // '§'  164
   char_class_punct,        // '•'  165
   char_class_punct,        // '¶'  166
   char_class_punct,        // 'ß'  167
   char_class_punct,        // '®'  168
   char_class_punct,        // '©'  169
   char_class_punct,        // '™'  170
   char_class_punct,        // '´'  171
   char_class_punct,        // '¨'  172
   char_class_punct,        // '≠'  173
   char_class_punct,        // 'Æ'  174
   char_class_punct,        // 'Ø'  175
   char_class_punct,        // '∞'  176
   char_class_punct,        // '±'  177
   char_class_punct,        // '≤'  178
   char_class_punct,        // '≥'  179
   char_class_punct,        // '¥'  180
   char_class_punct,        // 'µ'  181
   char_class_punct,        // '∂'  182
   char_class_punct,        // '∑'  183
   char_class_punct,        // '∏'  184
   char_class_punct,        // 'π'  185
   char_class_punct,        // '∫'  186
   char_class_punct,        // 'ª'  187
   char_class_punct,        // 'º'  188
   char_class_punct,        // 'Ω'  189
   char_class_punct,        // 'æ'  190
   char_class_punct,        // 'ø'  191
   char_class_alpha |   char_class_upper,        // '¿'  192
   char_class_alpha |   char_class_upper,        // '¡'  193
   char_class_alpha |   char_class_upper,        // '¬'  194
   char_class_alpha |   char_class_upper,        // '√'  195
   char_class_alpha |   char_class_upper,        // 'ƒ'  196
   char_class_alpha |   char_class_upper,        // '≈'  197
   char_class_alpha |   char_class_upper,        // '∆'  198
   char_class_alpha |   char_class_upper,        // '«'  199
   char_class_alpha |   char_class_upper,        // '»'  200
   char_class_alpha |   char_class_upper,        // '…'  201
   char_class_alpha |   char_class_upper,        // ' '  202
   char_class_alpha |   char_class_upper,        // 'À'  203
   char_class_alpha |   char_class_upper,        // 'Ã'  204
   char_class_alpha |   char_class_upper,        // 'Õ'  205
   char_class_alpha |   char_class_upper,        // 'Œ'  206
   char_class_alpha |   char_class_upper,        // 'œ'  207
   char_class_alpha |   char_class_upper,        // '–'  208
   char_class_alpha |   char_class_upper,        // '—'  209
   char_class_alpha |   char_class_upper,        // '“'  210
   char_class_alpha |   char_class_upper,        // '”'  211
   char_class_alpha |   char_class_upper,        // '‘'  212
   char_class_alpha |   char_class_upper,        // '’'  213
   char_class_alpha |   char_class_upper,        // '÷'  214
   char_class_punct,        // '◊'  215
   char_class_alpha |   char_class_upper,        // 'ÿ'  216
   char_class_alpha |   char_class_upper,        // 'Ÿ'  217
   char_class_alpha |   char_class_upper,        // '⁄'  218
   char_class_alpha |   char_class_upper,        // '€'  219
   char_class_alpha |   char_class_upper,        // '‹'  220
   char_class_alpha |   char_class_upper,        // '›'  221
   char_class_alpha |  char_class_upper,        // 'ﬁ'  222
   char_class_alpha |  char_class_lower,        // 'ﬂ'  223
   char_class_alpha |  char_class_lower,        // '‡'  224
   char_class_alpha |  char_class_lower,        // '·'  225
   char_class_alpha |  char_class_lower,        // '‚'  226
   char_class_alpha |  char_class_lower,        // '„'  227
   char_class_alpha |  char_class_lower,        // '‰'  228
   char_class_alpha |  char_class_lower,        // 'Â'  229
   char_class_alpha |  char_class_lower,        // 'Ê'  230
   char_class_alpha |  char_class_lower,        // 'Á'  231
   char_class_alpha |  char_class_lower,        // 'Ë'  232
   char_class_alpha |  char_class_lower,        // 'È'  233
   char_class_alpha |  char_class_lower,        // 'Í'  234
   char_class_alpha |  char_class_lower,        // 'Î'  235
   char_class_alpha |  char_class_lower,        // 'Ï'  236
   char_class_alpha |  char_class_lower,        // 'Ì'  237
   char_class_alpha |  char_class_lower,        // 'Ó'  238
   char_class_alpha |  char_class_lower,        // 'Ô'  239
   char_class_alpha |  char_class_lower,        // ''  240
   char_class_alpha |  char_class_lower,        // 'Ò'  241
   char_class_alpha |  char_class_lower,        // 'Ú'  242
   char_class_alpha |  char_class_lower,        // 'Û'  243
   char_class_alpha |  char_class_lower,        // 'Ù'  244
   char_class_alpha |  char_class_lower,        // 'ı'  245
   char_class_alpha |  char_class_lower,        // 'ˆ'  246
   char_class_punct,        // '˜'  247
   char_class_alpha |  char_class_lower,        // '¯'  248
   char_class_alpha |  char_class_lower,        // '˘'  249
   char_class_alpha |  char_class_lower,        // '˙'  250
   char_class_alpha |  char_class_lower,        // '˚'  251
   char_class_alpha |  char_class_lower,        // '¸'  252
   char_class_alpha |  char_class_lower,        // '˝'  253
   char_class_alpha |  char_class_lower,        // '˛'  254
   char_class_alpha |  char_class_lower,        // 'ˇ'  255
};

JM_IX_DECL wchar_t re_lower_case_map_w[] = {
   0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf,
   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 
   0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
   0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 
   0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 
   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 
   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 
   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 
   0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 
   0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 
   0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 
   0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 
   0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 
   0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xd7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xdf, 
   0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 
   0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 
};


JM_IX_DECL wchar_t re_zero_w;
JM_IX_DECL wchar_t re_ten_w;

JM_IX_DECL re_initialiser<wchar_t> init_w;

unsigned int nlsw_count = 0;
re_str<char>* wlocale_name = 0;
bool isPlatformNT = false;

struct syntax_map
{
   wchar_t c;
   unsigned int type;
};

re_list<syntax_map, jm_def_alloc>* syntax;

JM_IX_DECL unsigned int RE_CALL re_get_syntax_type(wchar_t c)
{
   JM_GUARD_STACK
   re_list<syntax_map, jm_def_alloc>::const_iterator first, last;
   first = syntax->begin();
   last = syntax->end();
   while(first != last)
   {
      if((*first).c == c)
         return (*first).type;
      ++first;
   }
   return 0;
}

JM_IX_DECL void RE_CALL re_init_w()
{
   JM_GUARD_STACK
   re_init_threads();
#ifdef JM_THREADS
   cs_guard g(*p_re_lock);
#endif
   re_message_init();
   re_init_classes();
   re_init_collate();
   if(nlsw_count == 0)
   {
      wlocale_name = new re_str<char>("xxxxxxxxxxxxxxxx");
      syntax = new re_list<syntax_map, jm_def_alloc>();
#ifdef RE_LOCALE_W32
      OSVERSIONINFO VersionInformation = {0};
      VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      GetVersionEx(&VersionInformation);
      if(VersionInformation.dwPlatformId == VER_PLATFORM_WIN32_NT)
         isPlatformNT = true;
#endif
   }
   ++nlsw_count;
}

JM_IX_DECL void RE_CALL re_update_w()
{
   JM_GUARD_STACK
#ifdef JM_THREADS
   cs_guard g(*p_re_lock);
#endif
   re_message_update();
   re_update_classes();
   re_update_collate();
   re_str<char> l(get_global_locale_name(LC_CTYPE));
   if(*wlocale_name != l)
   {
      *wlocale_name = l;
      re_str<wchar_t> s;
      const wchar_t* p = L"zero";
      if(re_lookup_collate(s, p, p+4))
      {
         jm_assert(s.size() == 1);
         re_zero_w = *s.c_str();
      }
      else
         re_zero_w = L'0';

      p = L"ten";
      if(re_lookup_collate(s, p, p+3))
      {
         jm_assert(s.size() == 1);
         re_ten_w = *s.c_str();
      }
      else
         re_ten_w = L'a';

      unsigned int i;
      wchar_t buf[256];
      syntax_map sm;
      syntax->clear();
      for(i = 1; i < syntax_max; ++i)
      {
         wchar_t* ptr = buf;
         re_get_message((wchar_t*)buf, 256, i+100);
         for(; *ptr; ++ptr)
         {
            sm.c = *ptr;
            sm.type = i;
            syntax->add(sm);
         }
      }
   }
}

JM_IX_DECL void RE_CALL re_free_w()
{
   JM_GUARD_STACK
#ifdef JM_THREADS
   cs_guard g(*p_re_lock);
#endif
   --nlsw_count;
   re_message_free();
   re_free_classes();
   re_free_collate();
   if(nlsw_count == 0)
   {
      // cleanup:
      delete wlocale_name;
      delete syntax;
   }
#ifdef JM_THREADS
   g.acquire(false);
   re_free_threads();
#endif
}

JM_IX_DECL bool RE_CALL re_iswclass(wchar_t c, jm_uintfast32_t f)
{
   JM_GUARD_STACK
   if(c < 256)
      return JM_MAKE_BOOL(re_unicode_classes[c] & f);
#ifdef RE_LOCALE_W32
   WORD mask;
   if(isPlatformNT && GetStringTypeW(CT_CTYPE1, &c, 1, &mask))
      return JM_MAKE_BOOL(mask & f);
   else if(f & char_class_unicode)
      return true;
   else if((f & char_class_graph) == char_class_graph)
      return true;
   return false;
#else
   #ifdef JM_NO_WCSTRING
   if(f & char_class_unicode)
      return true;
   else if((f & char_class_graph) == char_class_graph)
      return true;
   else if((f & char_class_print) == char_class_print)
      return true;
   return false;
   #else
   if((f & char_class_alpha) && iswalpha(c))
      return true;
   if((f & char_class_cntrl) && iswcntrl(c))
      return true;
   if((f & char_class_digit) && iswdigit(c))
      return true;
   if((f & char_class_lower) && iswlower(c))
      return true;
   if((f & char_class_punct) && iswpunct(c))
      return true;
   if((f & char_class_space) && iswspace(c))
      return true;
   if((f & char_class_upper) && iswupper(c))
      return true;
   if((f & char_class_xdigit) && iswxdigit(c))
      return true;
   if(f & char_class_unicode)
      return true;
   return false;
   #endif
#endif
}

JM_IX_DECL wchar_t RE_CALL re_wtolower(wchar_t c)
{
   JM_GUARD_STACK
   if(c < 256)
      return re_lower_case_map_w[c];
#ifdef RE_LOCALE_W32
   if(isPlatformNT)
      return LOWORD(CharLowerW((wchar_t*)(size_t)(MAKELONG(c, 0))));
   return c;
#elif !defined(JM_NO_WCSTRING)
   return towlower(c);
#else
   return c;
#endif
}

JM_IX_DECL void RE_CALL re_transform(re_str<wchar_t>& out, const re_str<wchar_t>& in)
{
   JM_GUARD_STACK
#ifdef RE_LOCALE_C
   size_t n = wcsxfrm(0, in.c_str(), 0);
#else
   auto_array<char> alt;
   size_t n;
   if(isPlatformNT)
      n = LCMapStringW(GetUserDefaultLCID(), LCMAP_SORTKEY, in.c_str(), -1, 0, 0);
   else
   {
      n = re_strnarrow((char*)0, 0, in.c_str());
      alt = new char[n+1];
      re_strnarrow((char*)alt, (unsigned int)(n+1), in.c_str());
      n = LCMapStringA(GetUserDefaultLCID(), LCMAP_SORTKEY, (char*)alt, -1, 0, 0);
   }
#endif
   if(n == TRANSFORM_ERROR)
   {
      out = in;
      return;
   }
   auto_array<wchar_t> buf(new wchar_t[n+1]);
#ifdef RE_LOCALE_C
   n = wcsxfrm((wchar_t*)buf, in.c_str(), n+1);
#else
   // under win32 we get mapped to an array of bytes
   // not characters; since the underlying engine has to
   // deal with chars we widen the bytes to wchar_t to ensure
   // the sort order remains unchanged when we compare.
   auto_array<char> t(new char[n+1]);
   if(isPlatformNT)
      n = LCMapStringW(GetUserDefaultLCID(), LCMAP_SORTKEY, in.c_str(), -1, (wchar_t*)(char*)t, (int)n);
   else
      n = LCMapStringA(GetUserDefaultLCID(), LCMAP_SORTKEY, (char*)alt, -1, (char*)t, (int)n);
   int i = -1;
   do
   {
      ++i;
      buf[i] = (wchar_t)(unsigned char)(char*)t[i];
   } while(t[i]);
#endif
   if(n == TRANSFORM_ERROR)
   {
      out = in;
      return;
   }
   out = (wchar_t*)buf;
}


#endif

//
// re_is_combining
// returns true if the character is a unicode combining character
//

const wchar_t ranges[] = { 0x0300, 0x0361, 
                           0x0483, 0x0486, 
                           0x0903, 0x0903, 
                           0x093E, 0x0940, 
                           0x0949, 0x094C,
                           0x0982, 0x0983,
                           0x09BE, 0x09C0,
                           0x09C7, 0x09CC,
                           0x09D7, 0x09D7,
                           0x0A3E, 0x0A40,
                           0x0A83, 0x0A83,
                           0x0ABE, 0x0AC0,
                           0x0AC9, 0x0ACC,
                           0x0B02, 0x0B03,
                           0x0B3E, 0x0B3E,
                           0x0B40, 0x0B40,
                           0x0B47, 0x0B4C,
                           0x0B57, 0x0B57,
                           0x0B83, 0x0B83,
                           0x0BBE, 0x0BBF,
                           0x0BC1, 0x0BCC,
                           0x0BD7, 0x0BD7,
                           0x0C01, 0x0C03,
                           0x0C41, 0x0C44,
                           0x0C82, 0x0C83,
                           0x0CBE, 0x0CBE,
                           0x0CC0, 0x0CC4,
                           0x0CC7, 0x0CCB,
                           0x0CD5, 0x0CD6,
                           0x0D02, 0x0D03,
                           0x0D3E, 0x0D40,
                           0x0D46, 0x0D4C,
                           0x0D57, 0x0D57,
                           0x0F7F, 0x0F7F,
                           0x20D0, 0x20E1, 
                           0x3099, 0x309A,
                           0xFE20, 0xFE23, 
                           0xffff, 0xffff, };

JM_IX_DECL bool RE_CALL re_is_combining(wchar_t c)
{
   JM_GUARD_STACK
   const wchar_t* p = ranges + 1;
   while(*p < c) p += 2;
   --p;
   if((c >= *p) && (c <= *(p+1)))
         return true;
   return false;
}

JM_END_NAMESPACE

#endif


