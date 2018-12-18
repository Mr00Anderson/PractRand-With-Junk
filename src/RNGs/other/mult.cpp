#include <string>
#include <sstream>
#include "PractRand/config.h"
#include "PractRand/rng_basics.h"
#include "PractRand/rng_helpers.h"
#include "PractRand/rng_internals.h"

#include "PractRand/RNGs/other/mult.h"
//#include "PractRand/test_helpers.h"

namespace PractRand {
	using namespace Internals;
	namespace RNGs {
		namespace Polymorphic {
			namespace NotRecommended {
				Uint32 lcg32of64_varqual::raw32() {
					state = state * 1103515245 + 12345;
					return Uint32(state >> outshift);
				}
				std::string lcg32of64_varqual::get_name() const {
					std::ostringstream str;
					str << "lcg(32," << (32 + outshift) << ")";
					return str.str();
				}
				void lcg32of64_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				Uint16 lcg16of64_varqual::raw16() {
					state = state * 1103515245 + 12345;
					return Uint16(state >> outshift);
				}
				std::string lcg16of64_varqual::get_name() const {
					std::ostringstream str;
					str << "lcg(16," << (16 + outshift) << ")";
					return str.str();
				}
				void lcg16of64_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				Uint8 lcg8of64_varqual::raw8() {
					state = state * 1103515245 + 12345;
					return Uint8(state >> outshift);
				}
				std::string lcg8of64_varqual::get_name() const {
					std::ostringstream str;
					str << "lcg(8," << (8 + outshift) << ")";
					return str.str();
				}
				void lcg8of64_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}

				Uint32 lcg32of128_varqual::raw32() {
					//large multiplication is much harder to write in C (than asm)
					//made some compromises here... restricting the multiplier to 32 bits
					//which would hurt quality some, being so small compared to the state
					//but I think the correct comparison is to the output window, not the state
					//which is only 16 bits, so it should all be good
					const Uint32 multiplier = 1103515245;
					const Uint64 adder = 1234567;
					Uint64 a = Uint32(low) * Uint64(multiplier);
					Uint64 b = (low >> 32) * Uint64(multiplier);
					Uint64 old = low;
					low = a + (b << 32);
					b += a >> 32;
					high *= multiplier;
					high += b >> 32;
					high += old;//adds 2**64 to the multiplier
					low += adder;
					if (a < adder) high++;
					if (outshift >= 64) return Uint32(high >> (outshift-64));
					if (outshift > 32) return Uint32( (low >> outshift) | (high << (64-outshift)) );
					return Uint32(low >> outshift);
				}
				std::string lcg32of128_varqual::get_name() const {
					std::ostringstream str;
					str << "lcg(32," << (32 + outshift) << ")";
					return str.str();
				}
				void lcg32of128_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(low);
					walker->handle(high);
				}
				Uint16 lcg16of128_varqual::raw16() {
					const Uint32 multiplier = 1103515245;
					const Uint64 adder = 1234567;
					Uint64 a = Uint32(low) * Uint64(multiplier);
					Uint64 b = (low >> 32) * Uint64(multiplier);
					Uint64 old = low;
					low = a + (b << 32);
					b += a >> 32;
					high *= multiplier;
					high += b >> 32;
					high += old;//adds 2**64 to the multiplier
					low += adder;
					if (a < adder) high++;
					if (outshift >= 64) return Uint16(high >> (outshift-64));
					if (outshift > 48) return Uint16( (low >> outshift) | (high << (64-outshift)) );
					return Uint16(low >> outshift);
				}
				std::string lcg16of128_varqual::get_name() const {
					std::ostringstream str;
					str << "lcg(16," << (16 + outshift) << ")";
					return str.str();
				}
				void lcg16of128_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(low);
					walker->handle(high);
				}
				Uint8 lcg8of128_varqual::raw8() {
					const Uint32 multiplier = 1103515245;
					const Uint64 adder = 1234567;
					Uint64 a = Uint32(low) * Uint64(multiplier);
					Uint64 b = (low >> 32) * Uint64(multiplier);
					Uint64 old = low;
					low = a + (b << 32);
					b += a >> 32;
					high *= multiplier;
					high += b >> 32;
					high += old;//adds 2**64 to the multiplier
					low += adder;
					if (a < adder) high++;
					if (outshift >= 64) return Uint8(high >> (outshift-64));
					if (outshift > 56) return Uint8( (low >> outshift) | (high << (64-outshift)) );
					return Uint8(low >> outshift);
				}
				std::string lcg8of128_varqual::get_name() const {
					std::ostringstream str;
					str << "lcg(8," << (8 + outshift) << ")";
					return str.str();
				}
				void lcg8of128_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(low);
					walker->handle(high);
				}


				//similar to lcg16of32, but with a longer period
				Uint16 lcg16of32_extended::raw16() {
					state = state * 1103515245 + add;
					if (!(state & 0x7fff)) add += 2;
					return Uint16(state >> 16);
				}
				std::string lcg16of32_extended::get_name() const {return "lcg16of32_extended";}
				void lcg16of32_extended::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					walker->handle(add);
					add |= 1;
				}

				//similar to lcg32, but with a longer period
				Uint32 lcg32_extended::raw32() {
					state = state * 1103515245 + add;
					if (!(state & 0x7fff)) add += 2;
					return state;
				}
				std::string lcg32_extended::get_name() const {return "lcg32_extended";}
				void lcg32_extended::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					walker->handle(add);
					add |= 1;
				}

				Uint32 clcg32of96_varqual::raw32() {
					lcg1 = lcg1 * 1103515245 + 12345;
					Uint64 tmp = Uint64(lcg2) * 1579544716;
					lcg2 = Uint32(tmp & 0x7FffFFff) + Uint32(tmp >> 33) + 1;
					return lcg2 + Uint32(lcg1 >> outshift);
				}
				std::string clcg32of96_varqual::get_name() const {
					std::ostringstream str;
					str << "clcg(32," << (32 + outshift + 32) << ")";
					return str.str();
				}
				void clcg32of96_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(lcg1); walker->handle(lcg2);
					if (!lcg2) lcg2 = 1;
				}
				Uint16 clcg16of96_varqual::raw16() {
					lcg1 = lcg1 * 1103515245 + 12345;
					Uint64 tmp = Uint64(lcg2) * 1579544716;
					lcg2 = Uint32(tmp & 0x7FffFFff) + Uint32(tmp >> 33) + 1;
					return Uint16(lcg2 >> 12) + Uint16(lcg1 >> outshift);
					//return Uint16((lcg2 + lcg1) >> outshift);
				}
				std::string clcg16of96_varqual::get_name() const {
					std::ostringstream str;
					str << "clcg(16," << (16 + outshift + 32) << ")";
					return str.str();
				}
				void clcg16of96_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(lcg1); walker->handle(lcg2);
					if (!lcg2) lcg2 = 1;
				}
				Uint8 clcg8of96_varqual::raw8() {
					lcg1 = lcg1 * 1103515245 + 12345;
					Uint64 tmp = lcg2 * Uint64(1579544716);
					lcg2 = Uint32(tmp & 0x7FffFFff) + Uint32(tmp >> 33) + 1;
					return Uint8(lcg2) + Uint8(lcg1 >> outshift);
				}
				std::string clcg8of96_varqual::get_name() const {
					std::ostringstream str;
					str << "clcg(8," << (8 + outshift + 32) << ")";
					return str.str();
				}
				void clcg8of96_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(lcg1); walker->handle(lcg2);
					if (!lcg2) lcg2 = 1;
				}

				void pcg32::seed(Uint64 s) { state = 0; raw32(); state += s; raw32(); }
				Uint32 pcg32::raw32() {
					Uint64 oldstate = state;
					state = state * 0x5851f42d4c957f2dULL + inc;
					Uint32 xorshifted = Uint32(((oldstate >> 18u) ^ oldstate) >> 27u);
					Uint32 rot = Uint32(oldstate >> 59u);
					return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31));
				}
				std::string pcg32::get_name() const {
					if (inc == 0xda3e39cb94b95bdbULL) return "pcg32";
					std::ostringstream str;
					str << "pcg32(" << std::hex << inc << ")";
					return str.str();
				}
				void pcg32::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				void pcg32_norot::seed(Uint64 s) { state = 0; raw32(); state += s; raw32(); }
				Uint32 pcg32_norot::raw32() {
					Uint64 oldstate = state;
					state = state * 0x5851f42d4c957f2dULL + inc;
					Uint32 xorshifted = Uint32(((oldstate >> 18u) ^ oldstate) >> 27u);
					return xorshifted;
					//Uint32 rot = Uint32(oldstate >> 59u);
					//return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31));
				}
				std::string pcg32_norot::get_name() const {
					if (inc == 0xda3e39cb94b95bdbULL) return "pcg32_norot";
					std::ostringstream str;
					str << "pcg32_norot(" << std::hex << inc << ")";
					return str.str();
				}
				void pcg32_norot::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				void cmrg32of192::seed(Uint64 s) {
					n1m0 = Uint32(s);
					n1m1 = n1m2 = 1;
					n2m0 = Uint32(s >> 32);
					n2m1 = n2m2 = 1;
				}
				Uint32 cmrg32of192::raw32() {
					Uint64 n1 = (Uint64(n1m1) * 1403580 - Uint64(n1m2) * 810728) % ((1ull << 32) - 209);
					Uint64 n2 = (Uint64(n2m0) * 527612 - Uint64(n2m2) * 1370589) % ((1ull << 32) - 22853);
					n1m2 = n1m1;
					n1m1 = n1m0;
					n1m0 = n1;
					n2m2 = n2m1;
					n2m1 = n2m0;
					n2m0 = n2;
					return n1 + n2;
				}
				std::string cmrg32of192::get_name() const {return "cmrg32of192";}
				void cmrg32of192::walk_state(StateWalkingObject *walker) {
					walker->handle(n1m0);
					walker->handle(n1m1);
					walker->handle(n1m2);
					walker->handle(n2m0);
					walker->handle(n2m1);
					walker->handle(n2m2);
				}

				Uint32 xsh_lcg_bad::raw32() {
					Uint64 tmp = x1 ^ (x1 << 11);
					x1 = x2;
					x2 = x3;
					x3 = x0;
					x0 = (x0 >> 19) ^ tmp ^ (tmp >> 8);
					lcg = (lcg * 279470273) % 4294967291;
					return x0 ^ lcg;
				}
				std::string xsh_lcg_bad::get_name() const { return "xsh_lcg_bad"; }
				void xsh_lcg_bad::seed(Uint64 s) {
					x1 = s;
					x0 = x2 = x3 = 0xFFffFFffFFffFFffull;//changed to prevent the bad all-zeroes case
					lcg = 2233445566;
					for (int i = 0; i < 64; i++) raw32();
				}
				void xsh_lcg_bad::walk_state(StateWalkingObject *walker) {
					walker->handle(x0);
					walker->handle(x1);
					walker->handle(x2);
					walker->handle(x3);
					walker->handle(lcg);
				}


				Uint16 mmr16::raw16() {
					Uint16 old = a;
					a = b * 0x69ad;
					b = rotate16(b, 7) ^ c;
					c = rotate16(c, 5) + old;
					return old;
				}
				std::string mmr16::get_name() const { return "mmr16"; }
				void mmr16::walk_state(StateWalkingObject *walker) {
					walker->handle(a);
					walker->handle(b);
					walker->handle(c);
				}
				Uint32 mmr32::raw32() {
					Uint32 old = a;
					a = b * 0xAC4969AD;
					b = rotate16(b, 13) ^ c;
					c = rotate16(c, 9) + old;
					return old;
				}
				std::string mmr32::get_name() const { return "mmr32"; }
				void mmr32::walk_state(StateWalkingObject *walker) {
					walker->handle(a);
					walker->handle(b);
					walker->handle(c);
				}

				Uint16 garthy16::raw16() {
					if (!counter) scale += 2;
					scale += 2;
					Uint16 temp = value * scale;
					value += ((temp << 7) | (temp >> 9)) ^ counter++;
					return value;
				}
				std::string garthy16::get_name() const { return "garthy16"; }
				void garthy16::walk_state(StateWalkingObject *walker) {
					walker->handle(value); walker->handle(counter); walker->handle(scale);
					scale |= 1;
				}
				Uint32 garthy32::raw32() {
					if (!counter) scale += 2;
					scale += 2;
					Uint32 temp = value * scale;
					value += ((temp << 13) | (temp >> 19)) ^ counter++;
					return value;
				}
				std::string garthy32::get_name() const {return "garthy32";}
				void garthy32::walk_state(StateWalkingObject *walker) {
					walker->handle(value); walker->handle(counter); walker->handle(scale);
					scale |= 1;
				}

				Uint16 binarymult16::raw16() {
					//with Bays-Durham shuffle (size 16) fails @ 32 GB
					Uint16 old = a;
					a = b * (c | 1);
					b = c ^ (old >> 7);
					c ^= old + d++;
					return a;
				}
				std::string binarymult16::get_name() const {return "binarymult16";}
				void binarymult16::walk_state(StateWalkingObject *walker) {
					walker->handle(a); walker->handle(b); walker->handle(c); walker->handle(d);
				}
				Uint32 binarymult32::raw32() {
					Uint32 old = a;
					a = b * (c | 1);
					b = c ^ (old >> 13);
					c ^= old + d++;
					return a;
				}
				std::string binarymult32::get_name() const {return "binarymult32";}
				void binarymult32::walk_state(StateWalkingObject *walker) {
					walker->handle(a); walker->handle(b); walker->handle(c); walker->handle(d);
				}

				Uint16 rxmult16::raw16() {
					if (!a) {c++; if (!c) {c=1; d+=2;}}
					a = a * 0x9ad + d;
					b = (((b<<7)|(b>>9)) + a) ^ c;
					Uint16 tmp = b * 5245;
					tmp ^= tmp >> 8;
					return tmp + a;
				}
				std::string rxmult16::get_name() const {return "rxmult16";}
				void rxmult16::walk_state(StateWalkingObject *walker) {
					walker->handle(a); walker->handle(b); walker->handle(c); walker->handle(d);
					d |= 1;
				}

				Uint64 multish2x64::raw64() {
					Uint64 old = ~a;
					a = (a * 0xa536c4b9) + b;
					b += (old << 21) | (old >> 43);
					return old;
				}
				std::string multish2x64::get_name() const {return "multish2x64";}
				void multish2x64::walk_state(StateWalkingObject *walker ) {
					walker->handle(a); walker->handle(b);
				}
				Uint32 multish3x32::raw32() {
					Uint32 old = a;
					a = (b * 0xa536c4b9) + c++;
					b = ((b << 7) | (b >> 25)) + old;
					return old;
				}
				std::string multish3x32::get_name() const {return "multish3x32";}
				void multish3x32::walk_state(StateWalkingObject *walker ) {
					walker->handle(a); walker->handle(b); walker->handle(c);
				}
				Uint16 multish4x16::raw16() {
					Uint16 old = a;
					if (!c++) d++;
					a = (b^d) * 0x96b9 + c;
					b = ((b << 5) | (b >> 11)) ^ old;
					return old;
				}
				std::string multish4x16::get_name() const {return "multish4x16";}
				void multish4x16::walk_state(StateWalkingObject *walker ) {
					walker->handle(a); walker->handle(b); walker->handle(c); walker->handle(d);
				}

				Uint16 old_mwlac16::raw16() {
					Uint16 oa;
					oa = a;
					a = (b * 0x9785) ^ (a >> 7);
					b = c + (oa >> 2);
					c = d;
					d += ~oa;
					return c;
				}
				std::string old_mwlac16::get_name() const { return "old_mwlac16"; }
				void old_mwlac16::walk_state(StateWalkingObject *walker) {
					walker->handle(a); walker->handle(b); walker->handle(c); walker->handle(d);
				}
				Uint16 mwlac_varA::raw16() {
					Uint16 oa;
					oa = a * 0x9785;//   1001011110000101
					a = b ^ rotate16(a, 7);
					b += c;
					c = oa;
					return c;
				}
				std::string mwlac_varA::get_name() const { return "mwlac_varA"; }
				void mwlac_varA::walk_state(StateWalkingObject *walker) {
					walker->handle(a); walker->handle(b); walker->handle(c);
				}
				Uint16 mwlac_varB::raw16() {
					Uint16 oa;
					oa = a * 0x9785;//   1001011110000101
					b = rotate(b, 13);
					a = b ^ rotate16(a, 7);
					b += c;
					c = oa;
					return c;
				}
				std::string mwlac_varB::get_name() const { return "mwlac_varB"; }
				void mwlac_varB::walk_state(StateWalkingObject *walker) {
					walker->handle(a); walker->handle(b); walker->handle(c);
				}
				Uint16 mwlac_varC::raw16() {
					a *= 0x9785;//   1001011110000101
					b = rotate16(b, 5);
					c = rotate16(c, 13);
					b ^= a;
					a ^= c;
					c += b;
					return b;
				}
				std::string mwlac_varC::get_name() const { return "mwlac_varC"; }
				void mwlac_varC::walk_state(StateWalkingObject *walker) {
					walker->handle(a); walker->handle(b); walker->handle(c);
				}
				Uint16 mwlac_varD::raw16() {
					a += b;
					b -= c;
					c += a;
					a *= 0x9785;//   1001011110000101
					b = rotate16(b, 7);
					c = rotate16(c, 4);
					return a;
				}
				std::string mwlac_varD::get_name() const { return "mwlac_varD"; }
				void mwlac_varD::walk_state(StateWalkingObject *walker) {
					walker->handle(a); walker->handle(b); walker->handle(c);
				}
				Uint16 mwlac_varE::raw16() {
					c ^= a;
					a += b;
					b -= c;
					a += c;
					c *= 0x9785;//   1001011110000101
					//shift:	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15
					//						37	39-	41	34-	34	38-	39	38-	35	20
					b = rotate16(b, 6);
					return a;
				}
				std::string mwlac_varE::get_name() const { return "mwlac_varE"; }
				void mwlac_varE::walk_state(StateWalkingObject *walker) {
					walker->handle(a); walker->handle(b); walker->handle(c);
				}

				Uint32 mwc64x::raw32() {
					Uint32 c = state >> 32;
					Uint32 x = Uint32(state);
					state = x * Uint64(4294883355U) + c;
					return x ^ c;
				}
				std::string mwc64x::get_name() const { return "mwc64x"; }
				void mwc64x::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}

				Uint32 xlcg32of64_varqual::raw32() {
					enum {
						X = 0xC74EAD55,//must end in 5 or D
						M = 0x947E3DB3,//must end in 3 or B
					};
					state = (state ^ X) * M;
					return Uint32(state >> outshift);
				}
				std::string xlcg32of64_varqual::get_name() const {
					std::ostringstream str;
					str << "xlcg(32," << (32 + outshift) << ")";
					return str.str();
				}
				void xlcg32of64_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				Uint16 xlcg16of64_varqual::raw16() {
					enum {
						X = 0xC74EAD55,//must end in 5 or D
						M = 0x947E3DB3,//must end in 3 or B
					};
					state = (state ^ X) * M;
					return Uint16(state >> outshift);
				}
				std::string xlcg16of64_varqual::get_name() const {
					std::ostringstream str;
					str << "xlcg(16," << (16 + outshift) << ")";
					return str.str();
				}
				void xlcg16of64_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				Uint8 xlcg8of64_varqual::raw8() {
					enum {
						X = 0xC74EAD55,//must end in 5 or D
						M = 0x947E3DB3,//must end in 3 or B
					};
					state = (state ^ X) * M;
					return Uint8(state >> outshift);
				}
				std::string xlcg8of64_varqual::get_name() const {
					std::ostringstream str;
					str << "xlcg(8," << (8 + outshift) << ")";
					return str.str();
				}
				void xlcg8of64_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				Uint32 xlcg32of128_varqual::raw32() {
					enum {
						X = 0xC74EAD55,//must end in 5 or D
						M = 0x947E3DB3,//must end in 3 or B
					};
					Uint64 a = Uint32(low) * Uint64(M);
					Uint64 b = (low >> 32) * Uint64(M);
					low = a + (b << 32);
					b += a >> 32;
					high = high * M + (b >> 32);
					low ^= X;
					if (outshift >= 64) return Uint16(high >> (outshift - 64));
					if (outshift > 48) return Uint16((low >> outshift) | (high << (64 - outshift)));
					return Uint16(low >> outshift);
				}
				std::string xlcg32of128_varqual::get_name() const {
					std::ostringstream str;
					str << "xlcg(32," << (32 + outshift) << ")";
					return str.str();
				}
				void xlcg32of128_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(low); walker->handle(high);
				}
				Uint16 xlcg16of128_varqual::raw16() {
					enum {
						X = 0xC74EAD55,//must end in 5 or D
						M = 0x947E3DB3,//must end in 3 or B
					};
					Uint64 a = Uint32(low) * Uint64(M);
					Uint64 b = (low >> 32) * Uint64(M);
					low = a + (b << 32);
					b += a >> 32;
					high = high * M + (b >> 32);
					low ^= X;
					if (outshift >= 64) return Uint16(high >> (outshift - 64));
					if (outshift > 48) return Uint16((low >> outshift) | (high << (64 - outshift)));
					return Uint16(low >> outshift);
				}
				std::string xlcg16of128_varqual::get_name() const {
					std::ostringstream str;
					str << "xlcg(16," << (16 + outshift) << ")";
					return str.str();
				}
				void xlcg16of128_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(low); walker->handle(high);
				}
				Uint8 xlcg8of128_varqual::raw8() {
					enum {
						X = 0xC74EAD55,//must end in 5 or D
						M = 0x947E3DB3,//must end in 3 or B
					};
					Uint64 a = Uint32(low) * Uint64(M);
					Uint64 b = (low >> 32) * Uint64(M);
					low = a + (b << 32);
					b += a >> 32;
					high = high * M + (b >> 32);
					low ^= X;
					if (outshift >= 64) return Uint16(high >> (outshift - 64));
					if (outshift > 48) return Uint16((low >> outshift) | (high << (64 - outshift)));
					return Uint16(low >> outshift);
				}
				std::string xlcg8of128_varqual::get_name() const {
					std::ostringstream str;
					str << "xlcg(8," << (8 + outshift) << ")";
					return str.str();
				}
				void xlcg8of128_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(low); walker->handle(high);
				}

				Uint32 cxlcg32of96_varqual::raw32() {
					enum {
						X = 0xC74EAD55,//must end in 5 or D
						M = 0x947E3DB3,//must end in 3 or B
					};
					lcg1 = (lcg1 ^ X) * M;
					Uint64 tmp = lcg2 * Uint64(1579544716);
					lcg2 = Uint32(tmp & 0x7FffFFff) + Uint32(tmp >> 33) + 1;
					return lcg2 + Uint32(lcg1 >> outshift);
				}
				std::string cxlcg32of96_varqual::get_name() const {
					std::ostringstream str;
					str << "cxlcg(32," << (32 + outshift + 32) << ")";
					return str.str();
				}
				void cxlcg32of96_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(lcg1); walker->handle(lcg2);
					if (!lcg2) lcg2 = 1;
				}
				Uint16 cxlcg16of96_varqual::raw16() {
					enum {
						X = 0xC74EAD55,//must end in 5 or D
						M = 0x947E3DB3,//must end in 3 or B
					};
					lcg1 = (lcg1 ^ X) * M;
					Uint64 tmp = lcg2 * Uint64(1579544716);
					lcg2 = Uint32(tmp & 0x7FffFFff) + Uint32(tmp >> 33) + 1;
					return Uint16(lcg2) + Uint16(lcg1 >> outshift);
				}
				std::string cxlcg16of96_varqual::get_name() const {
					std::ostringstream str;
					str << "cxlcg(16," << (16 + outshift + 32) << ")";
					return str.str();
				}
				void cxlcg16of96_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(lcg1); walker->handle(lcg2);
					if (!lcg2) lcg2 = 1;
				}
				Uint8 cxlcg8of96_varqual::raw8() {
					enum {
						X = 0xC74EAD55,//must end in 5 or D
						M = 0x947E3DB3,//must end in 3 or B
					};
					lcg1 = (lcg1 ^ X) * M;
					Uint64 tmp = lcg2 * Uint64(1579544716);
					lcg2 = Uint32(tmp & 0x7FffFFff) + Uint32(tmp >> 33) + 1;
					return Uint8(lcg2) + Uint8(lcg1 >> outshift);
				}
				std::string cxlcg8of96_varqual::get_name() const {
					std::ostringstream str;
					str << "cxlcg(8," << (8 + outshift + 32) << ")";
					return str.str();
				}
				void cxlcg8of96_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(lcg1); walker->handle(lcg2);
					if (!lcg2) lcg2 = 1;
				}

				Uint64 cxm64_varqual::raw64() {
					const Uint64 K = 0x6595a395a1ec531b;
					Uint64 tmp = high >> 32;
					low += K;
					high += K + ((low < K) ? 1 : 0);
					tmp ^= high ^ 0;//(Uint64)this;
					for (int i = 1; i < num_mult; i++) {
						tmp *= K;
						tmp ^= tmp >> 32;
					}
					tmp *= K;
					return tmp + low;
				}
				std::string cxm64_varqual::get_name() const {
					std::ostringstream str;
					str << "cxm" << num_mult << "n64";
					return str.str();
				}
				void cxm64_varqual::walk_state(StateWalkingObject *walker) {
					walker->handle(low);
					walker->handle(high);
					//walker->handle(num_mult);
				}


				Uint32 mo_Cmfr32::raw32() {
					state = ~(2911329625u*state); state = rotate32(state,17);
					return state;
				}
				std::string mo_Cmfr32::get_name() const {return "mo_Cmfr32";}
				void mo_Cmfr32::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				Uint32 mo_Cmr32::raw32() {
					state = 4031235431u * state; state = rotate32(state, 15);
					return state;
				}
				std::string mo_Cmr32::get_name() const { return "mo_Cmr32"; }
				void mo_Cmr32::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				Uint32 mo_Cmr32of64::raw32() {
					state = 38217494031235431ull * state; state = rotate64(state, 37);
					return Uint32(state);
				}
				std::string mo_Cmr32of64::get_name() const { return "mo_Cmr32of64"; }
				void mo_Cmr32of64::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}

				Uint32 murmlac32::raw32() {
					Uint32 tmp = state1;
					for (int i = 0; i < rounds; i++) {
						tmp *= 4031235431u;
						tmp ^= tmp >> 16;
					}
					state1 += state2;
					state2 = tmp;
					return state1;
				}
				std::string murmlac32::get_name() const {
					std::ostringstream str;
					str << "murmlac32(" << rounds << ")";
					return str.str();
				}
				void murmlac32::walk_state(StateWalkingObject *walker) {
					walker->handle(state1); walker->handle(state2);
				}

				Uint64 mulcr64::raw64() {
					Uint64 rv = a * count;
					a = rotate64(a, 24) + b;
					count += 2;
					b = rotate64(b, 37) ^ rv;
					return rv;
				}
				std::string mulcr64::get_name() const { return "mulcr64"; }
				void mulcr64::walk_state(StateWalkingObject *walker) {
					walker->handle(a);
					walker->handle(b);
					walker->handle(count);
					count |= 1;
				}
				Uint32 mulcr32::raw32() {
					Uint32 rv = a * 2911329625u;
					a = b ^ count++;
					b = rotate32(b, 11) + rv;
					return rv;
				}
				std::string mulcr32::get_name() const { return "mulcr32"; }
				void mulcr32::walk_state(StateWalkingObject *walker) {
					walker->handle(a);
					walker->handle(b);
					walker->handle(count);
				}
				Uint16 mulcr16::raw16() {
					Uint16 rv = a * 2911329625u;
					a = b ^ count++;
					b = rotate16(b, 6) + rv;
					return rv;
				}
				std::string mulcr16::get_name() const { return "mulcr16"; }
				void mulcr16::walk_state(StateWalkingObject *walker) {
					walker->handle(a);
					walker->handle(b);
					walker->handle(count);
				}






				//0xC5B3CF786C806DF7
				Uint64 mo_Cmres64::raw64() {
					const uint64_t m = state * UINT64_C(0x7CDD0CFB6AD2A499), // 0xC5B3CF786C806DF7 does well up to at least 256GB with rotation by 33, no anomalies
						n = state2 * UINT64_C(0x2C40E78566614E13);
					return (state -= rotate64(m, 35)) ^ (state2 -= rotate64(n, 33));// ^ (weyl += UINT64_C(0x9E3779B97F4A7C15));
				}
				std::string mo_Cmres64::get_name() const { return "mo_Cmres64"; }
				void mo_Cmres64::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					walker->handle(state2);
					printf("Seed is 0x%X, seed2 is 0x%X\r\n", state, state2);
				}

				Uint64 tiptoe64::raw64() {
					//WORKS WELL, FAST, NOT EQUIDISTRIBUTED
					//const uint64_t s = state;
					//const uint64_t z = (s ^ s >> 26) * (state += 0x6A5D39EAE116586AULL);
					//return z ^ (z >> 28);

					// passes 2TB no anomalies, does pretty well in gjrand huge, miserably fails gjrand --tera
					//const uint64_t s = (state += 0x6A5D39EAE12657B7ULL); //0x6A5D39EAE12657A9ULL
					//const uint64_t z = (s ^ (s >> 25)) * (s | 1ULL);
					//return z ^ (z >> 19);

					// passes 32TB no failures, passes gjrand huge and tera, but was tested with seed 0 only, which may bias tests. Not equidistributed!
					//const uint64_t s = (state += 0x6C8E9CF570932BD5ULL); //0x6A5D39EAE12657A9ULL
					//const uint64_t z = (s ^ (s >> 25)) * (s | 0xA529ULL);
					//return z ^ (z >> 22);

					//uint64_t z = (state += UINT64_C(0x352E9CF570932BDD)); //UINT64_C(0x6A5D39EAE12657A9)
					//z = ((z >> 25) ^ z) * (z | UINT64_C(1));// (s | 1ULL); //UINT64_C(0x6C8E9CF570932BD5) // UINT64_C(0x2545F4914F6CDD1D)
					//return (z ^ (z >> 22));
					//UINT64_C(0x6C8E9CF570932BD5)

					// left off here, similar to the one that passes 32TB, but not equidistributed
					//state += UINT64_C(0x6C8E9CF570932BD5);
					//uint64_t z = (state ^ state >> 25) * (state | UINT64_C(0xA529));
					//return (z ^ (z >> 22));


					//Uint64 z = (state += 0x9E3779B97F4A7C15);
					//z ^= z >> 27;
					//z *= (Uint64(0x6C8E9CF570932BD5) ^ (z & Uint64(0x8000000000000000)));
					//return z ^ z >> 31;

					// good to 32TB, 2 unusual anomalies
					//Uint64 z = state++;
					//z = ((z ^ rotate64(z, 59) ^ rotate64(z, 23))) * Uint64(0x6C8E9CF570932BD3);
					//z = (z ^ z >> (5 + (z >> 59))) * Uint64(0xAEF17502108EF2D9);
					//return z ^ z >> 25;

					//public static long tiptoe(long state) { return (state = ((state = (state ^ (state << 23 | state >>> 41) ^ (state << 59 | state >>> 5)) * 0x6C8E9CF570932BD5L) ^ state >>> (5 + (state >>> 59))) * 0xAEF17502108EF2D9L) ^ state >>> 25); }
					//Long.rotateLeft(result + 0x6C8E9CF570932BD3L, 17) + 0x9E3779B97F4A7C15
					// seems to work very well; 8TB with one "unusual" anomaly at 1TB
					//Uint64 z = ++state;
					//z = ((z << ((z & 31) + 5u)) ^ z) * Uint64(0x6C8E9CF570932BD3) ^ Uint64(0x9E3779B97F4A7C15);
					//z = (z ^ z >> 24u) * Uint64(0xAEF17502108EF2D9);
					//return (z >> 25u) ^ z;

					//Uint64 z = ++state;
					//z = (z ^ z << 12u) * Uint64(0x6C8E9CF570932BD3);
					//z = (z ^ z >> 25u) * Uint64(0xAEF17502108EF2D9) + Uint64(0x9E3779B97F4A7C15);
					//return z ^ z >> 26u;

					//z ^= z >> 27u;

					// Excellent! Passes 32TB with only 1 anomaly, at 8TB!
					//uint64_t z = state;
					//z = (z ^ rotate64(z, 13) ^ rotate64(z, 31) ^ rotate64(z, 41) ^ rotate64(z, 59)) * UINT64_C(0x6C8E9CF570932BD3);
					//state += UINT64_C(0x9E3779B97F4A7C15);
					//return z ^ z >> 26u;

					// fails at 8TB
					//Uint64 z = (state += Uint64(0x9E3779B97F4A7C15));
					//z = (z ^ rotate64(z, 13) ^ rotate64(z, 41)) * UINT64_C(0x6C8E9CF570932BD3);
					//return z ^ z >> 26u;
					// 0x41C64E6D
					// 0x6F3E235 works pretty well, L'Ecuyer, errata

					//uint64_t z = (state = state * UINT64_C(0x41C64E6D) + UINT64_C(1));//stream);
					//z = (z ^ z >> 32u) * UINT64_C(0xAEF17502108EF2D9);
					//return (z ^ z >> 28u);


					//passes 32TB no anomalies!
					//uint64_t z = state;
					//z = (z ^ z >> 27) * UINT64_C(0xAEF17502108EF2D9);
					//state = state * UINT64_C(1103515245) + UINT64_C(1);
					//return (z ^ z >> 25u);


					//state = (state ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0x6C8E9CF570932BD3);


					//z = (z ^ z << 13u) * Uint64(0x6C8E9CF570932BD3) ^ Uint64(0x6C8E9CF570932BD3) << ((z & 63u) | 8u);

					//public static long tiptoe(long state) { return (state = ((state = (state ^ (state << (state | 8L))) * 0x6C8E9CF570932BD5L) ^ state >>> 24) * 0xAEF17502108EF2D9L) ^ state >>> 25; }

					//uint32_t rot1 = (uint32_t)(state >> 59u);
					//uint32_t high = (uint32_t)(state >> 32u);
					//uint32_t low = (uint32_t)state;
					//uint32_t xored = high ^ low;
					//Uint64 next = rotate32(xored, rot1);
					//return next | (rotate32(high, next & 31u) << 32u);

					// seems to be working well, no anomalies up to 128GB
					//Uint64 z = ++state;
					//z = (z ^ rotate64(z, 58) ^ rotate64(z, 23)) * Uint64(0x6C8E9CF570932BD3);
					//z = (z ^ z >> 26) * Uint64(0xAEF17502108EF2D9);
					//return z ^ z >> 25;


					//return (z ^ z >> (5 + (z >> 59)));

					//return z + (z << (5 + (z & 31)));
					//z += z << (5 + (z & 31));

					//Uint64 z = (state += Uint64(0xBF58476D1CE4E5B9));

					// left off with this at 512GB doing well
					//Uint64 z = ++state ^ Uint64(0x6C8E9CF570932BD5);
					//z *= Uint64(0x94D049BB133111EB);
					//z ^= z >> 22;
					//z = (z ^ z >> 24) * Uint64(0xBF58476D1CE4E5B9);
					//return z ^ z >> 25;

					//passes 32TB but is likely not equidistributed (it could be?)
					//Uint64 z = ++state;
					//z ^= rotate64(z, 58) ^ rotate64(z, 23);
					//Uint64 y = z * Uint64(0x9E3779B97F4A7C15);
					//z = ~(z * Uint64(0x6C8E9CF570932BD6));
					//z *= (y ^= y >> (5 + (y >> 59)));
					//return z ^ z >> 25;

					//Uint64 z = (state += Uint64(0x9E3779B97F4A7C15)), a = z ^ z + 0x94D049BB133111EB, x = 3 * a ^ 2;
					//x *= 2 - a * x;        // 10
					//x *= 2 - a * x;        // 20
					//z = (z ^ z >> 25) * x;
					//x *= 2 - a * x;        // 40
					//x *= 2 - a * x;        // 80
					//z = (z ^ z >> 25) * x;
					//return z ^ z >> 28;

					//Uint64 z = ++state;
					//z = (z ^ rotate64(z, 58) ^ rotate64(z, 23)) * Uint64(0xAEF17502108EF2D9);
					//z = (z ^ z >> 24) * Uint64(0x6C8E9CF570932BD5);
					////z = (z ^ rotate64(z, 42) ^ rotate64(z, 17));
					//return z ^ z >> 25;



					//Uint64 z = (state += Uint64(0x6C8E9CF570932BD5));
					// + Uint64(0x9E3779B97F4A7C15)
					//z = (z ^ z >> 25) * Uint64(0x352E9CF570932BDD);

					//Uint64 z = (state += Uint64(0x6C8E9CF570932BD5));
					////z = (z ^ rotate64(z, 22) ^ rotate64(z, 47)) * Uint64(0x6C8E9CF570932BD5);
					////z *= Uint64(0x6C8E9CF570932BD5);
					//z = (z ^ z >> 26) * Uint64(0x352E9CF570932BDD); //352E9CF5947E3DB3
					//return (z ^ rotate64(z, 22) ^ rotate64(z, 47));

					//public static long tiptoe(long seed) { return ((seed = ((seed *= 0x6C8E9CF570932BD5L) ^ seed >>> 25) * 0x352E9CF570932BDDL) ^ (seed << 22 | seed >>> 42) ^ (seed << 47 | seed >>> 17)); }

					//* Uint64(0x6C8E9CF570932BD5) + Uint64(0x9E3779B97F4A7C15);
					//z = rotate64(z, 37);


					//z -= (rotate64(z, 39) & rotate64(z, 14));
					//uint64_t z = (UINT64_C(0x9E3779B97F4A7C15) ^ (state >> 25)) * (state << 3 ^ UINT64_C(0x6C8E9CF570932BD5));

					//        return (int) ((((state = (x ^ (state += 0x352E9CF570932BDDULL) ^ (state >>> 25)) * (state * x | 1L)) ^ (state >>> 22)) ^
					//((state = (y ^ (state += 0x352E9CF570932BDDL) ^ (state >> > 25)) * (state * y | 1L)) ^ (state >> > 22))) >> > 59);

					//const uint64_t s = (state *= 0x369DEA0F31A53F85ULL);
					//const uint64_t z = (s ^ (s >> 30)) * 0x61C8864680B583EBULL;
					//state += 0x9E3779B97F4A7C15ULL;
					//return z ^ (z >> 26);

					//does well to at least 512 GB, several seeds tried, all good
					//uint64_t z = (state = state * 0x2545F4914F6CDD1DULL + stream),
					//	y = (z ^ z >> 28);
					//return (y ^ (y >> 16)) * 0x5851F42D4C957F2DULL + (0x61C8864680B583EBULL);

					//state = state * 0x369DEA0F31A53F85ULL + stream;
					//return (state ^ state >> 25) + ((state << 19) | (state >> 45)) + ((state << 27) | (state >> 37));

					//state = state * 0x369DEA0F31A53F85ULL + stream;
					//return (rotate64(state, 23) - rotate64(state, 19)) - ((state >> 27) ^ state);

					/*
					uint64_t state = 0ULL; const uint64_t stream = 1ULL; inline uint64_t random() { state = state * 0x369DEA0F31A53F85ULL + stream; return ((state << 23) | (state >> 64 - 23)) - ((state << 19) | (state >> 64 - 19)) - ((state >> 27) ^ state); }
					*/
					//(rotl(state, 27) + rotl(~state, 22)) + ((state >> 26) ^ state);
					//return ((rotate64(state, 37) - rotate64(state, 20))) * 0x2545F4914F6CDD1DULL;
					//return (state ^ state >> 27) + ((state << 27) | (state >> 37)) - ((state << 19) | (state >> 45));
					//return state ^ (rotate64(state, 27) + rotate64(state, 19) + rotate64(-state, 37));
					//return state ^ ((state << 27) | (state >> 37)) + ((state << 19) | (state >> 45)) + ((-state << 37) | (-state >> 27));
					//const uint64_t z = (state ^ state >> 26) + ((state << 27) | (state >> 37));
					//return z ^ z >> 37;

					//uint64_t z = (state = state * 0x369DEA0F31A53F85ULL + 0x1ULL);
					//z = (-z ^ (z >> 25)) * 0x2545F4914F6CDD1DULL;
					//return z ^ z >> 28;
					//0x61C8864680B583EDULL - z

					//return (state ^ (state >> 28)) ^ (((state >> 6) ^ (state >> 33)) * 0x5851F42D4C957F2DL - stream);
					//state = state * 0x2545F4914F6CDD1DULL + stream;
					//const uint64_t z = (state ^ (state >> 28)) * (state | 0x4A529ULL);
					//return z ^ (z >> 28);

					/*
					public ulong state = 1UL; public ulong randomULong() { ulong z = (state *= 0x369DEA0F31A53F85UL); z = (z ^ (z >> 30)) * 0x61C8864680B583EBUL; state += 0x9E3779B97F4A7C15UL;return z ^ (z >> 26); }
					*/
					//z = (j *= 0x369DEA0F31A53F85ULL);
					//z = (z ^ z >> 30) * (z - (j += 0x9E3779B97F4A7C15ULL));
					//z ^= (z >> 26);



					//Uint64 z = state;
					//z ^= (((state += 0xA99635D5B8597AE5ULL) ^ z >> 23) * 0xAD5DE9A61A9C3D95ULL);
					//return z ^ (z >> 29);

					//z ^= (((state += 0x9E3779B97F4A7BB5ULL) ^ z >> 23) * 0x6A5D39EAE116586DULL);
					//z ^= ((state += 0x9E3779B97F4A7C15ULL) ^ (z >> 25)) * 0x6A5D39EAE116586DULL;
					//Uint64 z = (state += 0x9E3779B97F4A7C15ULL);
					//z ^= (z ^ (z >> 25)) * 0x6A5D39EAE116586DULL;
					//return z ^ (z >> 24);

					//z = (z ^ z >> 26) * 0x5851F42D4C957F2DULL;
					//z *= (z << 1) ^ 0x5851F42D4C957F2D; // ^ (z << 11) //0x2545F4914F6CDD1D

					//const uint64_t s = state;
					//const uint64_t z = (s >> 29) * (s | (state += 0x9E3779B97F4A7A55ULL));
					//return z ^ (z >> 29);

					//LinnormRNG normal
					//					uint64_t z = (state = state * UINT64_C(0x41C64E6D) + UINT64_C(1));
					//					z = (z ^ z >> 27u) * UINT64_C(0xAEF17502108EF2D9);
					//					return z ^ z >> 25u;

					//uint64_t z = (state = (state ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0x41C64E6B));
					//uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15));
					//return (stream = (stream >> 1 ^ (-(stream & 1) & UINT64_C(0xD800000000000000))));
					//(stream = (stream >> 1 ^ (-(stream & 8) & UINT64_C(0x1C80000000000000))));
					//((stream = (stream >> 1 ^ (-(stream & 8) & UINT64_C(0x1C80000000000000)))) << 3 ^ UINT64_C(5))

					//QuixoticRNG attempt for speed maybe
					//uint64_t z = (state = (state ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0x42042E4DD58B));
					//uint64_t z = (state = (state ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0x5DA942042E4DD58B));// 0x5888120408461063 // 0x4120508604029043 //0x482050128A101603
					
					//QuixoticRNG normal
					//uint64_t z = (state = (state ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0x41C64E6B));
					//z = (z ^ z >> 27u) + UINT64_C(0xAEF17502108EF2D9);
					//return z ^ z >> 25u;
					
					//uint64_t z = (state = (state ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0xC6BC279692B5CC83));
					//z = rotate64(z, 27);
					//z = (z << 26) - z;
					//return z ^ z >> 25u;

					// fails at 512GB
					//					uint64_t z = (state = state * UINT64_C(0x41C64E6B) + UINT64_C(1));
					//					z = (z ^ z >> 27u) + UINT64_C(0xAEF17502108EF2D9);
					//					return z ^ z >> 25u;

					// works really well for some reason???
					//uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15)) + (stream = (stream >> 1 ^ (-(stream & 1) & UINT64_C(0xD800000000000000))));
					//z = (z ^ z >> 26u) * UINT64_C(0xAEF17502108EF2D9);
					//return (z ^ z >> 29u);

					// LinnormRNG.determine()
					uint64_t z = ((state += UINT64_C(0x632BE59BD9B4E019)) ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xC6BC279692B5CC83);
					z = (z ^ z >> 27u) * UINT64_C(0xAEF17502108EF2D9);
					return (z ^ z >> 25u);

					// Quixotic determine attempt?
					// where we left off
					//uint64_t z = ((state += UINT64_C(0x9E3779B97F4A7C15)) ^ UINT64_C(0xAC51F42D4C957F2D)) + UINT64_C(0xC6BC279692B5CC85);// ^ UINT64_C(0x5851F42D4C957F2D)) * UINT64_C(0x41C64E6B); //0xC6BC279692B5CC83
					//z = (z ^ z >> 28) * UINT64_C(0xAEF17502108EF2DB);// *UINT64_C(0xAEF17502108EF2D9); // ^ UINT64_C(0x6C8E9CF570932BD5)
					//z = (z ^ z >> 17) + UINT64_C(0x6C8E9CF570932BD5) ^ UINT64_C(0x9E3779B97F4A7C15);
					//return (z ^ z >> 31u);

					// PaperweightRNG; rather slow
					//uint64_t z = (state -= UINT64_C(0x9E3779B97F4A7C15));
					//z ^= z >> 13;
					//z = (z << 19) - z;
					//z ^= z >> 12;
					//z = (z << 17) - z;
					//z ^= z >> 14;
					//z = (z << 13) - z;
					//return z ^ z >> 24;


					//uint64_t s = (state -= UINT64_C(0x9E3779B97F4A7C15));// ^ UINT64_C(0x6A5D39EAE127586A);
					//s = ((s << 10) - rotate32(s, 6));
					//s -= s << 7;
					//s ^= s >> 15;// ^ UINT64_C(0x6C8E9CF570932BD5);
					//s = (s << 15) - rotate64(s, 11);
					//s -= s << 5;
					//s ^= s >> 11;
					//s = (s << 9) - rotate64(s, 5);
					//s -= s << 13;
					//return s ^ s >> 27;

					// 0xFF51AFD7ED558CCD 0xC4CEB9FE1A85EC53

					//uint64_t x = (state += UINT64_C(0x9E3779B97F4A7C15));
					//x = (x ^ x >> 16) * UINT64_C(0x85EBCA6B);
					//x = (x ^ x >> 13) * UINT64_C(0xC2B2AE35);
					//return x ^ x >> 16;

					//  ^ UINT64_C(0x2545F4914F6CDD1D)) * UINT64_C(0x41C64E6B)
					//const uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15)), x = (z ^ rotate64(z, 19) ^ rotate64(z, 43)) * UINT64_C(0x9E3779BD);
					//return (x ^ rotate64(x, 29) ^ rotate64(x, 37));

					//const uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15)), x = (z ^ rotate64(z, 18) ^ rotate64(z, 47)) * UINT64_C(0x41C64E6D);
					//return (x ^ rotate64(x, 25) ^ rotate64(x, 38));

					////WHERE WE LEFT OFF
					//uint64_t y = state, z = y ^ (state += UINT64_C(0xC6BC279692B5CC85));
					//y -= rotate64(z, 29);
					//y *= z;
					//return y ^ y >> 28;

					//x = (x ^ x >> 31) * UINT64_C(0x369DEA0F31A53F85) + UINT64_C(0xAEF17502108EF2D9);
					//x = ((x ^ x >> 34) * UINT64_C(0x41C64E6D) ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0x41C64E6B);
					// (z ^ rotate64(z, 42) ^ rotate64(z, 23))
					//z -= (z << 19);// -UINT64_C(0x6C8E9CF570932BD5);
					//z ^= rotate64(z, 39) ^ rotate64(z, 11);// ^ UINT64_C(0xC6BC279692B5CC85);
					//z ^= z >> 11;
					//z -= (z << 13); - UINT64_C(0x6C8E9CF570932BD5);
					//return x ^ x >> 31;

					// seems pretty good, not too fast though
					//uint64_t z = (state -= UINT64_C(0x9E3779B97F4A7C15));
					//z ^= rotate64(z, 42) ^ rotate64(z, 23);// ^ UINT64_C(0x6C8E9CF570932BD5);
					//z -= (z << 19);
					//z ^= rotate64(z, 39) ^ rotate64(z, 11);// ^ UINT64_C(0xC6BC279692B5CC85);
					//z -= (z << 13);
					//return z ^ z >> 31;
					//

					//s = (s ^ s >> 31 ^ UINT64_C(0xC6BC279692B5CC85)) * UINT64_C(0xAEF17502108EF2DB);
					//s = (s ^ s >> 30 ^ UINT64_C(0xAC51F42D4C957F2D)) * UINT64_C(0x41C64E6B);

					//const uint64_t t = ((s << 11) - rotate64(s, 9)) * 127;// UINT64_C(0x6C8E9CF570932BD5);
					//return ((s << 5) - rotate64(s, 3)) ^ (t ^ t >> 30);// ((t << 11) - rotate64(t, 9));


					//s = (s ^ s >> 33 ^ UINT64_C(0xC6BC279692B5CC85)) * UINT64_C(0xAEF17502108EF2DB);
					//return s ^ rotate64(s, 17) ^ rotate64(s, 42);
					//return (s << 7) - rotate64(s, 3);
					//s ^= s >> 30;


					//s = ((s << 14) - rotate64(s, 12)) * UINT64_C(0xC6BC279692B5CC85);
					//s = ((s >> 31) ^ s ^ UINT64_C(0xC6BC279692B5CC85));
					//uint64_t t = s * UINT64_C(3188803096312630803);
					//t = rotate64(t, 33) - s;
					//const uint64_t t = state * 3188803096312630803L;
					//state -= rotate64(t, 33);
					//return (state << 23) - rotate64(state, 19);
					//uint64_t z = ((state += UINT64_C(0x9E3779B97F4A7C15)) ^ UINT64_C(0x5851F42D4C957F2D)) + UINT64_C(0xAEF17502108EF2DB);// ^ UINT64_C(0x5851F42D4C957F2D)) * UINT64_C(0x41C64E6B); //0xC6BC279692B5CC83
					//z = (z ^ z >> 28) * UINT64_C(0xC6BC279692B5CC83);// *UINT64_C(0xAEF17502108EF2D9); // ^ UINT64_C(0x6C8E9CF570932BD5)
					//return (z ^ z >> 27u);


					//uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15));
					//uint64_t z = ((state += UINT64_C(0x632BE59BD9B4E019)) ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xC6BC279692B5CC83); 
					//z = (z ^ z >> 26u) * UINT64_C(0xAEF17502108EF2D9);
					//return (z ^ z >> ((z >> 59u)+5u));

					//uint64_t z = (++state ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xC6BC279692B5CC83);
					//uint64_t z = (state += UINT64_C(0xC6BC279692B5CC83));
					//z ^= -(z >> 63) & UINT64_C(0x632BE59BD9B4E019);
					//z = (z ^ z >> 27u) * UINT64_C(0xAEF17502108EF2D9);
					//z ^= z >> 26u;
					//z = (z ^ UINT64_C(0x6A5D39EAE116586D)) * UINT64_C(0x94D049BB133111EB);// +(-(z >> 63) & UINT64_C(0x9E3779B97F4A7C15));// UINT64_C(0xBC6EF372FE94F82A));
					//z *= (-(z >> 63u) | UINT64_C(0x00FF00FF00FF00FF)) & UINT64_C(0x369DEA0F31A53F85);
					//return (z ^ z >> 30u);// 26u);


					//uint64_t s = (stream = (stream >> 1 ^ (-(stream & 1) & UINT64_C(0xD800000000000000)))),
					//uint64_t z = (state = (state ^ 0xC74EAD55u) * 0x947E3DB3u);
					//^ ~((stream = (stream >> 1 ^ (-(stream & 1) & UINT64_C(0xD800000000000000)))) & UINT16_C(2)); // & UINT64_C(0x6A5D39EAE116586D)
					//z = (z ^ z >> 26u) * UINT64_C(0xAEF17502108EF2D9);
					//return (z ^ z >> 29u);

					//uint64_t z = (state = state * UINT64_C(0x41C64E6D) + UINT64_C(1));
					//z = ((stream = (stream >> 1 ^ (-(stream & 1) & UINT64_C(0xD800000000000000)))) + (z ^ z >> 32u)) * UINT64_C(0xAEF17502108EF2D9);
					//return (z ^ z >> 30u);

					//philox, roughly
					//uint64_t l = (state += UINT64_C(0x9E3779B97F4A7C15)), r;
					//l = _mulx_u64(l, UINT64_C(0xD2B74407B1CE6E93), &r) + stream;
					//r = _mulx_u64(r ^ l, UINT64_C(0xD2B74407B1CE6E93), &l);
					//return l ^ r;

					//Passes 32TB with only one unusual anomaly, around 4GB
					//const uint64_t l = (state += UINT64_C(0x9E3779B97F4A7C15));
					//uint64_t x = (l ^ l >> 28) * UINT64_C(0xCA5A826395121157), y = (x >> 32 ^ l) * UINT64_C(0xD2B74407B1CE6E93);
					//x += rotate64(y, 19);// (y ^ y >> 32);// +UINT64_C(0xD2B74407B1CE6E93);
					//y += rotate64(x, 13);// (x ^ x >> 32);// +UINT64_C(0xCA5A826395121157);
					//return x ^ y;
					//const uint64_t l = (state += UINT64_C(0x9E3779B97F4A7C15));
					//uint64_t x = (l ^ l >> 28), y = (x ^ l >> 17) * UINT64_C(0xD2B74407B1CE6E93);
					//y += rotate64(x, 19);// (y ^ y >> 32);// +UINT64_C(0xD2B74407B1CE6E93);
					//x += rotate64(y, 13);// (x ^ x >> 32);// +UINT64_C(0xCA5A826395121157);
					//return x ^ y;

					//// needs work
					//uint64_t z = ((++state) ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xD1B54A32D192ED03);
					//z = (z ^ z >> 30u ^ UINT64_C(0xDB4F0B9175AE2165)) * UINT64_C(0xE60E2B722B53AEEB);
					//return (z ^ z >> 28u);

					//uint64_t z = (state = (state ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0xD1B54A32D192ED03));
					//z -= z >> 28;
					//return z ^ z >> 26;
                    //uint64_t z = (state += UINT64_C(0x6C8E9CF570932BD5));
					//z ^= z >> 25;
					//z = rotate64(z, 21) * UINT64_C(0xDB4F0B9175AE2165);
					//return rotate64(z, 21) * UINT64_C(0x9E3779B97F4A7C15);
					
					//uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15));
					//z = (UINT64_C(0xDB4F0B9175AE2165) ^ (z ^ z >> 28)) * UINT64_C(0xD1B54A32D192ED03);
					//z ^= z << 15;
					//return z ^ z >> 21;

					//return s ^ rotate64(s, 11) ^ rotate64(s, 21);
					//s = (state = (z ^ UINT64_C(0xDB4F0B9175AE2165)) * UINT64_C(0x41C64E6B)) - (z >> 28);// 0xD1B54A32D192ED03 0x41C64E6B   0x6C8E9CF570932BD5
					//               s = (state = (z * UINT64_C(0x369DEA0F31A53F85)) + UINT64_C(0x6C8E9CF570932BD5)) - (z >> 27);
					//const uint64_t h = rotate64(l, 29);// +UINT64_C(0xD2B74407B1CE6E93);
					// philox 0xCA5A826395121157 0xD2B74407B1CE6E93
					// neely  0xC6BC279692B5CC83
					// golden 0xD1B54A32D192ED03 0xDB4F0B9175AE2165
					//const uint64_t z = (l ^ l >> 25 ^ UINT64_C(0xDB4F0B9175AE2165)) * UINT64_C(0xC6BC279692B5CC83) + UINT64_C(0x632BE59BD9B4E019);// *UINT64_C(0xDB4F0B9175AE2165);// +UINT64_C(0xCA5A826395121157);
					//const uint64_t z = (l ^ l >> 27 ^ UINT64_C(0xDB4F0B9175AE2165)) * UINT64_C(0xC6BC279692B5CC83);// +UINT64_C(0xCA5A826395121157);
					//return (z ^ z >> 26);
					//return (z ^ rotate64(z, 46) ^ rotate64(z, 19));
					// return (state = ((state *= 0xD2B74407B1CE6E93L) ^ (state << 19 | state >>> 45) ^ (state << 37 | state >>> 27)) * 0xCA5A826395121157L) ^ state >>> 28;
					// return ((s = ((s *= 0x9E3779B97F4A7C15L) ^ s >>> 27 ^ 0xDB4F0B9175AE2165L) * 0xC6BC279692B5CC83L) ^ (s << 11 | s >>> 53) ^ (s << 23 | s >>> 41));
				}
				std::string tiptoe64::get_name() const { return "tiptoe64"; }
				void tiptoe64::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					walker->handle(stream);
					//stream = (stream ^ UINT64_C(0x369DEA0F31A53F85)) * UINT64_C(0x6A5D39EAE116586D) + (state ^ state >> 17) * UINT64_C(0x9E3779B97F4A7C15);
					//stream = stream << 3 ^ UINT64_C(0x369DEA0F31A53F89);
					printf("Seed is 0x%X, Stream is 0x%X\r\n", state, stream);
					//printf("Seed is 0x%I64X\r\n", state);
				}


				// UINT64_C(0x369DEA0F31A53F85); // UINT64_C(0x632BE59BD9B4E019);
				Uint64 linnormA::raw64() {
					//(state = state * UINT64_C(0x41C64E6D) + UINT64_C(1));
					//// DiverRNG, passes 32TB with one anomaly
					//uint64_t z = (state = (state ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0xC6BC279692B5CC83));
					//z = rotate64(z, 27) * UINT64_C(0xDB4F0B9175AE2165);
					//return z ^ (z >> 25u);
					
					//uint64_t z = (rotate64(state, 21) ^ rotate64(state, 41) ^ state++) * UINT64_C(0xC6BC279692B5CC83);
					//z = (z ^ z >> 29) * UINT64_C(0xAEF17502108EF2D9);
					//return z ^ z >> 30;
					//z = rotate64(z, 29) * UINT64_C(0xD1B54A32D192ED03);
					//z = (z ^ z >> 29u) * UINT64_C(0xAEF17502108EF2D9);
					////Early version of DiverRNG.determine(), passes 32TB no anomalies, not very fast
					//uint64_t z = (state++ ^ UINT64_C(0xDB4F0B9175AE2165)) * UINT64_C(0xC6BC279692B5CC83); // golden ratio, neely
					//z = (z ^ z >> 28u ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xD2B74407B1CE6E93); // phi, used in philox
					//return z ^ rotate64(z, 19) ^ rotate64(z, 37); // primes close to 1/3 and 3/5 of 64

					//uint64_t z = (rotate64(state, 19) ^ rotate64(state, 37) ^ state++) * UINT64_C(0xC6BC279692B5CC83);
					//z = (z ^ z >> 30) * UINT64_C(0xDB4F0B9175AE2165); // phi, used in philox
					//return z ^ z >> 29; // primes close to 1/3 and 3/5 of 64

					// fails at 16TB suddenly wih BRank issue
					//uint64_t z = (rotate64(state, 21) ^ rotate64(state, 33) ^ state++ ^ UINT64_C(0xDB4F0B9175AE2165)) * UINT64_C(0xC6BC279692B5CC83); // generalized golden ratio, neely
					//z = (z ^ z >> 28 ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xD2B74407B1CE6E93); // phi, used in philox
					//return z ^ z >> 28;

					uint64_t z = state++;
					z = (rotate64(z, 21) ^ rotate64(z, 35) ^ z ^ UINT64_C(0xDB4F0B9175AE2165)) * UINT64_C(0xC6BC279692B5CC83);
					z = (z ^ z >> 28 ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0xD2B74407B1CE6E93);// +UINT64_C(0x9E3779B97F4A7C15);
					//z = rotate64(z, 31);
					return z ^ z >> 26;

					//z = (z ^ z >> 28 ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xD2B74407B1CE6E93); // phi, used in philox

					//uint64_t z = state++;
					//z = (z << 4) - rotate64(z, 2);
					//z = ((z << 8) - rotate64(z, 6)) * UINT64_C(0xC6BC279692B5CC83);
					//z = (z ^ z >> 27) * UINT64_C(0x9E3779B97F4A7C15);//) * UINT64_C(0xD2B74407B1CE6E93); // phi, used in philox
					//return z ^ z >> 29;
					//z = ((z ^ rotate64(z, 21) ^ rotate64(z, 33) ^ UINT64_C(0xDB4F0B9175AE2165)) * UINT64_C(0xC6BC279692B5CC83));
						// ^ ((rotate64(z, 39) ^ rotate64(z, 11) ^ rotate64(z, 47) ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xD2B74407B1CE6E93));
					//z = (z ^ z >> 29 ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xD2B74407B1CE6E93);
					//z = rotate64(z, 27) * UINT64_C(0x9E3779B97F4A7C15);
					//z = rotate64(z, 29) * UINT64_C(0xD2B74407B1CE6E93);

					//z = (rotate64(z, 43) ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xD2B74407B1CE6E93);
					//z = (rotate64(z, 51) ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0xD1B54A32D192ED03);


					//return (z ^ z >> 30u);

				}
				std::string linnormA::get_name() const { return "LinnormA"; }
				void linnormA::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					printf("Seed is 0x%X, Stream is 0x%X\r\n", state, 1);// stream);
				}

				Uint64 linnormB::raw64() {
					//UINT64_C(0x9E3779B97F4A7C15)
					//uint64_t z = (state = state * UINT64_C(0x41C64E6D) + UINT64_C(3));
					uint64_t z = (state = (state ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0x41C64E6B));
					z = (z ^ z >> 27u) * UINT64_C(0xAEF17502108EF2D9);
					return z ^ (z >> 30u);

					//uint64_t z = ((state += UINT64_C(0x9E3779B97F4A7C15)) ^ UINT64_C(0x369DEA0F31A53F85) * UINT64_C(0x61C8864680B583EB));
					//z = (z ^ z >> 30u) * UINT64_C(0x632BE59BD9B4E019);// UINT64_C(0x369DEA0F31A53F85); // UINT64_C(0xAEF17502108EF2D9);
					//return z ^ (z >> 31u);
				}
				std::string linnormB::get_name() const { return "LinnormB"; }
				void linnormB::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					printf("Seed is 0x%X, Stream is 0x%X\r\n", state, 3);// stream);
				}

				Uint32 linnorm32::raw32() {
					//uint32_t z = (stateA = stateA * UINT32_C(0x41C64E6D) + UINT32_C(1));// + (stateC >> 1 ^ (-(stateC & 1) & UINT32_C(0xA3000000)));//stream);
					//uint32_t y = (stateB = (stateB * UINT32_C(163490618)) % UINT32_C(0x7FFFFFFF)) ^ z;
					//z = (z ^ z >> 16u) * UINT32_C(0xAEF1F2D9); //0xAEF17502108EF2D9
					//y = (y ^ y >> 14u) * UINT32_C(0x94D041EB) + UINT32_C(0x6A5D39E1);
					//return (z ^ z >> 15u) + (y ^ y >> 13u);

					//stateB += UINT64_C(0x9E3779B97F4A7C15);
					//stateA += stateA >> 21;

					//stateA = rotate32(stateA, 13) * UINT32_C(0x89A7);
					//stateB = rotate32(stateB, 17) * UINT32_C(0xBCFD);
					//return stateA ^ stateB;
					return (stateA = rotate32(stateA, 17) * UINT32_C(0xBCFD)) + (stateB = (stateB ^ UINT32_C(0x9E3779BD)) * UINT32_C(0xA952B));// ((stateB += UINT32_C(0x9E3779B9)) | UINT32_C(1));
																																			   //stateC = UINT32_C(0xC3E157C1) - rotate32(stateC, 19);
																																			   //stateB += stateB >> 19;
																																			   //stateA += stateA << 8;
																																			   //stateB += stateB << 11;
																																			   //public class SimpleRandom extends Random {public int a=1,b=1; public int next(int bits){return ((a = (a << 13 | a >>> 19) * UINT32_C(0x89A7)) ^ (b = (b << 17 | b >>> 15) * UINT32_C(0xBCFD))) >>> -bits;}}

				}
				std::string linnorm32::get_name() const { return "linnorm32"; }
				void linnorm32::walk_state(StateWalkingObject *walker) {
					walker->handle(stateA);
					walker->handle(stateB);
					walker->handle(stateC);
					printf("Seed is 0x%X, 0x%X, 0x%X\r\n", stateA, stateB, stateC);// stream);
				}
				Uint16 linnormBounded::raw16() {
					Uint64 z = (state = state * UINT64_C(0x41C64E6D) + UINT64_C(1));
					z = (z ^ z >> 27u) * UINT64_C(0xAEF17502108EF2D9);
					return z ^ (z >> 25u);
					//Uint64 rlow = z & UINT64_C(0xFFFFFFFF);
					//return z;
					//Uint64 low = 0, top = UINT64_C(0x10000);
					//return (rlow >> 16) + (z << 16);

				}
				std::string linnormBounded::get_name() const { return "linnormBounded"; }
				void linnormBounded::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					printf("Seed is 0x%016X\r\n", state);
				}

				Uint64 mingler::raw64() {
					//const uint64_t y = -(stateB & 1);
					//const uint64_t y = (stateB * 0x09BEAB3A);

					// excellent. period is 0xFFFFFFFFFFFFFFFF0000000000000000; a little less than xoroshiro but without BRank issues. Not as fast as xoroshiro128++
					//const uint64_t y = -(stateB & 1);
					//const uint64_t z = (stateA = stateA * UINT64_C(0x41C64E6D) + UINT64_C(1) + (stateB = stateB >> 1u ^ (y & UINT64_C(0xD800000000000000))));
					//return (z ^ z >> 28) + (y ^ UINT64_C(0x9E3779B97F4A7C15));

					// high quality (usually?), not terribly fast
					//stateB ^= stateB >> 21;
					//uint64_t z = (stateA = (stateA ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0x41C64E6B) + (stateB ^= stateB << 9));
					//return (z ^ z >> 28) + (stateB ^= stateB >> 29);

					// where we left off
					//uint64_t y = (stateB += (stateB == UINT64_C(0x61C8864680B583EB) ? UINT64_C(0x3C6EF372FE94F82A) : UINT64_C(0x9E3779B97F4A7C15)));
					//y ^= y >> 28;
					//uint64_t z = (stateA = (stateA ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0x41C64E6B) + y);
					//return (z ^ z >> 28) + y;

					//const uint64_t y = stateB * 0x41C64E6B;
					//return (stateB = rotate64(y, 23)) ^ (stateA += UINT64_C(0x9E3779B97F4A7C15));

					//stateA *= stateA;
					//stateA += (stateB += UINT64_C(0x9E3779B97F4A7C15));
					//stateA ^= stateA >> 31;
					////stateA = rotate64(stateA, 32);
					//return stateA ^ stateB;// ^ (stateB ^ stateB >> 32);

					//return (stateA = (stateA << 5) + rotate32(stateA, 1)) * ((stateB += 0x41C64E6D) | 1);
					return (stateA = (stateA << 5) + rotate64(stateA, 1)) * ((stateB += UINT64_C(0x9E3779B97F4A7C15)) | 1);

					// where we left off
					//stateA *= UINT64_C(0x41C64E6B);
					//stateA = rotate64(stateA, 28);
					//stateB = UINT64_C(0xC6BC279692B5CC8B) - rotate64(stateB, 35);
					//return stateA ^ stateB;
					//end where we left off

					//stateA = stateA * UINT64_C(0x41C64E6D) + UINT64_C(1);
					//stateA += stateA >> 48;
					//stateB += UINT64_C(0x9E3779B97F4A7C15);
					//stateA = UINT64_C(0x9E3779B97F4A7C15) - rotate64(stateA, 42);
					//stateA = rotate64(stateA, 43) - stateA;
					//uint64_t s0 = s[0] * UINT64_C(0x41C64E6B), s1 = s[1];
					//return (s[0] = rotl(s0, 28)) ^ (s[1] = UINT64_C(0xC6BC279692B5CC8B) - rotate64(s1, 35));
					//stateB = UINT64_C(0xC13FA9A902A6328F) - rotate64(stateB, 19);
					//stateB = 3286325185u - rotate32(stateB, 19);
					//stateA += (stateA << 16);
					//stateB += stateB >> 22;
					//stateB += stateB << 5;
					//return z + (stateB ^= stateB >> 29);
					//return (stateA = stateA * UINT64_C(0x41C64E6D) + UINT64_C(1) + (stateB = stateB >> 1u ^ (y & UINT64_C(0xD800000000000000))))
					//	* UINT64_C(0x369DEA0F31A53F85) + (y ^ UINT64_C(0x9E3779B97F4A7C15));
					//return (z ^ z >> 27) + (y ^ UINT64_C(0x9E3779B97F4A7C15));
					// WORKING WELL
					// ((y ^ UINT64_C(0x369DEA0F31A53F85)) | UINT64_C(1))
					//uint64_t y = stateB ^ stateB >> 31;
					//const uint64_t z = (stateA = (stateA * UINT64_C(0x41C64E6D)) + UINT64_C(1)) + (y ^= y << 25);
					//return (z ^ z >> 27u) + (stateB = y ^ y >> 37);

					//YEP
					//uint64_t z = (stateA = (stateA ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0x41C64E6B)) + (stateB = stateB >> 1u ^ (y & UINT64_C(0xD800000000000000)));
					//const uint64_t z = (stateA = (stateA ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0x41C64E6B)) + (y ^= y << 25);
					//???
					//const uint64_t z = (stateA = (stateA * UINT64_C(0x41C64E6D)) + UINT64_C(1)) + (y ^= y << 25);
					//NOPE
					//uint64_t z = (stateA = (stateA ^ UINT64_C(0x6C8E9CF570932BD5)) * UINT64_C(0x5DA942042E4DD58B)) + (stateB = (y & UINT64_C(0x7FFFFFFF))+(y>>31));

					//YEP?
					//uint64_t z = (stateA = (stateA * UINT64_C(0x41C64E6D)) + UINT64_C(1)) + (stateB = stateB >> 1u ^ (y & UINT64_C(0xD800000000000000)));
					//NOPE
					//uint64_t z = (stateA += UINT64_C(0x632BE59BD9B4E019)) + (stateB = stateB >> 1u ^ (y & UINT64_C(0xD800000000000000)));

					//Uint64 z = (stateA += UINT64_C(0x632BE59BD9B4E019)) ^ y;// (stateA += UINT64_C(0x9E3779B97F4A7C15));// (stateA = stateA * UINT64_C(0x41C64E6D) + UINT64_C(1));
					//z *= y | UINT64_C(1);
					//return (z ^ z >> 33) + y;
					//???
					//return (z ^ z >> 27u) + (stateB = y ^ y >> 37);

					// + UINT64_C(0x6A5D39EAE12657AA)
					//const uint64_t
					//	y = (stateB = stateB >> 1 ^ (-(stateB & 1) & UINT64_C(0xD800000000000000))),
					//	x = ((stateA += UINT64_C(0x9E3779B97F4A7C15)) ^ y) * UINT64_C(0xAEF17502108EF2D9);
					//z = ((x ^ x >> 27) + (y ^ y >> 30)) * UINT64_C(0xAEF17502108EF2D9);
					//return (x ^ x >> 25) + y;
					//(stateA = stateA * UINT64_C(0x41C64E6D))
					//z = (z ^ z >> 27u) * UINT64_C(0xC6BC279692B5CC83);
					//z = (z ^ z >> 27u) * UINT64_C(0xAEF17502108EF2D9);
					//return z ^ (z >> 25u);
					//					stateB ^= stateB << 13;
					//					stateB ^= stateB >> 7;
					//					stateB ^= stateB << 17;
					//					uint64_t z = (stateA += UINT64_C(0x9E3779B97F4A7C15));// stateA * UINT64_C(0x41C64E6D) + UINT64_C(1);
					//					uint64_t z = ((stateA += UINT64_C(0x632BE59BD9B4E019)) ^ UINT64_C(0x9E3779B97F4A7C15)) * UINT64_C(0xC6BC279692B5CC83);
					//					z = ((z ^ z >> 27) + _pext_u64(z + 0x9E3779B97F4A7C15, rotate64(z, 41))) * UINT64_C(0xAEF17502108EF2D9);
					//					return z ^ z >> 25;
				}
				std::string mingler::get_name() const { return "mingler"; }
				void mingler::walk_state(StateWalkingObject *walker) {
					walker->handle(stateA);
					walker->handle(stateB);
					//stateB = (stateB & UINT64_C(0x7FFFFFFF)) + (stateB >> 31);
					//if (stateB == 0) stateB = 1;
					printf("stateA is 0x%016X, stateB is 0x%016X\r\n", stateA, stateB);
				}

				Uint32 ta32::raw32() {
					const Uint32 INCR = 0x6C8E9CF5, ADJ = 0x1;
					enum { SHIFT_A = 13, SHIFT_B = 11 };
					stateB ^= stateB >> 6;
					stateA += INCR;
					Uint64 z = (stateA ^ stateA >> SHIFT_A) * ((stateB ^= stateB << 1) | ADJ);
					return (z ^ (z >> SHIFT_B)) + (stateB ^= stateB >> 11);
				}
				std::string ta32::get_name() const { return "ta32"; }
				void ta32::walk_state(StateWalkingObject *walker) {
					walker->handle(stateA);
					walker->handle(stateB);
					stateB |= (stateB == 0);
					printf("Seed is 0x%08X, 0x%08X\r\n", stateA, stateB);
				}


				Uint64 thrust63::raw64() {
					//WORKS WELL, FAST, NOT EQUIDISTRIBUTED
					// fails at 8TB on binrank
					// 0x3C6EF372FE94F82A , 21 , 28 
					// 0x4F1BBCDCBFA53E0A , 23 , 28 these
					// 0x4F1BBCDCBFA53E0A , 21 , 28 all
					// 0x4F1BBCDCBFA53E0A , 23 , 30 fail binrank
					//uint64_t z = (state += UINT64_C(0xBC6EF372FE94F82A));
					//z *= ((z >> 25) ^ z);
					//return (z >> 28) ^ z;

					// passes 32TB, but with issues at the end, no failures though
					uint64_t z = (state += UINT64_C(0x3C6EF372FE94F82A));
					z *= ((z >> 21) ^ z);
					return z - (z >> 29);

					//uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C16)); z *= (z >> 25 ^ z); return z - (z >> 22); // 0x6C6EF372FB7CD85A
																															  //uint64_t z = (state += UINT64_C(0x3C6EF372FE94F82A));
																															  //z *= ((z >> 21) ^ z);
																															  //return z - (z >> 25);

																															  // 0xD91D39EAE12657AA
																															  //0xAD91D39EAE12657A
																															  // 0x4F1BBCDCBFA53E0A maybe?

																															  // 0x6A5D39EAE127586AULL works, but fails some gjrand tests
																															  // 0x6A5D39EAE12657AAULL works on both gjrand and PractRand, with shifts 25 then 22
																															  //const uint64_t s = state;
																															  //const uint64_t z = (s ^ (s >> 25)) * (state += 0x6A5D39EAE12657AAULL);// 0x6A5D39EAE116586AULL);// 0x6C138EB769B56EBAULL); // 0x6A5D39EAE126579AULL
																															  //return z ^ (z >> 22);


																															  //z = (s ^ (s >> 25)) * (j += 0x7C139EB769B97F32ULL);
																															  //z ^= (z >> 25);

				}
				std::string thrust63::get_name() const { return "thrust63"; }
				void thrust63::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					state |= 1;
				}
				void thrust63::seed(Uint64 seed)
				{
//					shift2 = seed & 63u;
//					shift1 = seed >> 8 & 63u;
					autoseed();
					state |= 1;
				}

				Uint64 splitmix64::raw64() {
					//const Uint64 inc = 0x9E3779B97F4A7C15;
					//const Uint64 stage1 = 0xBF58476D1CE4E5B9;
					//const Uint64 stage2 = 0x94D049BB133111EB;
					Uint64 z = (state += 0x9E3779B97F4A7C15);
					z = (z ^ z >> 30) * 0xBF58476D1CE4E5B9;
					z = (z ^ z >> 27) * 0x94D049BB133111EB;
					return z ^ z >> 31;
				}
				std::string splitmix64::get_name() const { return "splitmix64"; }
				void splitmix64::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				Uint64 thrustAlt64::raw64() {
					Uint64 z = (state += UINT64_C(0x6C8E9CF570932BD5));
					z = (z ^ z >> 25) * (z | 0xA529L);
					return z ^ z >> 22;
				}
				std::string thrustAlt64::get_name() const { return "thrustAlt64"; }
				void thrustAlt64::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}
				Uint32 mulberry32::raw32() {
					uint32_t z = (j += 0x6D2B79F5UL) ^ 0x9E3779B5;
					z = (z ^ z >> 11) * 0xFFF + 0x9E3779B5;
					z = (z ^ z >> 13) * 0x1FF - 0x9E3779B5;
					z = (z ^ z >> 12) * 0x3FF + 0x9E3779B5;
					z = (z ^ z >> 14) * 0x7FF - 0x9E3779B5;
					return z ^ z >> 15;

					//Uint32 z = (j += 0x6D2B79F5UL);
					//z = (z ^ (z >> 15)) * (z | 61UL);
					//z ^= z >> 8;
					//z += ((z ^ (z >> 7)) * (j | 0xA529UL));
					//return z ^ (z >> 14);
				}
				std::string mulberry32::get_name() const { return "mulberry32"; }
				void mulberry32::walk_state(StateWalkingObject *walker) {
					walker->handle(j);
				}

				Uint64 vortex::raw64() {
					/*
					uint64_t z = (state += UINT64_C(0x6C8E9CF570932BD5)) ^ ((stream *= UINT64_C(0x2545F4914F6CDD1D)) >> 30);
					z ^= (z >> 25) * (stream + UINT64_C(0x6A5D39EAE12657BA));
					return z ^ (z >> 28) ^ (state & stream);
					*/
					uint64_t z = (state += UINT64_C(0x6C8E9CF570932BD5));
					z ^= (z >> 26) * (stream *= UINT64_C(0x2545F4914F6CDD1D));
					return z ^ (~z) >> 26; //UINT64_C(0x9E3779B97F4A7BB5) - 
				}
				std::string vortex::get_name() const { return "vortex"; }
				void vortex::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					walker->handle(stream);
					stream |= 1;
				}
				void vortex::seed(Uint64 seed)
				{
					state = seed & 0xFFFFUL;
					state += UINT64_C(0x6C8E9CF570932BD5);
					state = (state ^ state >> 25) * (state | UINT64_C(0xA529));
					state ^= state >> 22;

					seed >>= 16;

					seed += UINT64_C(0x6C8E9CF570932BD5);
					seed = (seed ^ seed >> 25) * (seed | UINT64_C(0xA529));
					stream = (seed ^ seed >> 22) | UINT16_C(1);
				}

				Uint64 vortex2::raw64() {
					// 0xA529 UINT64_C(0x9E3779B97F4A7BB5)
					state += UINT64_C(0x6C8E9CF570932BD5);
					uint64_t z = (state ^ state >> 25) * ((stream += UINT64_C(0x9E3779B97F4A7BB5)) | UINT64_C(1));
					return (z ^ (z >> 28));
				}
				std::string vortex2::get_name() const { return "vortex2"; }
				void vortex2::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
					walker->handle(stream);
				}
				void vortex2::seed(Uint64 seed)
				{
					state = seed;
					seed += UINT64_C(0x6C8E9CF570932BD5);
					seed = (seed ^ seed >> 25) * (seed | UINT64_C(0xA529));
					stream = seed ^ seed >> 22;
				}
				Uint64 vortex64::raw64() {
					//with just multiply
					//0x2545F4914F6CDD1D
					//with odd increment
					//0x369DEA0F31A53F85

					//0x6C8E9CF570932BD5 0x6C8E9CF970932BD5

					Uint64 z = (state += UINT64_C(0x9E3779B97F4A7C15)); //0x6C8E9CF570932BD5
																		//z = (z ^ z >> 25);// *UINT64_C(0x2545F4914F6CDD1D);
																		//z ^= z >> 13;
																		//z ^= z << 7;

																		//// passes at least 8TB no anomalies
																		//z = (z ^ z >> 13) * UINT64_C(0x7FFFF);// *UINT64_C(0xAEF17502108EF2D9);// 0x2545F4914F6CDD1D 0x9E3779B97F4A7C15
																		//z = (z ^ z >> 12) * UINT64_C(0x1FFFF);
																		//z = (z ^ z >> 14) * UINT64_C(0x1FFF);
																		//return z ^ z >> 24;

																		//z ^= z >> 9;
																		//z -= (z << 13) + UINT64_C(0xAEF17502108EF2D9);
																		//z ^= z >> 7;
																		//z -= (z << 11) + UINT64_C(0x6C8E9CF570932BD5);
					z ^= z >> 9 ^ 0x94D049BB133111EB;
					z -= (z << 13) + UINT64_C(0xAEF17502108EF2D9);
					z ^= z >> 7 ^ 0xBF58476D1CE4E5B9;
					z -= (z << 11) + UINT64_C(0x6C8E9CF570932BD5);

					//z -= z << 17;
					//z -= z << 19;
					//z = ((z << 14) - rotate64(z, 12));
					//z = ((z << 20) - rotate64(z, 17));
					return z ^ z >> 25; //((z >> 22) ^ rotate64(z, 44))
										//z ^= ((z << 19) | (z >> 45)) ^ ((z << 53) | (z >> 11));
										/*
										long z = (state += 0x9E3779B97F4A7C15L);
										z = (z ^ z >> 13) * 0x7FFFF;
										z = (z ^ z >> 12) * 0x1FFFF;
										z = (z ^ z >> 14) * 0x1FFF;
										return z ^ z >> 24;
										*/

										//Uint64 a = (state = state * Uint64(0x369DEA0F31A53F85) + Uint64(0x6C8E9CF970932BD5));

										//Uint64 a = (state += stream);
										////a ^= rotate64(a, a & 63) ^ rotate64(a, a + 39 & 63);
										//a = (a ^ a >> 24) * Uint64(0x2545F4914F6CDD1D);
										//return a ^ a >> 25;


										//Uint64 a = (state += 0x6C8E9CF970932BD5L);
										//Uint64 b = _byteswap_uint64(a);
										//a = (a ^ a >> 25) * Uint64(0x2545F4914F6CDD1D);
										//b ^= b >> 22 ^ a ^ a >> 28;
										//return (b ^ b >> 25);



										//z = (z ^ _byteswap_uint64(z) ^ z >> 25) * Uint64(0x2545F4914F6CDD1D);
										//return (z ^ z >> 22);

										//					Uint64 z = ((state ^ state >> 25)) * UINT64_C(0x59071D96D81ECD35);
										//					return z ^ ((z << 12) | (z >> 52)) ^ ((z << 47) | (z >> 17)) ^ _byteswap_uint64(state += stream);
										//					return z ^ z >> 25 ^ ((z << 12) | (z >> 52)) ^ ((z << 47) | (z >> 17));
										//Uint64 z = (state ^ state >> 25) ^ ((state << 43) | (state >> 21)) ^ ((state << 17) | (state >> 47));
										//return -z ^ ((z << 19) | (z >> 45)) ^ ((z << 53) | (z >> 11));

										//return z ^ z >> 25;

										//passes 32TB with no anomalies (!!!) with stream 12 and seed 0x33f3ca7e
										//state += stream; //0x6C8E9CF570932BD5 0x6C8E9CF970932BD5
										//Uint64 z = (state ^ state >> 25) * UINT64_C(0x2545F4914F6CDD1D);
										//z ^= ((z << 19) | (z >> 45)) ^ ((z << 53) | (z >> 11));
										//return z ^ z >> 25;

										//works at 16TB, probably further (very few anomalies, none worse than unusual)
										//state += stream;
										//Uint64 z = (state ^ state >> 25) * (state | Uint64(0xA529));
										//return z ^ z >> 22;

										// works, passes PractRand at 32TB with stream upper bits 12
										//state += stream;
										//Uint64 z = (state ^ state >> 25) * (state | Uint64(0xA529));
										//return ((z << 43) | (z >> 21)) ^ (((z << 17) | (z >> 47)) * Uint64(0x6C8E9CF570932BD5));

										// also works at 32TB
										//state += stream;
										//Uint64 z = (state ^ state >> 25) * (state | Uint64(0xA529));
										//return z ^ ((z << 42) | (z >> 22)) ^ ((z << 25) | (z >> 39));


										//older
										//Uint64 z = (state ^ state >> 25) * ((state & Uint64(0xFFFFFFF8)) ^ Uint64(0x9E3779B97F4A7C15));

				}
				std::string vortex64::get_name() const {
					std::ostringstream tmp;
					tmp << "vortex64x" << (int)((stream ^ Uint64(0x6C8E9CF570932BD5)) >> 32);
					return tmp.str();
				}
				void vortex64::walk_state(StateWalkingObject *walker) {
					walker->handle(state);
				}


				Uint64 molerat64::raw64() {
					//surprisingly, passes 32TB with one anomaly, seed = 0xde9b705d
					return (a = rotate64(a, 21) * UINT64_C(0x9E3779B9)) * UINT64_C(0x41C64E6D);
					// 0xAEF17502108EF2D9; 0x9E3779B97F4A7AF5
					//return (z << 29) + rotate64(z, 20);
					//z = (z ^ z >> 28);
					//return z;
				}
				std::string molerat64::get_name() const { return "molerat64"; }
				void molerat64::walk_state(StateWalkingObject *walker) {
					walker->handle(a);
					uint64_t r = a;

					//a = UINT64_C(0x9B1B51BEB2EFF7A1); //0x41C64E6B
					//for (uint64_t ra = (r & 0xFFFF); ra; ra--)
					//{
					//	a = rotate64(a, 20) + (a << 27);
					//}
					a = UINT64_C(0x9E3779B9); //0x41C64E6B
					for (uint64_t ra = (r & 0xFFFF); ra; ra--)
					{
						a = rotate64(a, 21) * UINT64_C(0x9E3779B9);
					}

				}

				Uint64 moverCounter64::raw64() {
					//passes 32TB with no anomalies, seed = 0x8b2bfcb
					//return (a = rotate64(a, 21) * UINT64_C(0x9E3779B9)) * (b += UINT64_C(0x9E3779B97F4A7AF6));// *((b += UINT64_C(0x9E3779B97F4A7AF5)) | UINT64_C(1));
                    // 0xAEF17502108EF2D9; 0x9E3779B97F4A7AF5 0xC6BC279692B5CC83 0x6C8E9CF570932BD5
					//return (b += (a = rotate64(a, 21) * UINT64_C(0x9E3779B9)) ^ UINT64_C(0x9E3779B97F4A7AF5));//* UINT64_C(0x41C64E6D)); //
					//return (a = (rotate64(a, 21) ^ (b += UINT64_C(0x9E3779B97F4A7AF5))) * UINT64_C(0xC6BC279692B5CC83)) * UINT64_C(0x41C64E6B);
					return (a = rotate64(a, 21) * (b += UINT64_C(0x9E3779B97F4A7AF6))) * UINT64_C(0x41C64E6B);
				}
				std::string moverCounter64::get_name() const { return "moverCounter64"; }
				void moverCounter64::walk_state(StateWalkingObject *walker) {
					walker->handle(a);
					walker->handle(b);
					b |= UINT64_C(1);
					//b = b << 3 | UINT64_C(5);

					//uint64_t r = a;

					//////a = UINT64_C(0x9B1B51BEB2EFF7A1); //0x41C64E6B
					//////for (uint64_t ra = (r & 0xFFFF); ra; ra--)
					//////{
					//////	a = rotate64(a, 20) + (a << 27);
					//////}
					//a = UINT64_C(0x9E3779B9); //0x41C64E6B 0x9E3779B9
					//for (uint64_t ra = (r & 0xFFFF); ra; ra--)
					//{
					//	a = rotate64(a, 21) * UINT64_C(0x9E3779B9);
					//}

				}

				Uint64 xrsr_rev_mul::raw64() {
					const Uint64 s0 = state0;
					Uint64 s1 = state1;
					const Uint64 result = s1 * UINT64_C(0x369DEA0F31A53F85);
					s1 ^= s0;
					state0 = rotate64(s0, 24) ^ s1 ^ (s1 << 16); // a, b
					state1 = rotate64(s1, 37); // c
					return (result ^ result >> 25) + s0;
				}
				std::string xrsr_rev_mul::get_name() const { return "xrsr_rev_mul"; }
				void xrsr_rev_mul::walk_state(StateWalkingObject *walker) {
					walker->handle(state0);
					walker->handle(state1);
				}
				Uint32 twirl32::raw32() {

					//a += 0x62BD5;
					//Uint32 z = (a ^ a >> 13) * ((a & 0xFFFF8) ^ 0xCD7B5);// +0x64B05;
					//return (((z << 7) | (z >> 25)) - a) ^ (z >> 13);

					//const Uint32 INCR = 0x70932BD5, ADJ = 0x63C5, MUL = 0x2C9277B5, PLUS = 0xAC564B05;
					//enum { SHIFT_A = 13, SHIFT_B = 13, ROT = 7 };

					//return rotate32(z, ROT) ^ (z >> SHIFT_B);
					//return (z ^ z >> SHIFT_B);

					//enum {
					//	X = 0xC74EAD55,//0x632BE5AD,//must end in 5 or D
					//	Y = 0x632BE5AD,
					//	M = 0x93DB3,//must end in 3 or B
					//	N = 0xDAD7B,
					//};
					enum {
						X = 0xC74EAD55,//0x632BE5AB,//must end in 5 or D
						M = 0xD3AB3,//must end in 3 or B
					};
					Uint32 z = (a = (a ^ X) * M);
					z ^= z >> 16;
					z ^= z << 7;
					z = (z ^ z >> 11) * Uint32(0xEF52D);
					return z ^ z >> 13;
					/*
					Uint32 z = (a += Uint32(0x6A52D));
					z = (z ^ ((z << 11) | (z >> 21)) ^ ((z << 28) | (z >> 4)) ^ ((z << 5) | (z >> 27)) ^ ((z << 19) | (z >> 13))) * 0xAD7B5;
					z ^= z >> 15;
					z ^= z << 13;
					z ^= z >> 14;
					z = z * Y + N;
					z = (z ^ ((z << 12) | (z >> 20)) ^ ((z << 17) | (z >> 15)) ^ ((z << 22) | (z >> 10)) ^ ((z << 7) | (z >> 25))) * 0x62BD5;
					z ^= z >> 3;
					z ^= z << 23;
					z ^= z >> 25;
					z = (z ^ X) * M;
					return z ^ z >> 16;
					*/
					/* // left off here, fails tests early
					Uint32 z = a += Uint32(0x62BD5);
					z ^= ((z << 11) | (z >> 21)) ^ ((z << 18) | (z >> 14));
					//z = z * Uint32(0xBE5AD) + Uint32(0x6A529);
					z = (z ^ z >> 14) * Uint32(0xAD7B5);
					z = (z ^ z >> 15) * Uint32(0xCDD1D);
					z ^= ((z << 9) | (z >> 23)) ^ ((z << 19) | (z >> 13));
					z = (z ^ z >> 13) * Uint32(0x74C6D);
					return z ^ (z >> 15);
					*/
					//z ^= ((z << 13) | (z >> 19)) ^ ((z << 7) | (z >> 25));
					//z ^= z >> 11;
					//z ^= z << 21;
					//z = (z ^ z >> 13) * 0xCD7B5;
					//return z ^ z >> 13;

					//					z ^= ((z << 21) | (z >> 11)) ^ ((z << 15) | (z >> 17));


					//state += stream; //0x6C8E9CF570932BD5 0x6C8E9CF970932BD5
					//Uint64 z = (state ^ state >> 25) * UINT64_C(0x2545F4914F6CDD1D);
					//z ^= ((z << 19) | (z >> 45)) ^ ((z << 53) | (z >> 11));
					//return z ^ z >> 25;


					//Uint32 state = ++a * 0x62BD5 ^ 0x9E3779B9;
					//Uint32 z = (state ^ state >> 13) * ((state & 0xFFFF8) ^ 0xCD7B5) ^ 0x632BE5AB;
					//return ((z << 21) | (z >> 11)) ^ (((z << 7) | (z >> 25)) * 0x62BD5);

					//return Integer.rotateLeft((state = ((state = state * 0x62BD5 ^ 0x9E3779B9) ^ state >>> 13) * ((state & 0xFFFF8) ^ 0xCD7B5) ^ 0x632BE5AB), 21) ^ (Integer.rotateLeft(state, 7) * 0x62BD5 + 0x932BD);
				}
				std::string twirl32::get_name() const { return "twirl32"; }
				void twirl32::walk_state(StateWalkingObject *walker) {
					walker->handle(a);
					//if (!a) a = 1;
				}
				Uint32 zig32::raw32() {
					/*
					enum {
					X = 0xC74EAD55,//must end in 5 or D
					Y = 0x70932BD5,
					M = 0x947E3DB3,//0x94DB3,//must end in 3 or B
					N = 0xDB3947E3,//0xC564B,
					};
					a = (a ^ X) * M;
					b = (b ^ Y) * N;
					*/
					enum {
						X = 0xC74EAD55,//0x632BE5AB,//must end in 5 or D
						M = 0xA5CB3,//must end in 3 or B
					};
					// doesn't work well at all
					//a ^= a >> 13;
					//a ^= a << 5;
					//a ^= a >> 19;
					//return (a ^ a >> 10) + (b *= 0xE5CB5);

					//LCG style
					//Uint32 z = (b = b * 0xA5CB5 + 0xC74EAD53);
					//XLCG style
					//Uint32 z = (b = (b ^ 0xC74EAD55) * 0xA5CB3);

					// Zog, passes 32TB with 1 "unusual"
					Uint32 z = (b += 0xC74EAD55);
					a ^= a >> 14;
					z = (z ^ z >> 10) * 0xA5CB3;
					a ^= a >> 15;
					return (z ^ z >> 20) + (a ^= a << 13);


					// passes 32TB with 2 "unusual"
					//Uint32 z = (b = (b ^ 0xC74EAD55) * 0xA5CB3);
					//a ^= a >> 14;
					//z ^= z >> 10;
					//a ^= a >> 15;
					//return (z ^ z >> 20) + (a ^= a << 13);

					// passes 32TB with 3 "unusual"
					// X is 0xC74EAD55
					// M is 0xA5CB3
					/*
					Uint32 z = (b = (b ^ X) * M);
					a ^= a >> 14;
					z ^= z >> 13;
					a ^= a >> 15;
					z = (z ^ z >> 11) + (a ^= a << 13);
					return (z ^ z >> 7);
					*/

					/*
					Uint32 z = (b = (b ^ X) * M);
					a ^= a >> 14;
					z ^= z >> 16;
					a ^= a << 1;
					z ^= z << 7;
					return (z ^ z >> 11) * 0x94DB5 + (a ^= a >> 15);
					*/

					//z = (z ^ z << 7) + (a ^= a >> 15);
					//return z ^ z >> 13; // does not work well with 14
					//z += (z << 16) + (a ^= a >> 15); // unsure if this works
					//z = (z ^ z >> 12) + (a ^= a >> 15); // 64GB pass
					//z = (z ^ z >> 13) + (a ^= a >> 15); // has worked with z >> 12

					// where we left off, passes at least 4 TB
					/*
					Uint32 z = (b += 0x632BE5AB);
					a ^= a >> 14;
					z = (z ^ z >> 15) * Uint32(0xAD7B5);
					a ^= a << 1;
					z = (z ^ z >> 12) * Uint32(0xCDD1D) + (a ^= a >> 15);
					return z ^ z >> 13;
					*/
					/*
					enum {
					X = 0xC74EAD55,//0x632BE5AB,//must end in 5 or D
					M = 0x93DB3,//must end in 3 or B
					};
					Uint32 z = (b = (b ^ X) * M);
					a ^= a >> 14;
					z ^= z >> 15;
					a ^= a << 1;
					z += (a ^= a >> 15) ^ (z ^ z >> 12) * UINT32_C(0xCDD1D);
					return z ^ z >> 13;
					*/

					/* // passes 32TB with only one anomaly, "unusual" on [Low1/32]FPF-14+6/16:all2 at 1TB
					Uint32 z = (b += 0x632BE5AB);
					a ^= a >> 14;
					z = (z ^ z >> 14) * Uint32(0xAD7B5);
					a ^= a << 1; // 13 maybe?
					z = (z ^ z >> 15) * Uint32(0xCDD1D);
					z += (a ^= a >> 15);
					return z ^ z >> 13; // maybe 16 ?

					*/

					//a += 0x62BD5;
					//Uint32 z = (a ^ a >> 13) * ((a & 0xFFFF8) ^ 0xCD7B5);// +0x64B05;
					//return (((z << 7) | (z >> 25)) - a) ^ (z >> 13);

					//const Uint32 INCR = 0x70932BD5, ADJ = 0x63C5, MUL = 0x2C9277B5, PLUS = 0xAC564B05;
					//enum { SHIFT_A = 13, SHIFT_B = 13, ROT = 7 };

					//return rotate32(z, ROT) ^ (z >> SHIFT_B);
					//return (z ^ z >> SHIFT_B);


					// works, passes 32TB of tests
					/*
					a ^= a << 7;
					a ^= a >> 1;
					a ^= a << 9;
					b += 0x632BE5AB;
					Uint32 z = (a + b ^ a - b >> 14) * ((b & 0xFFFF8) ^ 0xCD7B5);
					return z ^ z >> 13 ^ a;
					*/
					//b = b * ((a & 0xFFFF8) ^ 0xCD7B5) + (a & 0x62BD5);
					//return b ^ (b - a) >> 13 ^ a;

					// where we left off
					/*
					a ^= a << 1;
					a ^= a >> 27;
					b += 0x632BE5AB;
					a ^= a << 27;
					Uint32 z = (b + a ^ b - a >> 14) * ((a & 0xFFFFE) ^ 0xCD7B5);
					return z ^ z >> 13;
					*/

					//return (z + b ^ a >> 13) ^ ((z + a ^ b >> 11) ^ (b + a ^ z >> 12));
					//Uint32 z = (b ^ b - a >> 13);
					//return ((z << 21) | (z >> 11)) ^ ((z << 7) | (z >> 25)) * ((a & 0xFFFF8) ^ 0xCD7B5);

					//return (a ^ b >> 10) ^ (b - a >> 13) ^ a * (0xA529 | (b >> 12));
					//Uint32 z = (a ^ a >> 13) * 0x62BD5;
					//return (a ^ z) + ((a << 7) | (a >> 25)) ^ ((z << 22) | (z >> 10));

					//Uint32 state = ++a * 0x62BD5 ^ 0x9E3779B9;
					//Uint32 z = (state ^ state >> 13) * ((state & 0xFFFF8) ^ 0xCD7B5) ^ 0x632BE5AB;
					//return ((z << 21) | (z >> 11)) ^ (((z << 7) | (z >> 25)) * 0x62BD5);

					//return Integer.rotateLeft((state = ((state = state * 0x62BD5 ^ 0x9E3779B9) ^ state >>> 13) * ((state & 0xFFFF8) ^ 0xCD7B5) ^ 0x632BE5AB), 21) ^ (Integer.rotateLeft(state, 7) * 0x62BD5 + 0x932BD);
				}
				std::string zig32::get_name() const { return "zig32"; }
				void zig32::walk_state(StateWalkingObject *walker) {
					walker->handle(a);
					if (a == 0) a = 1;
					walker->handle(b);
				}

			}
		}
	}
}