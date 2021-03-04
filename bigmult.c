#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h> /* declarations of uintX_t, etc. */
#include <limits.h>

void partialprod32(uint32_t as[], int sz_a, uint32_t bs[], int sz_b, uint32_t d, int shift)
{
    // Assume sz_b + sz_c <= sz_a => sz_b < sz_a
    // (this is because if we have two numbers with n and m digits, then their product would be n + m digits at most)
    // Compute as += bs * d

    int i;
    int i_shifted;
    uint32_t p_split = 0;
    uint32_t c = 0;
    uint64_t s;
    uint64_t p;

    for (i = 0; i < sz_b; i++) 
    {
        i_shifted = i+shift;
        p = (uint64_t) bs[i] * (uint64_t) d; // p is a 64 bit value
        s = (uint64_t) as[i_shifted] + (uint64_t) ((uint32_t) p) + (uint64_t) p_split + (uint64_t) c;
        p_split = p >> 32;
        c = s >> 32;
        as[i_shifted] = (uint32_t) s;
    }

    for ( ; i < sz_a-shift; i++)
    {
        if (p_split == 0 && c == 0)
        {
            break;
        }
        i_shifted = i+shift;
        s = (uint64_t) as[i_shifted] + (uint64_t) p_split + (uint64_t) c;
        p_split = 0;
        c = s >> 32;
        as[i+shift] = (uint32_t) s;
    }
}

void bigmul64(uint64_t a[], int sz_a, uint64_t b[], int sz_b, uint64_t c[], int sz_c)
{   
    uint32_t *as = (uint32_t *) a;
    uint32_t *bs = (uint32_t *) b;
    uint32_t *cs = (uint32_t *) c;

    int i;

    for (i = 0; i < 2*sz_c; i++)
    {
        partialprod32(as, 2*sz_a, bs, 2*sz_b, cs[i], i);
    }
}

int main(void)
{
    int i;

    // let's do some test cases
    // EXAMPLE 1
    int sz_a = 5; // sz_a = sz_b + sz_c
    int sz_b = 2;
    int sz_c = 3;

    uint64_t a[] = {0x0, 0x0, 0x0, 0x0, 0x0};
    uint64_t b[] = {0xffffffffffffffff, 0xffffffffffffffff};
    uint64_t c[] = {0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff};

    bigmul64(a, sz_a, b, sz_b, c, sz_c);

    printf("EXAMPLE 1:\nb = 0x");
    for (i = 0; i < sz_b; i++)
    {
        printf("%016lx", b[sz_b-1-i]);
    }
    printf("\nc = 0x");
    for (i = 0; i < sz_c; i++)
    {
        printf("%016lx", c[sz_c-1-i]);
    }
    printf("\na = b * c = 0x");
    for (i = 0; i < sz_a; i++)
    {
        printf("%016lx", a[sz_a-1-i]);
    }
    printf("\n\n");

    // EXAMPLE 2
    int sz_a_1 = 7; // sz_a > sz_b + sz_b (doesn't necessarily need to be =)
    int sz_b_1 = 3;
    int sz_c_1 = 3;

    uint64_t a_1[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    uint64_t b_1[] = {0x1aaa, 0x00f0a2, 0x4};
    uint64_t c_1[] = {0x1111111, 0x7a111113, 0x375};

    bigmul64(a_1, sz_a_1, b_1, sz_b_1, c_1, sz_c_1);

    printf("EXAMPLE 2:\nb = 0x");
    for (i = 0; i < sz_b_1; i++)
    {
        printf("%016lx", b_1[sz_b_1-1-i]);
    }
    printf("\nc = 0x");
    for (i = 0; i < sz_c_1; i++)
    {
        printf("%016lx", c_1[sz_c_1-1-i]);
    }
    printf("\na = b * c = 0x");
    for (i = 0; i < sz_a_1; i++)
    {
        printf("%016lx", a_1[sz_a_1-1-i]);
    }
    printf("\n");
}