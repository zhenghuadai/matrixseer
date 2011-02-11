

//#include "inttypes.h"

#include "mpeg2.h"
//#include "attributes.h"
#include "mpeg2_internal.h"
mpeg2_mc_t mpeg2_mc;

	void setConstant_sse2();
void mpeg2_mc_init (uint32_t accel)
{
    if (accel & MPEG2_ACCEL_X86_MMXEXT){
	}
    else if (accel & MPEG2_ACCEL_X86_3DNOW){
	}
    else if (accel & MPEG2_ACCEL_X86_MMX){
	}
    else
	mpeg2_mc = mpeg2_mc_c;
	#if 1 
	mpeg2_mc = mpeg2_mc_sse2;
	setConstant_sse2();
	#endif
}

#define avg2(a,b) ((a+b+1)>>1)
#define avg4(a,b,c,d) ((a+b+c+d+2)>>2)

#define predict_o(i) (ref[i])
#define predict_x(i) (avg2 (ref[i], ref[i+1]))
#define predict_y(i) (avg2 (ref[i], (ref+stride)[i]))
#define predict_xy(i) (avg4 (ref[i], ref[i+1], \
			     (ref+stride)[i], (ref+stride)[i+1]))

#define put(predictor,i) dest[i] = predictor (i)
#define avg(predictor,i) dest[i] = avg2 (predictor (i), dest[i])

/* mc function template */

#define MC_FUNC(op,xy)							\
static void MC_##op##_##xy##_16_c (uint8_t * dest, const uint8_t * ref,	\
				   const int stride, int height)	\
{									\
    do {								\
	op (predict_##xy, 0);						\
	op (predict_##xy, 1);						\
	op (predict_##xy, 2);						\
	op (predict_##xy, 3);						\
	op (predict_##xy, 4);						\
	op (predict_##xy, 5);						\
	op (predict_##xy, 6);						\
	op (predict_##xy, 7);						\
	op (predict_##xy, 8);						\
	op (predict_##xy, 9);						\
	op (predict_##xy, 10);						\
	op (predict_##xy, 11);						\
	op (predict_##xy, 12);						\
	op (predict_##xy, 13);						\
	op (predict_##xy, 14);						\
	op (predict_##xy, 15);						\
	ref += stride;							\
	dest += stride;							\
    } while (--height);							\
}									\
static void MC_##op##_##xy##_8_c (uint8_t * dest, const uint8_t * ref,	\
				  const int stride, int height)		\
{									\
    do {								\
	op (predict_##xy, 0);						\
	op (predict_##xy, 1);						\
	op (predict_##xy, 2);						\
	op (predict_##xy, 3);						\
	op (predict_##xy, 4);						\
	op (predict_##xy, 5);						\
	op (predict_##xy, 6);						\
	op (predict_##xy, 7);						\
	ref += stride;							\
	dest += stride;							\
    } while (--height);							\
}
/* definitions of the actual mc functions */
MC_FUNC (put,o)
MC_FUNC (avg,o)
MC_FUNC (put,x)
MC_FUNC (avg,x)
MC_FUNC (put,y)
MC_FUNC (avg,y)
MC_FUNC (put,xy)
MC_FUNC (avg,xy)

MPEG2_MC_EXTERN (c)
