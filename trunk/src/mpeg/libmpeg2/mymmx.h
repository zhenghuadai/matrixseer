
void mpeg2_idct_add_mmxext (const int last, int16_t * const block,
             uint8_t * const dest, const int stride);
void mpeg2_idct_mmx_init (void);
void mpeg2_idct_copy_mmxext (int16_t * const block, uint8_t * const dest,
              const int stride);




void mpeg2_idct_add_sse2 (const int last, int16_t * const block,
             uint8_t * const dest, const int stride);
void mpeg2_idct_sse2_init (void);
void mpeg2_idct_copy_sse2 (int16_t * const block, uint8_t * const dest,
              const int stride);




