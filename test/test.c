#define _CRT_SECURE_NO_DEPRECATE 1

//#define R128_ASSERT(x)
//#define R128_STDC_ONLY
#define R128_IMPLEMENTATION
#ifndef NDEBUG
#  define R128_DEBUG_VIS
#endif

#ifdef __MINGW32__
#  define __USE_MINGW_ANSI_STDIO 1 // force standard sprintf precision
#endif

#include "../r128.h"

#include <math.h>
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
         (R128_U32)((R128_U64)(r1) >> 32), (R128_U32)(r1), (R128_U32)((R128_U64)(r0) >> 32), (R128_U32)(r0)); \
      ++testsFailed; \
   }\
} while(0)

#define R128_TEST_EQ4(v, r0, r1, r2, r3) do { \
   ++testsRun; \
   if ((v).lo != (((R128_U64)(r1) << 32) | (r0)) || (v).hi != (((R128_U64)(r3) << 32) | (r2))) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: Got 0x%08x%08x.%08x%08x, expected 0x%08x%08x.%08x%08x\n", \
         __FILE__, __LINE__, R128_R3(&(v)), R128_R2(&(v)), R128_R1(&(v)), R128_R0(&(v)), \
         (R128_U32)(r3), (R128_U32)(r2), (R128_U32)(r1), (R128_U32)(r0)); \
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

#define R128_TEST_INTINTEQ(i1, i2) do { \
   ++testsRun; \
   if ((int)(i1) != (int)(i2)) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: %d != %d\n", \
         __FILE__, __LINE__, (int)(i1), (int)(i2)); \
      ++testsFailed; \
   } \
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
   R128 a, b;
   double d;
   int reta, retb;
   char *endptr;

   d = 0.9999;
   r128FromFloat(&a, d);

   retb = snprintf(bufb, sizeof(bufb), "%5.6f", d);
   reta = r128ToStringf(bufa, sizeof(bufa), "%5.6f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   R128_TEST_INTINTEQ(reta, retb);
   retb = snprintf(bufb, sizeof(bufb), "%1.0f", d);
   reta = r128ToStringf(bufa, sizeof(bufa), "%1.0f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   R128_TEST_INTINTEQ(reta, retb);
   retb = snprintf(bufb, sizeof(bufb), "%# 3.0f", d);
   reta = r128ToStringf(bufa, sizeof(bufa), "%# 3.0f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   R128_TEST_INTINTEQ(reta, retb);
   retb = snprintf(bufb, sizeof(bufb), "%-20.4f", d);
   reta = r128ToStringf(bufa, sizeof(bufa), "%-20.4f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   R128_TEST_INTINTEQ(reta, retb);
   retb = snprintf(bufb, sizeof(bufb), "%#+.50f", d);
   reta = r128ToStringf(bufa, sizeof(bufa), "%#+.50f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   R128_TEST_INTINTEQ(reta, retb);

   d = (1 / 18446744073709551616.0);
   r128FromFloat(&a, d);

   retb = snprintf(bufb, sizeof(bufb), "%1.200f", d);
   reta = r128ToStringf(bufa, sizeof(bufa), "%1.200f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   R128_TEST_INTINTEQ(reta, retb);

   retb = snprintf(bufb, sizeof(bufb), "%1.300f", d);
   reta = r128ToStringf(bufa, sizeof(bufa), "%1.300f", &a);
   R128_TEST_STRSTREQ(bufa, bufb);
   R128_TEST_INTINTEQ(reta, retb);

   r128FromInt(&b, 1);
   r128FromString(&a, "1", &endptr);
   R128_TEST_EQ(a, b);
   R128_TEST_INTINTEQ(*endptr, '\0');
   r128FromString(&a, "+1.", &endptr);
   R128_TEST_INTINTEQ(*endptr, '\0');
   R128_TEST_EQ(a, b);
   r128FromString(&a, "1.0", &endptr);
   R128_TEST_INTINTEQ(*endptr, '\0');
   R128_TEST_EQ(a, b);
   r128FromString(&a, "1.0 xxxxxxx", &endptr);
   R128_TEST_INTINTEQ(*endptr, ' ');
   R128_TEST_EQ(a, b);
}

static void test_sign()
{
    R128 a, b, c;
    r128FromFloat(&a, -4.25);
    r128FromFloat(&b, 4.25);

    R128_TEST_INTINTEQ(r128IsNeg(&a), 1);
    R128_TEST_INTINTEQ(r128IsNeg(&b), 0);

    r128Neg(&c, &a);
    R128_TEST_EQ(c, b);

    r128Neg(&c, &b);
    R128_TEST_EQ(c, a);

    r128Abs(&c, &a);
    R128_TEST_EQ(c, b);

    r128Abs(&c, &b);
    R128_TEST_EQ(c, b);

    r128Nabs(&c, &a);
    R128_TEST_EQ(c, a);

    r128Nabs(&c, &b);
    R128_TEST_EQ(c, a);
}

static void test_cmp()
{
   R128 a = { R128_LIT_U64(0x8000000000000000), 1 };     //1.5
   R128 b = { R128_LIT_U64(0x4000000000000000), 1 };     //1.25
   R128 c = { R128_LIT_U64(0x8000000000000000), R128_LIT_U64(0xffffffffffffffff) };    //-0.5
   R128 d = { R128_LIT_U64(0x4000000000000000), R128_LIT_U64(0xffffffffffffffff) };    //-0.75
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
   R128_TEST_STREQ(c, "4086226761.33142724434097380796");

   R128_SET2(&a, 0, 10);
   R128_SET2(&b, 0, 3);
   r128Div(&c, &a, &b);
   R128_TEST_STREQ(c, "3.333333333333333333333");

   R128_SET2(&a, 0, R128_LIT_U64(2113123919594));
   R128_SET2(&b, 0, R128_LIT_U64(0xffffffffffffd159));
   r128Div(&c, &a, &b);
   R128_TEST_STREQ(c, "-176934096.92656786402076530185");

   R128_SET2(&a, 0, R128_LIT_U64(62727997390472));
   R128_SET2(&b, 0, R128_LIT_U64(154));
   r128Div(&c, &a, &b);
   R128_TEST_STREQ(c, "407324658379.68831168831168831169");

   r128FromInt(&a, 100);
   r128FromString(&b, "10.003048780487804878", NULL);
   r128Div(&c, &a, &b);
   R128_TEST_STREQ(c, "9.99695214873514172513");

   r128Copy(&a, &R128_one);
   r128Shr(&a, &a, 2);
   r128Div(&c, &a, &R128_min);
   R128_TEST_FLEQ(c, 0);

   r128Copy(&b, &R128_one);
   r128Shr(&b, &b, 1);     // b = .5
   r128Shr(&a, &b, 1);     // a = .25
   r128Div(&c, &a, &b);
   R128_TEST_EQ(c, b);

   r128Div(&c, &b, &a);
   R128_TEST_EQ2(c, 0, 2);

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

static void test_sqrt()
{
   R128 a, b;

   r128FromInt(&a, 100);
   r128Sqrt(&b, &a);
   R128_TEST_STREQ(b, "10");

   r128FromInt(&a, 2);
   r128Sqrt(&b, &a);
   R128_TEST_STREQ(b, "1.41421356237309504880");

   r128FromFloat(&a, 0.125);
   r128Sqrt(&b, &a);
   R128_TEST_STREQ(b, "0.35355339059327376220");

   r128FromFloat(&a, 0.999);
   r128Rsqrt(&b, &a);
   R128_TEST_STREQ(b, "1.00050037531277368426");

   r128FromInt(&a, 267794469);
   r128Sqrt(&b, &a);
   R128_TEST_STREQ(b, "16364.42693772073400801913");
}

int main()
{
   test_float();
   test_string();
   test_sign();
   test_cmp();
   test_mod();
   test_div();
   test_shift();
   test_sqrt();

   printf("%d tests run. %d tests passed. %d tests failed.\n",
      testsRun, testsRun - testsFailed, testsFailed);

   return testsFailed;
}
