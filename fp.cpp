// fp.cpp

#include "fp.h"

char *fp::myName(void)
{
	return "JustinHa";
}

int fp::add(int a, int b)
{
	FPNumber fa(a);
	FPNumber fb(b);
	FPNumber result(0);

	//Exception checking
	//If either is NaN, return NaN(just return the input)
	if (fa.isNaN()){
		return a;
	}
	if (fb.isNaN()){
		return b;
	}
	//If either input is 0, return the other nonzero
	if (fa.isZero()) {
		return b;
	}
	if (fb.isZero()) {
		return a;
	}
	//If both inputs are infinity:
	if (fa.isInfinity() && fb.isInfinity()){
		//	If signs are same, return same sign infinity (return the input)
		if (fa.s() == fb.s()) {
			return b;
		} else { //	If signs are different, return NaN
			result.setE(255);
			result.setF(1);
			return result.asInt();
		}
	}
	//If either input is infinity, return infinity
	if (fa.isInfinity()) {
		return a;
	} else if (fb.isInfinity()) {
		return b;
	}//Done exception checking

	//Sort A and B
	if (fa.e() < fb.e()) {
		swap(fa, fb);
	} else if (fa.e() == fb.e() && fa.f() < fb.f()) {
		swap(fa, fb);
	}
	//If denormalized, set exponent to 1
	if (fa.e() == 0) {
		fa.setE(1);
	}
	if (fb.e() == 0) {
		fb.setE(1);
	}
	//Align decimal point
	if (fa.e() != fb.e()) {
		int change = fa.e() - fb.e();
		if (change > 24) {
			return a;
		} else {
			fb.setF(fb.f() >> change);
			fb.setE(fb.e() + change);
		}
	}
	//Add or Subtract
	result.setS(fa.s());
	if (fa.s() == fb.s()) {	//addition
		int exponent = fa.e();

		long long mantissa = fa.f() + fb.f();
		long long check27bit = 0x4000000;			//hexad representation of binary 1000000... (27 bits)
		if ((mantissa & check27bit) == check27bit) {	//checks if the addition set the 27th bit
			mantissa >>= 1;
			exponent++;
		}
		if (exponent == 255) {	//check for overflow
			result.setE(255);
			result.setF(0);
		} else {	//normalize mantissa
			normalize(exponent, mantissa);
			result.setE(exponent);
			result.setF(mantissa);

		}
	} else {	//subtraction
		int exponent = fa.e();
		long long mantissa = fa.f() - fb.f();
		if (mantissa == 0){	//check if 0
			result.setE(0);
			result.setF(0);
		} else {	//normalize mantissa
			normalize(exponent, mantissa);
			result.setE(exponent);
			result.setF(mantissa);
		}
	}

	return result.asInt();
}

int fp::mul(int a, int b)
{
	FPNumber fa(a);
	FPNumber fb(b);
	FPNumber result(0);
	//XOR the sign bits
	result.setS(fa.s() == fb.s() ? 1 : -1);

	//Exception checking
	//If either is NaN, return NaN(just return the input)
	if (fa.isNaN()){
		return a;
	}
	if (fb.isNaN()){
		return b;
	}
	//If one input is zero and the other is infinity, return NaN
	if (fa.isZero() && fb.isInfinity() || fa.isInfinity() && fb.isZero()) {
		result.setE(255);
		result.setF(1);
		return result.asInt();
	}
	//If either input is zero, return 0
	if (fa.isZero() or fb.isZero()) {
		result.setE(0);
		result.setF(0);
		return result.asInt();
	}
	//If either input is infinity, return infinity
	if (fa.isInfinity() or fb.isInfinity()) {
		result.setE(255);
		result.setF(0);
		return result.asInt();
	}//Done exception checking

	//Add exponents
	int exponent = fa.e() + fb.e() - 127;	
	if (exponent > 254) {	//check for overflow
		result.setE(255);
		result.setF(0);
		return result.asInt();
	}
	if (exponent < 0) {	//check for underflow
		result.setE(0);
		result.setF(0);
		return result.asInt();
	}
	long long mantissa = fa.f() * fb.f();
	mantissa >>= 26;	//shift 25 bits right to get a 27 bit mantissa, then normalize to 26 bits
	exponent++;			//by shifting once more to the right and incrementing exponent
	if (exponent == 255) {	//check for overflow
		result.setE(255);
		result.setF(0);
	}
	normalize(exponent, mantissa);
	result.setE(exponent);
	result.setF(mantissa);

	return result.asInt();
}

void fp::swap(FPNumber &fa, FPNumber &fb) {
	FPNumber temp = fa;
	fa = fb;
	fb = temp;
}

void fp::normalize(int &exponent, long long &mantissa) {
	long long check26bit = 0x2000000;	//hexad representation of binary 1000000... (26 bits)
	while ((mantissa & check26bit) != check26bit) {	//while 26th bit is not set
		mantissa <<= 1;
		if (--exponent == 0) {
			mantissa >>= 1;
			break;
		}
	}
}