#include <stddef.h>
#include <stdint.h>

#include <nnpack/arm_neon.h>


void nnp_s4gemm_only_3x3__neon(
	size_t k, size_t update,
	const float a[restrict static 1],
	const float b[restrict static 1],
	float c[restrict static 1],
	size_t row_stride_c)
{
	float32x4_t acc00 = vdupq_n_f32(0.0f), acc01 = vdupq_n_f32(0.0f), acc02 = vdupq_n_f32(0.0f);
	float32x4_t acc10 = vdupq_n_f32(0.0f), acc11 = vdupq_n_f32(0.0f), acc12 = vdupq_n_f32(0.0f);
	float32x4_t acc20 = vdupq_n_f32(0.0f), acc21 = vdupq_n_f32(0.0f), acc22 = vdupq_n_f32(0.0f);
	do {
		const float32x4_t a0 = vld1q_f32_aligned(a + 0);
		const float32x4_t a1 = vld1q_f32_aligned(a + 4);
		const float32x4_t a2 = vld1q_f32_aligned(a + 8);

		const float32x4_t b0 = vld1q_f32_aligned(b +  0);
		acc00 = vmuladdq_f32(acc00, a0, b0);
		acc10 = vmuladdq_f32(acc10, a1, b0);
		acc20 = vmuladdq_f32(acc20, a2, b0);
		const float32x4_t b1 = vld1q_f32_aligned(b +  4);
		acc01 = vmuladdq_f32(acc01, a0, b1);
		acc11 = vmuladdq_f32(acc11, a1, b1);
		acc21 = vmuladdq_f32(acc21, a2, b1);
		const float32x4_t b2 = vld1q_f32_aligned(b +  8);
		acc02 = vmuladdq_f32(acc02, a0, b2);
		acc12 = vmuladdq_f32(acc12, a1, b2);
		acc22 = vmuladdq_f32(acc22, a2, b2);

		a += 12;
		b += 12;
	} while (--k);

	if (update != 0) {
		vst1q_f32_aligned(c +  0, vaddq_f32(vld1q_f32_aligned(c +  0), acc00));
		vst1q_f32_aligned(c +  4, vaddq_f32(vld1q_f32_aligned(c +  4), acc01));
		vst1q_f32_aligned(c +  8, vaddq_f32(vld1q_f32_aligned(c +  8), acc02));
		c += row_stride_c;
		vst1q_f32_aligned(c +  0, vaddq_f32(vld1q_f32_aligned(c +  0), acc10));
		vst1q_f32_aligned(c +  4, vaddq_f32(vld1q_f32_aligned(c +  4), acc11));
		vst1q_f32_aligned(c +  8, vaddq_f32(vld1q_f32_aligned(c +  8), acc12));
		c += row_stride_c;
		vst1q_f32_aligned(c +  0, vaddq_f32(vld1q_f32_aligned(c +  0), acc20));
		vst1q_f32_aligned(c +  4, vaddq_f32(vld1q_f32_aligned(c +  4), acc21));
		vst1q_f32_aligned(c +  8, vaddq_f32(vld1q_f32_aligned(c +  8), acc22));
	} else {
		vst1q_f32_aligned(c +  0, acc00);
		vst1q_f32_aligned(c +  4, acc01);
		vst1q_f32_aligned(c +  8, acc02);
		c += row_stride_c;
		vst1q_f32_aligned(c +  0, acc10);
		vst1q_f32_aligned(c +  4, acc11);
		vst1q_f32_aligned(c +  8, acc12);
		c += row_stride_c;
		vst1q_f32_aligned(c +  0, acc20);
		vst1q_f32_aligned(c +  4, acc21);
		vst1q_f32_aligned(c +  8, acc22);
	}
}

void nnp_s4gemm_upto_3x3__neon(
	uint32_t mr, uint32_t nr,
	size_t k, size_t update,
	const float a[restrict static 1],
	const float b[restrict static 1],
	float c[restrict static 1],
	size_t row_stride_c)
{
	float32x4_t acc00 = vdupq_n_f32(0.0f), acc01 = vdupq_n_f32(0.0f), acc02 = vdupq_n_f32(0.0f);
	float32x4_t acc10 = vdupq_n_f32(0.0f), acc11 = vdupq_n_f32(0.0f), acc12 = vdupq_n_f32(0.0f);
	float32x4_t acc20 = vdupq_n_f32(0.0f), acc21 = vdupq_n_f32(0.0f), acc22 = vdupq_n_f32(0.0f);
	do {
		float32x4_t a0, a1, a2;

		a0 = vld1q_f32_aligned(a);
		a += 4;
		if (mr > 1) {
			a1 = vld1q_f32_aligned(a);
			a += 4;
			if (mr > 2) {
				a2 = vld1q_f32_aligned(a);
				a += 4;
			}
		}

		const float32x4_t b0 = vld1q_f32_aligned(b);
		b += 4;
		acc00 = vmuladdq_f32(acc00, a0, b0);
		acc10 = vmuladdq_f32(acc10, a1, b0);
		acc20 = vmuladdq_f32(acc20, a2, b0);
		if (nr > 1) {
			const float32x4_t b1 = vld1q_f32_aligned(b);
			b += 4;
			acc01 = vmuladdq_f32(acc01, a0, b1);
			acc11 = vmuladdq_f32(acc11, a1, b1);
			acc21 = vmuladdq_f32(acc21, a2, b1);
			if (nr > 2) {
				const float32x4_t b2 = vld1q_f32_aligned(b);
				b += 4;
				acc02 = vmuladdq_f32(acc02, a0, b2);
				acc12 = vmuladdq_f32(acc12, a1, b2);
				acc22 = vmuladdq_f32(acc22, a2, b2);
			}
		}
	} while (--k);

	if (update != 0) {
		vst1q_f32_aligned(c, vaddq_f32(vld1q_f32_aligned(c), acc00));
		if (nr > 1) {
			vst1q_f32_aligned(c + 4, vaddq_f32(vld1q_f32_aligned(c + 4), acc01));
			if (nr > 2) {
				vst1q_f32_aligned(c + 8, vaddq_f32(vld1q_f32_aligned(c + 8), acc02));
			}
		}
		if (mr > 1) {
			c += row_stride_c;
			vst1q_f32_aligned(c, vaddq_f32(vld1q_f32_aligned(c), acc10));
			if (nr > 1) {
				vst1q_f32_aligned(c + 4, vaddq_f32(vld1q_f32_aligned(c + 4), acc11));
				if (nr > 2) {
					vst1q_f32_aligned(c + 8, vaddq_f32(vld1q_f32_aligned(c + 8), acc12));
				}
			}
			if (mr > 2) {
				c += row_stride_c;
				vst1q_f32_aligned(c, vaddq_f32(vld1q_f32_aligned(c), acc20));
				if (nr > 1) {
					vst1q_f32_aligned(c + 4, vaddq_f32(vld1q_f32_aligned(c + 4), acc21));
					if (nr > 2) {
						vst1q_f32_aligned(c + 8, vaddq_f32(vld1q_f32_aligned(c + 8), acc22));
					}
				}
			}
		}
	} else {
		vst1q_f32_aligned(c, acc00);
		if (nr > 1) {
			vst1q_f32_aligned(c + 4, acc01);
			if (nr > 2) {
				vst1q_f32_aligned(c + 8, acc02);
			}
		}
		if (mr > 1) {
			c += row_stride_c;
			vst1q_f32_aligned(c, acc10);
			if (nr > 1) {
				vst1q_f32_aligned(c + 4, acc11);
				if (nr > 2) {
					vst1q_f32_aligned(c + 8, acc12);
				}
			}
			if (mr > 2) {
				c += row_stride_c;
				vst1q_f32_aligned(c, acc20);
				if (nr > 1) {
					vst1q_f32_aligned(c + 4, acc21);
					if (nr > 2) {
						vst1q_f32_aligned(c + 8, acc22);
					}
				}
			}
		}
	}
}
