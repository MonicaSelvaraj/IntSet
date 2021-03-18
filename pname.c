#include "postgres.h"
#include "fmgr.h"
#include "utils/hashutils.h"

#include <stdint.h>

/* so that Postgres can detect which version this module is designed for */
PG_MODULE_MAGIC;

/* character range macros */
#define IS_UPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define IS_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define IS_LETTER(c) (IS_UPPER(c) || IS_LOWER(c) || ((c) == '-') || ((c) == '\''))

/* grammar check functions */
bool parse_input(char* input, char** family, uint32_t* familyLen, char** given, uint32_t* givenLen);
bool check_namelist(char* input, char** terminator);
bool check_name(char* input, char** restart);

typedef struct PersonName
{
    /* Don't set directly! Use SET_VARSIZE! */
    int32_t length;
    /* offset of the delimiter (synonymous to the "family" length) */
    uint32_t delim;
    /* flexible array */
    uint8_t data[];
} PersonName;

#define HEADERSZ (VARHDRSZ + sizeof(PersonName))

/* easy access to cstring family and given */
/* these "LEN" functions INCLUDES terminating NULL */
#define FAMILY(p) ((char*)p->data)
#define FAMILYLEN(p) (input->delim + 1)
#define GIVEN(p) ((char*)(p->data + p->delim + 1))
#define GIVENLEN(p) ((VARSIZE(input) - HEADERSZ) - input->delim - 1)

/* operator */
int pname_cmp(PersonName* a, PersonName* b);
bool pname_op_eq(PersonName* a, PersonName* b);

/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_in);

Datum
pname_in(PG_FUNCTION_ARGS)
{
    const char* input = PG_GETARG_CSTRING(0);
    char *family, *given;
    uint32_t familyLen, givenLen;
    PersonName* ret;

    /* don't include NULL */
    int32_t inputSize = strlen(input);
    /* size of output data structure that will be passed on to SET_VARSIZE */
    uint32_t retSz;

    /* copy to our own memory for easier manipulation. */
    /* also, we're forbidden from changing byref input pointers. */
    /* take into account the terminating NULL. */
    char *data = palloc(inputSize + 1);
    memcpy(data, input, inputSize);
    data[inputSize] = 0;

    if(!parse_input(data, &family, &familyLen, &given, &givenLen)) {
        pfree(data);
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type PersonName: \"%s\"", input)));
    }

    /* include both terminating NULL in the data structure. */
    retSz = HEADERSZ + familyLen + givenLen + 2;
    ret = palloc(retSz);
    SET_VARSIZE(ret, retSz);

    /* offset of the terminating NULL of family (also length of family) */
    ret->delim = familyLen;
    
    // copy family
    memcpy(FAMILY(ret), family, familyLen + 1);
    // copy given
    memcpy(GIVEN(ret), given, givenLen + 1);
    pfree(data);
    PG_RETURN_POINTER(ret);
}

PG_FUNCTION_INFO_V1(pname_out);

Datum
pname_out(PG_FUNCTION_ARGS)
{
	PersonName* input = (PersonName*) PG_GETARG_POINTER(0);
    /*Canonical format: family,given*/
    uint32_t len = VARSIZE(input) - HEADERSZ;
    // "len" already includes 2 NULL. We'll use one of them for ','
    char * result = palloc(len);
    // family. copy w/o the terminating NULL
    memcpy(result, FAMILY(input), FAMILYLEN(input) - 1);
    // formatting
    result[input->delim] = ',';
    // given. copy up to the terminating NULL
    memcpy(result + input->delim + 1, GIVEN(input), GIVENLEN(input));
	PG_RETURN_CSTRING(result);
}

/*****************************************************************************
 * Misc. Functions as required for assignment
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_family);
Datum
pname_family(PG_FUNCTION_ARGS)
{
    PersonName* input = (PersonName*) PG_GETARG_POINTER(0);
    PG_RETURN_CSTRING(FAMILY(input));
}

PG_FUNCTION_INFO_V1(pname_given);
Datum
pname_given(PG_FUNCTION_ARGS)
{
    PersonName* input = (PersonName*) PG_GETARG_POINTER(0);
    PG_RETURN_CSTRING(GIVEN(input));
}

PG_FUNCTION_INFO_V1(pname_show);
Datum
pname_show(PG_FUNCTION_ARGS)
{
    PersonName* input = (PersonName*) PG_GETARG_POINTER(0);

    // scan given for space or null
    char* given = GIVEN(input);
    char* givenEnd = strchrnul(given, ' ');
    size_t givenLen = givenEnd - given;

    // allocate cstring for return (one extra space and terminating NULL)
    char * result = palloc(givenLen + input->delim + 2);

    // given
    memcpy(result, given, givenLen);
    // space after given
    result[givenLen] = ' ';

    // family
    memcpy(result + givenLen + 1, FAMILY(input), input->delim + 1);

	PG_RETURN_CSTRING(result);
}

/*****************************************************************************
 * Misc. Functions as required for assignment, text version
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_family_text);
Datum
pname_family_text(PG_FUNCTION_ARGS)
{
    PersonName* input = (PersonName*) PG_GETARG_POINTER(0);
    // no need to include terminating NULL here
    const uint32_t retlen = VARHDRSZ + FAMILYLEN(input) - 1;
    text* ret = palloc(retlen);
    SET_VARSIZE(ret, retlen);
    memcpy(ret->vl_dat, FAMILY(input), FAMILYLEN(input) - 1);
    PG_RETURN_TEXT_P(ret);
}

PG_FUNCTION_INFO_V1(pname_given_text);
Datum
pname_given_text(PG_FUNCTION_ARGS)
{
    PersonName* input = (PersonName*) PG_GETARG_POINTER(0);
    // no need to include terminating NULL here
    const uint32_t retlen = VARHDRSZ + GIVENLEN(input) - 1;
    text* ret = palloc(retlen);
    SET_VARSIZE(ret, retlen);
    memcpy(ret->vl_dat, GIVEN(input), GIVENLEN(input) - 1);
    PG_RETURN_TEXT_P(ret);
}

PG_FUNCTION_INFO_V1(pname_show_text);
Datum
pname_show_text(PG_FUNCTION_ARGS)
{
    PersonName* input = (PersonName*) PG_GETARG_POINTER(0);

    // scan given for space or null
    char* given = GIVEN(input);
    char* givenEnd = strchrnul(given, ' ');
    size_t givenLen = givenEnd - given; // no terminator

    // allocate text for return (one extra space, no term. NULL)
    uint32_t resLen = VARHDRSZ + givenLen + FAMILYLEN(input);
    text* result = palloc(resLen);
    SET_VARSIZE(result, resLen);

    // given
    memcpy(result->vl_dat, given, givenLen);
    // space after given
    result->vl_dat[givenLen] = ' ';

    // family. remember, no terminating NULL!
    memcpy(result->vl_dat + givenLen + 1, FAMILY(input), input->delim);

	PG_RETURN_TEXT_P(result);
}

/*****************************************************************************
 * Operators
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_eq);
Datum
pname_eq(PG_FUNCTION_ARGS)
{
    PersonName* a = (PersonName*) PG_GETARG_POINTER(0);
    PersonName* b = (PersonName*) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(pname_op_eq(a,b));
}

PG_FUNCTION_INFO_V1(pname_gt);
Datum
pname_gt(PG_FUNCTION_ARGS)
{
    PersonName* a = (PersonName*) PG_GETARG_POINTER(0);
    PersonName* b = (PersonName*) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(pname_cmp(a,b) > 0);
}

PG_FUNCTION_INFO_V1(pname_ge);
Datum
pname_ge(PG_FUNCTION_ARGS)
{
    PersonName* a = (PersonName*) PG_GETARG_POINTER(0);
    PersonName* b = (PersonName*) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(pname_cmp(a,b) >= 0);
}

PG_FUNCTION_INFO_V1(pname_ne);
Datum
pname_ne(PG_FUNCTION_ARGS)
{
    PersonName* a = (PersonName*) PG_GETARG_POINTER(0);
    PersonName* b = (PersonName*) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(!pname_op_eq(a,b));
}

PG_FUNCTION_INFO_V1(pname_lt);
Datum
pname_lt(PG_FUNCTION_ARGS)
{
    PersonName* a = (PersonName*) PG_GETARG_POINTER(0);
    PersonName* b = (PersonName*) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(pname_cmp(a,b) < 0);
}

PG_FUNCTION_INFO_V1(pname_le);
Datum
pname_le(PG_FUNCTION_ARGS)
{
    PersonName* a = (PersonName*) PG_GETARG_POINTER(0);
    PersonName* b = (PersonName*) PG_GETARG_POINTER(1);
    PG_RETURN_BOOL(pname_cmp(a,b) <= 0);
}

/*****************************************************************************
 * BTree/Hash Support Functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_sint_cmp);
Datum
pname_sint_cmp(PG_FUNCTION_ARGS)
{
    PersonName* a = (PersonName*) PG_GETARG_POINTER(0);
    PersonName* b = (PersonName*) PG_GETARG_POINTER(1);
    PG_RETURN_INT32(pname_cmp(a,b));
}

PG_FUNCTION_INFO_V1(pname_hash);
Datum
pname_hash(PG_FUNCTION_ARGS)
{
    PersonName* p = (PersonName*) PG_GETARG_POINTER(0);
    /* Just hash the entire length of the 'data' part of the data structure. 
       A logically equivalent part will have the same 'data' part due to the bytewise comparison.
    */
    return hash_any(p->data, VARSIZE(p) - HEADERSZ);
}

/*****************************************************************************
 * Grammar Check Functions (Not registered under v1)
 *****************************************************************************/

bool parse_input(char* input, char** family, uint32_t* familyLen, char** given, uint32_t* givenLen) {
    char* delim;
    char* givenEnd;

    delim = strchr(input, ',');
    /* no delimiter? not allowed! */
    if(!delim)
        return false;
    
    /* the position of the delim will be the terminator of "family" */
    *delim = 0;
    *familyLen = delim - input;
    *family = input;

    /* delimiter may be optionally followed by one space */
    if(*(delim + 1) == ' ')
        ++delim;
    
    /* the beginning of "given" */
    *given = delim + 1;

    /* now check for "namelist" correctness for both family and given. */
    /* also, find the end of the given if it is correct (to calc. the length) */
    if(check_namelist(*family, NULL) && check_namelist(*given, &givenEnd)) {
        *givenLen = givenEnd - *given;
        return true;
    } else
        return false;
}

bool check_namelist(char* input, char** terminator) {
    /* used to resume our iteration */
    char* restart_name = input;
    do {
        if(!check_name(restart_name, &restart_name))
            return false;
        /* If check_name returns true, *restart_name will be either ' ' or NULL */
        /* ' ' means we will continue scanning. */
        if(!*restart_name) {
            if(terminator)
                *terminator = restart_name;
            break;
        }
        ++restart_name;
    } while(1);
    return true;
}

bool check_name(char* input, char** restart) {
    /* first char must upper */
    if(!IS_UPPER(*input))
        return false;
    ++input;
    /* second char must be letter */
    if(!IS_LETTER(*input))
        return false;
    ++input;
    
    /* Ensure everything is a letter.
       Stop 'til we reach the end of the string or a space. */
    while(*input) {
        if(*input == ' ')
            break;
        if(!IS_LETTER(*input))
            return false;
        ++input;
    }

    /* OKAY!! */
    *restart = input;
    return true;
}

/*****************************************************************************
 * Operator stub functions (Not registered under v1)
 *****************************************************************************/
bool pname_op_eq(PersonName* a, PersonName* b) {
    // should be faster than the standard approach as we don't branch here
    return strcmp(FAMILY(a), FAMILY(b)) == 0 && // family
        strcmp(GIVEN(a), GIVEN(b)) == 0; // given
}

int pname_cmp(PersonName* a, PersonName* b) {
    int famCompare = strcmp(FAMILY(a), FAMILY(b));
    if(famCompare == 0)
        return strcmp(GIVEN(a), GIVEN(b));
    else
        return famCompare;
}
