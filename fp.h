// fp.h

// The FPNumber class splits a floating point number into the S, E, and F fields.
// The F field will actually be 26 bits long, as this automatically adds the leading 1
// and the two guard bits.
//
// To use:
//		FPNumber fa(a);
//			This allocates a new FPNumber and loads it from the value a.  Note that a
//			will be an integer, but it has the bit-pattern of a floating point number.
//
//		fa.s()
//			This returns the sign of the number.  It will be either 1 or -1.
//
//		fa.e()
//			This gives the exponent, a number that should be between 0 and 255.
//
//		fa.f()
//			This gives the mantissa.  It will have the leading 1, and it also has
//			two guard bits, so it is 26 bits long, not 23!
//
//		fa.setS(val)
//			This changes the sign
//
//		fa.setE(val)
//			This changes the exponent
//
//		fa.setF(val)
//			This changes the mantissa
//
//		fa.isNaN()
//			returns true if the value is a NaN
//
//		fa.isInfinity()
//			returns true if the value is infinity, either + or -
//
//		fa.isZero()
//			returns true if the value is zero, either + or -
//
//		fa.asInt()
//			This packs the data back into an integer using the floating point bit pattern.
class FPNumber
{
    int _s, _e;
    long long _f;	// Will be 26 bits long, has the leading 1 and 2 guard bits
  public:
    FPNumber(int a)
    {
      _s = (((a >> 31) & 1) > 0) ? -1 : 1;
      _e = (a >> 23) & 0xFF;
      _f = a & 0x7FFFFF;
      if (_e != 0 && _e != 0xFF)
      {
        _f |= 0x800000;
      }
	  _f <<= 2;
    }

    ~FPNumber()
    {
      ; // EMPTY
    }

    inline int s(void) { return _s; }
    inline int e(void) { return _e; }
    inline long long f(void) { return _f; }
    inline void setS(int s) { _s = s; }
    inline void setE(int e) { _e = e; }
    inline void setF(long long f) { _f = f; }

    bool isNaN(void) { return _e == 0xFF && _f > 0; }
    bool isInfinity(void) { return _e == 0xFF && _f == 0; }
    bool isZero(void) { return _e == 0 && _f == 0; }

	int asInt(void)
	{
		return ((_s == -1) ? 0x80000000 : 0) | (_e << 23) | ((_f >> 2) & 0x7FFFFF);
	}
};

// You need to implement this class:
class fp
{
public:
  // This should return your name (so we know whose homework this is).
  // Give your name without spaces, such as "DaveJohannsen"
  char *myName(void);

  // This should simulate the IEEE 754 floating point add
  int add(int a, int b);

  // This should simulate the IEEE 754 floating point multiply
  int mul(int a, int b);

private:
  void swap(FPNumber &fa, FPNumber &fb);
  void normalize(int &exponent, long long &mantissa);
};
