#include "postgres.h"
#include "fmgr.h"
#include "utils/hashutils.h"
#include <stdint.h>
PG_MODULE_MAGIC;

/* STEPS TO CHECK INPUT VALIDITY
 * First strip off trailing whitespace before first and last character. 
 * The first character should be '{'. 
 * The last character should be '}'
 * Extract the substring between the first and last character '{substring}'
 * Split into comma separated values
 * If no commas were present - check if the one character extracted was a whitespace, meaning the set was empty - { } or check if the set had one integer - this integer needs to be positive and cannot have spaces between its numbers. Ex: { 1 2 3 4 } is invalid. 
 * If more than one value was present, check that the values were positive integers, did not have decimals, does not have spaces in between its digits, was not null/empty or a space.  
 *
 */

/* TODO: Function to check the validity of a set 
 * Input is a string.
 * Output is a boolean. 
 */


/* TODO: Function to check the validity of a number in the set. 
 * Input is a string.
 * Output is a boolean. 
 */ 


/* Representation for a set of integers */
typedef struct {
	int size;
	int elems[1];
}IntSet;

/* Function to create a new set */
IntSet newSet (int size){
	IntSet *set;
	set = malloc(sizeof(IntSet) + sizeof(int)*(size-1));
	if(set) set->size = size;
	return set;
}
/* use set->elems[0] = 1;.. to store the integers in the set */
/* you need to keep the array sorted - will make comparing two sets easier */
/* for the set of sets you can use a hash table/dynamically allocated array */
