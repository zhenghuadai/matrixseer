/*	
 * Copyright (c) 2006
 *	Jim Huang <jserv.tw@gmail.com>
 *
 *  This Program is free software; you can redistribute it and/or modify	
 *  it under the terms of the GNU General Public License as published by	
 *  the Free Software Foundation; either version 2, or (at your option)	
 *  any later version.	
 *	
 *  This Program is distributed in the hope that it will be useful,	
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of	
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the	
 *  GNU General Public License for more details.	
 *	
 *  You should have received a copy of the GNU General Public License	
 *  along with GNU Make; see the file COPYING.  If not, write to	
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.	
 *  http://www.gnu.org/copyleft/gpl.html	
 *	
 *  Based on Intel's AP-942	
 */

#include "config.h"

#if defined(HAVE_BUILTIN_VECTOR)

#if defined(ARCH_X86) || defined(ARCH_X86_64)
#ifdef HAVE_SSE2

#include <inttypes.h>
#include "mpeg2.h"
#include "attributes.h"
#include "mpeg2_internal.h"

#ifdef __GNUC__
  #ifndef __forceinline
    #define __forceinline __attribute__((__always_inline__)) inline
  #endif
#endif

#ifdef __GNUC__
  #define __inline __forceinline  // GCC needs to force inlining of intrinsics functions
#endif

#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

#ifdef __GNUC__
  #undef __inline
#endif

#ifdef __GNUC__
  #define __align8(t,v) t v __attribute__ ((aligned (8)))
  #define __align16(t,v) t v __attribute__ ((aligned (16)))
#else
  #define __align8(t,v) __declspec(align(8)) t v
  #define __align16(t,v) __declspec(align(16)) t v
#endif

static __m128i const_1_16_bytes;
static void __attribute__((constructor)) mpeg2_MC_sse_ctor()
{
	const_1_16_bytes = _mm_set1_epi16(1);
}

static void MC_put_o_16_sse2(uint8_t* ecx, const uint8_t* edx, const int eax, int esi)
{
	const int edi = eax + eax;
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		__m128i xmm0, xmm1;
		xmm0 = _mm_loadu_si128((__m128i*) edx);
		xmm1 = _mm_loadu_si128((__m128i*) (edx + eax));
		_mm_store_si128((__m128i*) ecx, xmm0);
		_mm_store_si128((__m128i*) (ecx + eax), xmm1);
	}
}

static void MC_put_o_8_sse2(uint8_t* ecx, const uint8_t* edx, const int eax, int esi)
{
	const int edi = eax + eax;
	for (; esi; edx += edi, ecx += edi, esi-= 2) {
		__m128d xmm0;
		xmm0 = _mm_loadl_pd(xmm0, (double*) edx);
		xmm0 = _mm_loadh_pd(xmm0, (double*) (edx + eax));
		_mm_storel_pd((double*) ecx, xmm0);
		_mm_storeh_pd((double*) (ecx + eax), xmm0);
	}
}

static void MC_put_x_16_sse2(uint8_t* ecx, const uint8_t* edx, const int eax, int esi)
{
	const int edi= eax + eax;
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		__m128i xmm0, xmm1, xmm2, xmm3;
		xmm0 = _mm_loadu_si128((__m128i*) edx);
		xmm1 = _mm_loadu_si128((__m128i*) (edx + 1));
		xmm2 = _mm_loadu_si128((__m128i*) (edx + eax));
		xmm3 = _mm_loadu_si128((__m128i*) (edx + eax + 1));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm2 = _mm_avg_epu8(xmm2, xmm3);
		_mm_store_si128((__m128i*) ecx, xmm0);
		_mm_store_si128((__m128i*) (ecx + eax), xmm2);
	}
}

static void MC_put_x_8_sse2(uint8_t* ecx, const uint8_t* edx, const int eax, int esi)
{
	const int edi = eax + eax;
	__m128i xmm0,xmm1;
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		xmm0 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm0), (double*) edx));
		xmm1 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm1), (double*) (edx + 1)));
		xmm0 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm0), (double*) (edx + eax)));
		xmm1 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm1), (double*) (edx + eax + 1)));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		_mm_storel_pd((double*) ecx, _mm_castsi128_pd(xmm0));
		_mm_storeh_pd((double*) (ecx + eax), _mm_castsi128_pd(xmm0));
	}
}

static void MC_put_y_16_sse2(uint8_t* ecx, const uint8_t* edx, const int eax, int esi)
{
	const int edi= eax + eax;
	__m128i xmm0;
	xmm0 = _mm_loadu_si128((__m128i*) edx);
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		__m128i xmm1,xmm2;
		xmm1 = _mm_loadu_si128((__m128i*) (edx + eax));
		xmm2 = _mm_loadu_si128((__m128i*) (edx + edi));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm1 = _mm_avg_epu8(xmm1, xmm2);
		_mm_store_si128((__m128i*) ecx, xmm0);
		xmm0 = xmm2;
		_mm_store_si128((__m128i*) (ecx + eax), xmm1);
	}
}

static void MC_put_y_8_sse2(uint8_t* ecx, const uint8_t* edx, const int eax, int esi)
{
	const int edi = eax + eax;
	__m128i xmm0;
	xmm0 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm0), (double*) edx));
	xmm0 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm0), (double*) (edx + eax)));
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		__m128i xmm1;
		xmm1 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm1), (double*) (edx + eax)));
		xmm1 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm1), (double*) (edx + edi)));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		_mm_storeh_pd((double*) ecx, _mm_castsi128_pd(xmm0));
		_mm_storel_pd((double*) (ecx + eax), _mm_castsi128_pd(xmm0));
		xmm0 = xmm1;
	}
}

static void MC_put_xy_16_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int eax = stride;
	int esi = height;
	int edi = eax + eax;
	__m128i xmm7, xmm0, xmm1, xmm4, xmm5, xmm2, xmm3;
	xmm7 = const_1_16_bytes;
	xmm0 = _mm_loadu_si128((__m128i*) edx);
	xmm1 = _mm_loadu_si128((__m128i*) (edx + 1));
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		xmm2 = _mm_loadu_si128((__m128i*) (edx + eax));
		xmm3 = _mm_loadu_si128((__m128i*) (edx + eax + 1));
		xmm4 = _mm_loadu_si128((__m128i*) (edx + edi));
		xmm5 = _mm_loadu_si128((__m128i*) (edx + edi + 1));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm2 = _mm_avg_epu8(xmm2, xmm3);
		xmm1 = xmm5;
		xmm5 = _mm_avg_epu8(xmm5, xmm4);
		xmm2 = _mm_subs_epu8(xmm2, xmm7);
		xmm0 = _mm_avg_epu8(xmm0, xmm2);
		xmm2 = _mm_avg_epu8(xmm2, xmm5);
		_mm_store_si128((__m128i*) ecx, xmm0);
		xmm0 = xmm4;
		_mm_store_si128((__m128i*) (ecx + eax), xmm2);
	}       
}

static void MC_put_xy_8_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int eax = stride;
	int esi = height;
	int edi = eax + eax;
	__m128i xmm7, xmm0, xmm2, xmm1, xmm3;
	xmm7 = const_1_16_bytes;
	xmm0 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm0), (double*) edx));
	xmm0 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm0), (double*) (edx + eax)));
	xmm2 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm2), (double*) (edx + 1)));
	xmm2 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm2), (double*) (edx + eax + 1)));
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		xmm1 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm1), (double*) (edx + eax)));
		xmm1 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm1), (double*) (edx + edi)));
		xmm3 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm3), (double*) (edx + eax + 1)));
		xmm3 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm3), (double*) (edx + edi + 1)));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm2 = _mm_avg_epu8(xmm2, xmm3);
		xmm0 = _mm_subs_epu8(xmm0, xmm7);
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		_mm_storeh_pd((double*) ecx, _mm_castsi128_pd(xmm0));
		_mm_storel_pd((double*) (ecx + eax), _mm_castsi128_pd(xmm0));
		xmm0 = xmm1;
		xmm2 = xmm3;
	}        
}

static void MC_avg_o_16_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int esi = height;
	int eax = stride;
	int edi = eax + eax;
	for (; esi; edx += edi, ecx += edi,esi -= 2) {
		__m128i xmm0, xmm1, xmm2, xmm3;
		xmm0 = _mm_loadu_si128((__m128i*) edx);
		xmm1 = _mm_loadu_si128((__m128i*) (edx + eax));
		xmm2 = _mm_load_si128((__m128i*) ecx);
		xmm3 = _mm_load_si128((__m128i*) (ecx + eax));
		xmm0 = _mm_avg_epu8(xmm0, xmm2);
		xmm1 = _mm_avg_epu8(xmm1, xmm3);
		_mm_store_si128((__m128i*) ecx, xmm0);
		_mm_store_si128((__m128i*) (ecx + eax), xmm1);
	}
}

static void MC_avg_o_8_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int esi = height;
	int eax = stride;
	int edi = eax + eax;
	__m128i xmm0, xmm1;
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		xmm0 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm0), (double*) edx));
		xmm0 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm0), (double*) (edx + eax)));
		xmm1 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm1), (double*) ecx));
		xmm1 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm1), (double*) (edx + eax)));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		_mm_storel_pd((double*) ecx, _mm_castsi128_pd(xmm0));
		_mm_storeh_pd((double*) (ecx + eax), _mm_castsi128_pd(xmm0));
	}
}

static void MC_avg_x_16_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int esi = height;
	int eax = stride;
	int edi = eax + eax;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5;
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		xmm0 = _mm_loadu_si128((__m128i*) edx);
		xmm1 = _mm_loadu_si128((__m128i*) (edx + 1));
		xmm2 = _mm_loadu_si128((__m128i*) (edx + eax));
		xmm3 = _mm_loadu_si128((__m128i*) (edx + eax + 1));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm2 = _mm_avg_epu8(xmm2, xmm3);
		xmm4 = _mm_load_si128((__m128i*) ecx);
		xmm5 = _mm_load_si128((__m128i*) (ecx + eax));
		xmm0 = _mm_avg_epu8(xmm0, xmm4);
		xmm2 = _mm_avg_epu8(xmm2, xmm5);
		_mm_store_si128((__m128i*) ecx, xmm0);
		_mm_store_si128((__m128i*) (ecx + eax), xmm2);
	}
}

static void MC_avg_x_8_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int esi = height;
	int eax = stride;
	int edi = eax + eax;
	__m128i xmm0, xmm1, xmm2;
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		xmm0 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm0), (double*) edx));
		xmm1 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm1), (double*) (edx + 1)));
		xmm0 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm0), (double*) (edx + eax)));
		xmm1 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm1), (double*) (edx + eax + 1)));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm2 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm2), (double*) ecx));
		xmm2 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm2), (double*) (ecx + eax)));
		xmm0 = _mm_avg_epu8(xmm0, xmm2);
		_mm_storel_pd((double*) ecx, _mm_castsi128_pd(xmm0));
		_mm_storeh_pd((double*) (ecx + eax), _mm_castsi128_pd(xmm0));
	}
}

static void MC_avg_y_16_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int esi = height;
	int eax = stride;
	int edi = eax + eax;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4;

	xmm0 = _mm_loadu_si128((__m128i*) edx);
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		xmm1 = _mm_loadu_si128((__m128i*) (edx + eax));
		xmm2 = _mm_loadu_si128((__m128i*) (edx + edi));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm1 = _mm_avg_epu8(xmm1, xmm2);
		xmm3 = _mm_load_si128((__m128i*) ecx);
		xmm4 = _mm_load_si128((__m128i*) (ecx + eax));
		xmm0 = _mm_avg_epu8(xmm0, xmm3);
		xmm1 = _mm_avg_epu8(xmm1, xmm4);
		_mm_store_si128((__m128i*) ecx, xmm0);
		xmm0 = xmm2;
		_mm_store_si128((__m128i*) (ecx + eax), xmm1);
	}
}

static void MC_avg_y_8_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int esi = height;
	int eax = stride;
	int edi = eax + eax;
	__m128i xmm0, xmm1, xmm2;
	xmm0 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm0), (double*) edx));
	xmm0 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm0), (double*) (edx + eax)));
	for (; esi; edx += edi, ecx += edi, esi -= 2) {
		xmm1 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm1), (double*) (edx + eax)));
		xmm1 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm1), (double*) (edx + edi)));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm2 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm2), (double*) ecx));
		xmm2 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm2), (double*) (ecx + eax)));
		xmm0 = _mm_avg_epu8(xmm0, xmm2);
		_mm_storeh_pd((double*) ecx, _mm_castsi128_pd(xmm0));
		_mm_storel_pd((double*) (ecx + eax), _mm_castsi128_pd(xmm0));
		xmm0 = xmm1;
	}
}

static void MC_avg_xy_16_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int esi = height;
	int eax = stride;
	int edi = eax + eax;
	__m128i xmm7, xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6;
	xmm7 = const_1_16_bytes;
	xmm0 = _mm_loadu_si128((__m128i*) edx);
	xmm1 = _mm_loadu_si128((__m128i*) (edx + 1));
	for (; esi; edx += edi, ecx += edi, esi-= 2) {
		xmm2 = _mm_loadu_si128((__m128i*) (edx + eax));
		xmm3 = _mm_loadu_si128((__m128i*) (edx + eax + 1));
		xmm4 = _mm_loadu_si128((__m128i*) (edx + edi));
		xmm5 = _mm_loadu_si128((__m128i*) (edx + edi + 1));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm2 = _mm_avg_epu8(xmm2, xmm3);
		xmm1 = xmm5;
		xmm5 = _mm_avg_epu8(xmm5, xmm4);
		xmm2 = _mm_subs_epu8(xmm2, xmm7);
		xmm0 = _mm_avg_epu8(xmm0, xmm2);
		xmm2 = _mm_avg_epu8(xmm2, xmm5);
		xmm5 = _mm_load_si128((__m128i*) ecx);
		xmm6 = _mm_load_si128((__m128i*) (ecx + eax));
		xmm0 = _mm_avg_epu8(xmm0, xmm5);
		xmm2 = _mm_avg_epu8(xmm2, xmm6);
		_mm_store_si128((__m128i*) ecx, xmm0);
		xmm0 = xmm4;
		_mm_store_si128((__m128i*) (ecx + eax), xmm2);
	}
}

static void MC_avg_xy_8_sse2(uint8_t* dest, const uint8_t* ref, const int stride, int height)
{
	const uint8_t *edx = ref;
	uint8_t *ecx = dest;
	int esi = height;
	int eax = stride;
	int edi = eax + eax;
	__m128i xmm7, xmm0, xmm2, xmm1, xmm3, xmm4;
	xmm7 = const_1_16_bytes;
	xmm0 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm0), (double*) edx));
	xmm0 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm0), (double*) (edx + eax)));
	xmm2 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm2), (double*) (edx + 1)));
	xmm2 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm2), (double*) (edx + eax + 1)));
	for (;esi;edx+=edi,ecx+=edi, esi -= 2) {
		xmm1 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm1), (double*) (edx + eax)));
		xmm1 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm1), (double*) (edx + edi)));
		xmm3 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm3), (double*) (edx + eax + 1)));
		xmm3 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm3), (double*) (edx + edi + 1)));
		xmm0 = _mm_avg_epu8(xmm0, xmm1);
		xmm2 = _mm_avg_epu8(xmm2, xmm3);
		xmm0 = _mm_subs_epu8(xmm0, xmm7);
		xmm0 = _mm_avg_epu8(xmm0, xmm2);
		xmm4 = _mm_castpd_si128(_mm_loadh_pd(_mm_castsi128_pd(xmm4), (double*) ecx));
		xmm4 = _mm_castpd_si128(_mm_loadl_pd(_mm_castsi128_pd(xmm4), (double*) (ecx + eax)));
		xmm0 = _mm_avg_epu8(xmm0, xmm4);
		_mm_storeh_pd((double*) ecx, _mm_castsi128_pd(xmm0));
		_mm_storel_pd((double*) (ecx + eax), _mm_castsi128_pd(xmm0));
		xmm0 = xmm1;
		xmm2 = xmm3;
	}
}

MPEG2_MC_EXTERN (sse2)

#endif
#endif
#endif
