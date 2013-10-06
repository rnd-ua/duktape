/*
 *  Replacements for missing platform functions.
 *
 *  Unlike the originals, fpclassify() and signbit() replacements don't
 *  work on any floating point types, only doubles.
 */

#ifdef DUK_USE_COMPUTED_NAN
double duk_computed_nan;
#endif

#ifdef DUK_USE_COMPUTED_INFINITY
double duk_computed_infinity;
#endif

#ifdef DUK_USE_REPL_FPCLASSIFY
int duk_repl_fpclassify(double x) {
	volatile duk_double_union u;
	int exp;
	int mzero;

	u.d = x;
	exp = (u.us[_DUK_IDX_US0] & 0x7ff0) >> 4;
	if (exp > 0x000 && exp < 0x7ff) {
		return DUK_FP_NORMAL;
	}

	mzero = (u.ui[_DUK_IDX_UI1] == 0 && (u.ui[_DUK_IDX_UI0] & 0x000fffff) == 0);
	if (exp == 0x000) {
		if (mzero) {
			return DUK_FP_ZERO;
		} else {
			return DUK_FP_SUBNORMAL;
		}
	} else {
		if (mzero) {
			return DUK_FP_INFINITE;
		} else {
			return DUK_FP_NAN;
		}
	}
}
#endif

#ifdef DUK_USE_REPL_SIGNBIT
int duk_repl_signbit(double x) {
	volatile duk_double_union u;
	u.d = x;
	return u.uc[_DUK_IDX_UC0] & 0x80;
}
#endif

#ifdef DUK_USE_REPL_ISFINITE
int duk_repl_isfinite(double x) {
	int c = DUK_FPCLASSIFY(x);
	if (c == DUK_FP_NAN || c == DUK_FP_INFINITE) {
		return 0;
	} else {
		return 1;
	}
}
#endif

#ifdef DUK_USE_REPL_ISNAN
int duk_repl_isnan(double x) {
	int c = DUK_FPCLASSIFY(x);
	return (c == DUK_FP_NAN);
}
#endif
