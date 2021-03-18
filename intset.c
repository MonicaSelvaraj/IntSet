/*
 * src/tutorial/complex.c
 *
 ******************************************************************************
  This file contains routines that can be bound to a Postgres backend and
  called by the backend in the process of processing queries.  The calling
  format for these routines is dictated by Postgres architecture.
******************************************************************************/

#include "postgres.h"

#include "fmgr.h"
#include "libpq/pqformat.h"		/* needed for send/recv functions */
#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdbool.h>

PG_MODULE_MAGIC;

typedef struct Set
{
    int data[];

}			Set;


/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/


Datum   intset_in(PG_FUNCTION_ARGS);
Datum	intset_out(PG_FUNCTION_ARGS);

Datum   intset_contains(PG_FUNCTION_ARGS)
Datum   intset_card(PG_FUNCTION_ARGS)
Datum   intset_superset(PG_FUNCTION_ARGS)
Datum   intset_subset(PG_FUNCTION_ARGS)
Datum   intset_eq(PG_FUNCTION_ARGS)
Datum   intset_neq(PG_FUNCTION_ARGS)
Datum   intset_union(PG_FUNCTION_ARGS)
Datum   intset_inter(PG_FUNCTION_ARGS)
Datum   intset_disjunction(PG_FUNCTION_ARGS)
Datum   intset_difference(PG_FUNCTION_ARGS)


PG_FUNCTION_INFO_V1(intset_in);
PG_FUNCTION_INFO_V1(intset_out);

PG_FUNCTION_INFO_V1(intset_contains);
PG_FUNCTION_INFO_V1(intset_card);
PG_FUNCTION_INFO_V1(intset_superset);
PG_FUNCTION_INFO_V1(intset_subset);
PG_FUNCTION_INFO_V1(intset_eq);
PG_FUNCTION_INFO_V1(intset_neq);
PG_FUNCTION_INFO_V1(intset_union);
PG_FUNCTION_INFO_V1(intset_inter);

PG_FUNCTION_INFO_V1(intset_disjunction);
PG_FUNCTION_INFO_V1(intset_difference);


Datum
intset_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
//	int		x,
//			y;
	Set    *result;
	
	char *tempstr = palloc(strlen(str));
	strcpy(tempstr, str);
//
//	if (sscanf(str, " ( %d , %d )", &x, &y) != 2)
//		ereport(ERROR,
//				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
//				 errmsg("invalid input syntax for type %s: \"%s\"",
//						"complex", str)));
						
						
//	if (sscanf(str, " ( %d , %d )", &x, &y) != 2)
//		ereport(ERROR,
//				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
//				 errmsg("invalid input syntax for type intset %s: \"%s\"",
//						"complex", str)));
//
//	result = (Integer *) palloc(sizeof(Integer));
//	result->x = x;
//	result->y = y;
//	PG_RETURN_POINTER(result);
}


Datum
intset_out(PG_FUNCTION_ARGS)
{
//	Integer    *integer = (Integer *) PG_GETARG_POINTER(0);
//	char	   *result;
//
//	result = psprintf("(%d,%d)", integer->x, integer->y);
//	PG_RETURN_CSTRING(result);
}

/*****************************************************************************
 * Binary Input/Output functions
 *
 * These are optional.
 *****************************************************************************/



/*****************************************************************************
 * Operators
 
 *****************************************************************************/

/*PG_FUNCTION_INFO_V1(integer_add);

Datum
integer_add(PG_FUNCTION_ARGS)
{
	Integer    *a = (Integer *) PG_GETARG_POINTER(0);
	Integer    *b = (Integer *) PG_GETARG_POINTER(1);
	Integer    *result;

	result = (Integer *) palloc(sizeof(Integer));
	result->x = a->x + b->x;
	result->y = a->y + b->y;
	PG_RETURN_POINTER(result);
}*/

Datum
intset_contains(PG_FUNCTION_ARGS){
		Set    *set1 = (Set *) PG_GETARG_POINTER(0);
		Set    *set2 = (Set *) PG_GETARG_POINTER(1);
		PG_RETURN_BOOL(set_contains(set1, set2) ? 0);
}

/*****************************************************************************
 * Operator class for defining B-tree index
 *

 *****************************************************************************/





