#define _CRT_SECURE_NO_DEPRECATE 1

#define R128_IMPLEMENTATION
#include "../r128.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int testsRun, testsFailed;

#if defined(_MSC_VER) && !defined(NDEBUG)
#  include <Windows.h>
#  define PRINT_FAILURE(fmt, ...) do { \
   char buf[1024]; \
   sprintf(buf, fmt, __VA_ARGS__); \
   fputs(buf, stderr); \
   OutputDebugStringA(buf); \
} while(0)
#else
#  define PRINT_FAILURE(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#endif

#define R128_TEST_EQ(v1, v2) do { \
   ++testsRun; \
   if ((v1).lo != (v2).lo || (v1).hi != (v2).hi) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: Got 0x%08x%08x.%08x%08x, expected 0x%08x%08x.%08x%08x\n", \
         __FILE__, __LINE__, R128_R3(&(v1)), R128_R2(&(v1)), R128_R1(&(v1)), R128_R0(&(v1)), \
         R128_R3(&(v2)), R128_R2(&(v2)), R128_R1(&(v2)), R128_R0(&(v2))); \
      ++testsFailed; \
   }\
} while(0)

#define R128_TEST_EQ2(v, r0, r1) do { \
   ++testsRun; \
   if ((v).lo != r0 || (v).hi != r1) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: Got 0x%08x%08x.%08x%08x, expected 0x%08x%08x.%08x%08x\n", \
         __FILE__, __LINE__, R128_R3(&(v)), R128_R2(&(v)), R128_R1(&(v)), R128_R0(&(v)), \
         (uint32_t)((r1) >> 32), (uint32_t)(r1), (uint32_t)((r0) >> 32), (uint32_t)(r0)); \
      ++testsFailed; \
   }\
} while(0)

#define R128_TEST_EQ4(v, r0, r1, r2, r3) do { \
   ++testsRun; \
   if ((v).lo != (((uint64_t)(r1) << 32) | (r0)) || (v).hi != (((uint64_t)(r3) << 32) | (r2))) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: Got 0x%08x%08x.%08x%08x, expected 0x%08x%08x.%08x%08x\n", \
         __FILE__, __LINE__, R128_R3(&(v)), R128_R2(&(v)), R128_R1(&(v)), R128_R0(&(v)), \
         (r3), (r2), (r1), (r0)); \
      ++testsFailed; \
   }\
} while(0)

#define R128_TEST_STRSTREQ(s1, s2) do { \
   ++testsRun; \
   if (strcmp(s1, s2)) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: \"%s\" != \"%s\"\n", \
         __FILE__, __LINE__, s1, s2); \
      ++testsFailed; \
   } \
} while(0)

#define R128_TEST_STREQ(r, s) do { \
   R128 rs; \
   r128FromString(&rs, (s), NULL); \
   R128_TEST_EQ(r, rs); \
} while(0)

#define R128_TEST_FLEQ(r, f) do { \
   char bufr[64], buff[64]; \
   r128ToStringf(bufr, sizeof(bufr), "%#.18f", &(r));\
   sprintf(buff, "%#.18f", (double)(f)); \
   R128_TEST_STRSTREQ(bufr, buff); \
} while(0)

#define R128_TEST_FLFLEQ(f1, f2) do { \
   char buf1[64], buf2[64]; \
   sprintf(buf1, "%.18f", (double)(f1)); \
   sprintf(buf2, "%.18f", (double)(f2)); \
   R128_TEST_STRSTREQ(buf1, buf2); \
} while(0)

static void test_float()
{
   double a;
   double b;
   R128 c;

   a = -2.125;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);

   a = 0.25;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);

   a = -0.25;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);

   a = -2.5;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);

   a = 2.118018798719000910681;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);

   a = 2.918018798719000910681;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);

   a = 2.518018798719000910681;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);

   a = -2.118018798719000910681;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);

   a = -2.918018798719000910681;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);

   a = 2.518018798719000910681;
   r128FromFloat(&c, a);
   R128_TEST_FLEQ(c, a);
   b = r128ToFloat(&c);
   R128_TEST_FLFLEQ(b, a);
}

static void test_string()
{
   char bufa[256];
   char bufb[256];
   R128 a;
   double b;

   b = 0.9999;
   r128FromFloat(&a, b);

   sprintf(bufb, "%5.6f", b);
   r128ToStringf(bufa, sizeof(bufa), "%5.6f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   sprintf(bufb, "%1.0f", b);
   r128ToStringf(bufa, sizeof(bufa), "%1.0f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   sprintf(bufb, "%# 3.0f", b);
   r128ToStringf(bufa, sizeof(bufa), "%# 3.0f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   sprintf(bufb, "%-20.4f", b);
   r128ToStringf(bufa, sizeof(bufa), "%-20.4f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   sprintf(bufb, "%#+.50f", b);
   r128ToStringf(bufa, sizeof(bufa), "%#+.50f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);

   b = (1 / 18446744073709551616.0);
   r128FromFloat(&a, b);

   sprintf(bufb, "%1.200f", b);
   r128ToStringf(bufa, sizeof(bufa), "%1.200f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
}

static void test_cmp()
{
   R128 a = { R128_LIT_U64(0x8000000000000000), 1 };     //1.5
   R128 b = { R128_LIT_U64(0x4000000000000000), 1 };     //1.25
   R128 c = { R128_LIT_U64(0x8000000000000000), R128_LIT_U64(0xffffffffffffffff) };    //-1.5
   R128 d = { R128_LIT_U64(0x4000000000000000), R128_LIT_U64(0xffffffffffffffff) };    //-1.25
   int cmp;

   cmp = r128Cmp(&a, &a);
   R128_TEST_FLFLEQ(cmp, 0);
   cmp = r128Cmp(&a, &b);
   R128_TEST_FLFLEQ(cmp, 1);
   cmp = r128Cmp(&a, &c);
   R128_TEST_FLFLEQ(cmp, 1);
   cmp = r128Cmp(&a, &d);
   R128_TEST_FLFLEQ(cmp, 1);
   cmp = r128Cmp(&b, &a);
   R128_TEST_FLFLEQ(cmp, -1);
   cmp = r128Cmp(&b, &b);
   R128_TEST_FLFLEQ(cmp, 0);
   cmp = r128Cmp(&b, &c);
   R128_TEST_FLFLEQ(cmp, 1);
   cmp = r128Cmp(&b, &d);
   R128_TEST_FLFLEQ(cmp, 1);
   cmp = r128Cmp(&c, &a);
   R128_TEST_FLFLEQ(cmp, -1);
   cmp = r128Cmp(&c, &b);
   R128_TEST_FLFLEQ(cmp, -1);
   cmp = r128Cmp(&c, &c);
   R128_TEST_FLFLEQ(cmp, 0);
   cmp = r128Cmp(&c, &d);
   R128_TEST_FLFLEQ(cmp, 1);
   cmp = r128Cmp(&d, &a);
   R128_TEST_FLFLEQ(cmp, -1);
   cmp = r128Cmp(&d, &b);
   R128_TEST_FLFLEQ(cmp, -1);
   cmp = r128Cmp(&d, &c);
   R128_TEST_FLFLEQ(cmp, -1);
   cmp = r128Cmp(&d, &d);
   R128_TEST_FLFLEQ(cmp, 0);
}

static void test_div()
{
   R128 a, b, c;

   R128_SET2(&a, R128_LIT_U64(0x000022ef000023510), R128_LIT_U64(97276714306369));
   R128_SET2(&b, R128_LIT_U64(0x00006b3700000000), R128_LIT_U64(23806));
   r128Div(&c, &a, &b);
   R128_TEST_STREQ(c, "4086226761.331427244340973807968245");

   R128_SET2(&a, 0, 10);
   R128_SET2(&b, 0, 3);
   r128Div(&c, &a, &b);
   R128_TEST_STREQ(c, "3.333333333333333333333");

   R128_SET2(&a, 0, R128_LIT_U64(2113123919594));
   R128_SET2(&b, 0, R128_LIT_U64(0xffffffffffffd159));
   r128Div(&c, &a, &b);
   R128_TEST_STREQ(c, "-176934096.92656786402076530185046");

   R128_SET2(&a, 0, R128_LIT_U64(62727997390472));
   R128_SET2(&b, 0, R128_LIT_U64(154));
   r128Div(&c, &a, &b);
   R128_TEST_STREQ(c, "407324658379.68831168831168831169");

   r128Copy(&a, &R128_one);
   r128Shr(&a, &a, 2);
   r128Div(&c, &a, &R128_min);
   R128_TEST_FLEQ(c, 0);

   r128Div(&c, &R128_one, &R128_smallest);
   R128_TEST_EQ(c, R128_max);
}

static void test_mod()
{
   R128 a, b, c;

   r128FromFloat(&a, 5.3);
   r128FromFloat(&b, 2);
   r128Mod(&c, &a, &b);
   R128_TEST_FLEQ(c, fmod(5.3, 2));

   r128FromFloat(&a, -18.5);
   r128FromFloat(&b, 4.2);
   r128Mod(&c, &a, &b);
   R128_TEST_FLEQ(c, fmod(-18.5, 4.2));
}

static void test_shift()
{
   R128 a, b;

   R128_SET2(&a, 5, 0);
   r128Shl(&b, &a, 1);
   R128_TEST_EQ4(b, 0x0000000a, 0x00000000, 0x00000000, 0x00000000);
   r128Shl(&b, &a, 33);
   R128_TEST_EQ4(b, 0x00000000, 0x0000000a, 0x00000000, 0x00000000);
   r128Shl(&b, &a, 65);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0x0000000a, 0x00000000);
   r128Shl(&b, &a, 97);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0x00000000, 0x0000000a);

   R128_SET2(&a, 5, 0);
   r128Shl(&b, &a, 0);
   R128_TEST_EQ4(b, 0x00000005, 0x00000000, 0x00000000, 0x00000000);
   r128Shl(&b, &a, 32);
   R128_TEST_EQ4(b, 0x00000000, 0x00000005, 0x00000000, 0x00000000);
   r128Shl(&b, &a, 64);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0x00000005, 0x00000000);
   r128Shl(&b, &a, 96);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0x00000000, 0x00000005);

   R128_SET4(&a, 0, 0, 0, 0xa0000000);
   r128Shr(&b, &a, 1);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0x00000000, 0x50000000);
   r128Shr(&b, &a, 33);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0x50000000, 0x00000000);
   r128Shr(&b, &a, 65);
   R128_TEST_EQ4(b, 0x00000000, 0x50000000, 0x00000000, 0x00000000);
   r128Shr(&b, &a, 97);
   R128_TEST_EQ4(b, 0x50000000, 0x00000000, 0x00000000, 0x00000000);

   r128Shr(&b, &a, 0);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0x00000000, 0xa0000000);
   r128Shr(&b, &a, 32);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0xa0000000, 0x00000000);
   r128Shr(&b, &a, 64);
   R128_TEST_EQ4(b, 0x00000000, 0xa0000000, 0x00000000, 0x00000000);
   r128Shr(&b, &a, 96);
   R128_TEST_EQ4(b, 0xa0000000, 0x00000000, 0x00000000, 0x00000000);

   r128Sar(&b, &a, 1);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0x00000000, 0xd0000000);
   r128Sar(&b, &a, 33);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0xd0000000, 0xffffffff);
   r128Sar(&b, &a, 65);
   R128_TEST_EQ4(b, 0x00000000, 0xd0000000, 0xffffffff, 0xffffffff);
   r128Sar(&b, &a, 97);
   R128_TEST_EQ4(b, 0xd0000000, 0xffffffff, 0xffffffff, 0xffffffff);

   r128Sar(&b, &a, 0);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0x00000000, 0xa0000000);
   r128Sar(&b, &a, 32);
   R128_TEST_EQ4(b, 0x00000000, 0x00000000, 0xa0000000, 0xffffffff);
   r128Sar(&b, &a, 64);
   R128_TEST_EQ4(b, 0x00000000, 0xa0000000, 0xffffffff, 0xffffffff);
   r128Sar(&b, &a, 96);
   R128_TEST_EQ4(b, 0xa0000000, 0xffffffff, 0xffffffff, 0xffffffff);
}

int main()
{
   R128 a, b, c;

   a.hi = 97276714306369ull;
   a.lo = 0x000022ef000023510ull;
   b.hi = 23806;
   b.lo = 0x00006b3700000000ull;
   r128Div(&c, &a, &b);

   test_float();
   test_string();
   test_cmp();
   test_mod();
   test_div();
   test_shift();

   printf("%d tests run. %d tests passed. %d tests failed.\n",
      testsRun, testsRun - testsFailed, testsFailed);

   return testsFailed;
}
