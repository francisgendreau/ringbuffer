#include <stdio.h>
#include <unistd.h>
#include <stddef.h>

#include "ringbuffer.h"

#define TU_MSG(msg) printf("%s: %s\n", __func__, msg)

#define TU_TEST_INIT                \
    unsigned int _tu_test_qty = 0;  \
    unsigned int _tu_test_fail = 0

#define TU_PRINT_SUMMARY            \
do {                                \
    printf("%s: test done:        %d\n", __func__, _tu_test_qty);                   \
    printf("%s: test succeeded:   %d\n", __func__, _tu_test_qty - _tu_test_fail);   \
    printf("%s: test failed:      %d\n", __func__,  _tu_test_fail);                  \
} while(0)

#define TU_TEST_DONE                    \
    TU_PRINT_SUMMARY;                   \
    printf("%s: done\n\n", __func__);   \
    return 0

#define _TU_ASSERT(msg, cond, expr)                 \
do {                                                \
    printf("%s: %s. %s...", __func__, msg, expr);   \
    _tu_test_qty++;                                 \
    if ((cond))                                     \
        puts(" SUCCESS");                           \
    else {                                          \
        puts(" FAILED");                            \
        _tu_test_fail++;                            \
    }                                               \
} while(0)


#define TU_ASSERT_T(msg, cond) _TU_ASSERT(msg, cond, #cond)
#define TU_ASSERT_F(msg, cond)  TU_ASSERT_T(msg, !(cond))
#define TU_ASSERT_EQ(msg, val1, val2) TU_ASSERT_T(msg, val1 == val2)
#define TU_ASSERT_NE(msg, val1, val2) TU_ASSERT_T(msg, val1 != val2)
#define TU_ASSERT_GE(msg, val1, val2) TU_ASSERT_T(msg, val1 >= val2)
#define TU_ASSERT_GT(msg, val1, val2) TU_ASSERT_T(msg, val1 > val2)
#define TU_ASSERT_LE(msg, val1, val2) TU_ASSERT_T(msg, val1 <= val2)
#define TU_ASSERT_LT(msg, val1, val2) TU_ASSERT_T(msg, val1 < val2)

#define _TUF_ASSERT(msg, cond, expr)                \
do {                                                \
    printf("%s: %s. %s...", __func__, msg, expr);   \
    _tu_test_qty++;                                 \
    if ((cond))                                     \
        puts(" SUCCESS");                           \
    else {                                          \
        puts(" FAILED");                            \
        __tu_test_fail++;                           \
      TU_PRINT_SUMMARY;                             \
        exit(-1);                                   \
    }                                               \
} while(0)

#define TUF_ASSERT_F(msg, cond) TUF_ASSERT_T(msg, !(cond))

int test_byte(void)
{
    TU_TEST_INIT;
    struct ringbuffer_t buf;
    char vect[128];

    TU_MSG("initializing a ringbuffer of 128 bytes.");
    ringbuffer_init(&buf, vect, 128, 1);
    TU_ASSERT_T("vector have same address", buf.base == vect);
    TU_ASSERT_T("base and head hold the same address", buf.base == buf.head);
    TU_ASSERT_T("head and tail hold the same address", buf.head == buf.tail);
    TU_ASSERT_T("base and end should be 128bytes apart", buf.end - buf.base == 128);
    TU_ASSERT_T("element_size is 1", buf.element_size == 1);

    TU_MSG("moving the head pointer to half the buffer");
    buf.head += sizeof(unsigned char) * 64;
    TU_ASSERT_EQ("", buf.head - buf.base, 64);
    TU_ASSERT_EQ("", ringbuffer_get_size(&buf), 128);
    TU_ASSERT_EQ("", ringbuffer_get_usage(&buf), 64);
    TU_MSG("moving tail pointer to head pointer.");
    buf.tail = buf.head;
    TU_ASSERT_EQ("", ringbuffer_get_size(&buf), 128);
    TU_ASSERT_EQ("", ringbuffer_get_usage(&buf), 0);
    TU_MSG("moving tail pointer ahead of 1 byte from head pointer.");
    buf.tail++;
    TU_ASSERT_EQ("", ringbuffer_get_usage(&buf), 127);


    TU_TEST_DONE;
}

int test_object(void)
{
    struct testobj_t
    {
        char *c;
        int   i;
        long long ll;
        long double ld;
        char tbl[16];
    } testobj_vect[512];
    struct ringbuffer_t buf;
    TU_TEST_INIT;

    TU_MSG("initiailizing a ringbuffer of 512 struct testobj_t");
    ringbuffer_init(&buf, testobj_vect, 512, sizeof(struct testobj_t));
    TU_ASSERT_T("vector have same address", buf.base == testobj_vect);
    TU_ASSERT_T("base and head hold the same address", buf.base == buf.head);
    TU_ASSERT_T("head and tail hold the same address", buf.head == buf.tail);
    TU_ASSERT_T("base and end should be 512 struct testobj_t apart", buf.end - buf.base == sizeof(testobj_vect));
    TU_ASSERT_T("element_size is sizeof(struct testobj_t)", buf.element_size == sizeof(struct testobj_t));

    TU_TEST_DONE;;
}


int main(int argc, char **argv)
{
    puts("TEST UNIT FOR libringbuffer\n");
    TU_MSG("validating ringbuffer_init() output.");
    test_byte();
    test_object();

}
