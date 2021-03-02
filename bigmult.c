#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h> /* declarations of uintX_t, etc. */
#include <limits.h>

uint32_t addto32(uint32_t as[], int sz_a, uint32_t bs[], int sz_b) 
{
    // Asse sz_b <= sz_a
    // Compute as += bs

    int i;
    uint32_t c = 0;
    uint64_t s;
    for (i = 0; i < sz_b; i++) 
    {
        s = (uint64_t) as[i] + (uint64_t) bs[i] + (uint64_t) c; // s is a 33 bit value
        c = s >> 32;
        as[i] = (uint32_t) s;
    }
    for ( ; i < sz_a; i++)
    {
        s = (uint64_t) as[i] + (uint64_t) c;
        c = s >> 32;
        as[i] = (uint32_t) s;
    }
    return c;
}

void partialprod32(uint32_t as[], int sz_a, uint32_t bs[], int sz_b, uint32_t d, int shift)
{
    // Assume sz_b + sz_c <= sz_a => sz_b < sz_a
    // (this is because if we have two numbers with n and m digits, then their product would be n + m digits at most)
    // Compute as += bs * d

    int i;
    uint32_t c1 = 0;
    uint32_t c2 = 0;
    uint64_t s;
    uint64_t p;

    for (i = 0; i < sz_b; i++) 
    {
        p = (uint64_t) bs[i] * (uint64_t) d; // p is a 64 bit value
        s = (uint64_t) as[i+shift] + (uint64_t) ((uint32_t) p) + (uint64_t) c1 + (uint64_t) c2;
        c1 = p >> 32;
        c2 = s >> 32;
        as[i+shift] = (uint32_t) s;
    }

    for ( ; i < sz_a-shift; i++)
    {
        s = (uint64_t) as[i+shift] + (uint64_t) c1 + (uint64_t) c2;
        c1 = 0;
        c2 = s >> 32;
        as[i+shift] = (uint32_t) s;
    }
}

void bigmul64(uint64_t a[], int sz_a, uint64_t b[], int sz_b, uint64_t c[], int sz_c)
{   
    uint32_t *as = (uint32_t *) a;
    uint32_t *bs = (uint32_t *) b;
    uint32_t *cs = (uint32_t *) c;

    int i;

    for (i = 0; i < 2*sz_b; i++)
    {
        printf("%08x", bs[2*sz_b-1-i]);
    }
    printf("\n\n");

    for (i = 0; i < 2*sz_c; i++)
    {
        printf("%08x", cs[2*sz_c-1-i]);
    }
    printf("\n\n");

    for (i = 0; i < 2*sz_c; i++)
    {
        partialprod32(as, 2*sz_a, bs, 2*sz_b, cs[i], i);
    }
    printf("\n");

    for (i = 0; i < 2*sz_a; i++)
    {
        printf("%08x", as[2*sz_a-1-i]);
    }
    printf("\n");
}

int main(void)
{
    int sz_a = 5;
    int sz_b = 2;
    int sz_c = 3;

    uint64_t a[] = {0x0, 0x0, 0x0, 0x0, 0x0};
    uint64_t b[] = {0xffffffffffffffff, 0xffffffffffffffff};
    uint64_t c[] = {0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff};

    bigmul64(a, sz_a, b, sz_b, c, sz_c);
}