/*
#include "postgres.h"
#include "fmgr.h"
#include "utils/hashutils.h"
#include <stdint.h>
PG_MODULE_MAGIC;
*/

/*************************** INTSET TYPE CREATION *****************************/

/* Representation for a set of integers */
typedef struct {
        int size;
        int integers[];
}IntSet;

/* Function to create a new IntSet */
IntSet newIntSet (int size){
        IntSet *newSet;
        newSet = malloc(sizeof(IntSet) + sizeof(int)*(size-1));
        if(newSet) newSet->size = size;
	newSet->integers[size] = 0;
        return newSet;
}

/*****************************************************************************/

/****************************** PARSING INPUT ********************************/

/* Defining a macro to check for valid positive integers */
#define IS_POSITIVE_INTEGER(i) ((i >= 0 ) && (i <= INT_MAX))

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

bool isValidSet(){
	//Remove leadng and trailing spaces 
	
	//Check if the first character is a '{'

	//Check if the last character is a '}'

	//Get substring between '{' and '}'
	
	//Split into comma separated values and store in an array

	//Case 1 - only one element in the set 
	//If there is only one element in the set, check if the set is empty 
	//If non-empty set, remove preceeding 0's if present and check isValidNum 
	
	//Case 2 - more than one element in the set
	//Remove trailing 0's, check if valid num
	//If more than one comma separated value is present, none of those values can be whitespace - isValidNum is set up to reflect this. 
	
	//Remove duplicates and sort array
	//Make newIntSet

	
}


/* Function to check the validity of a number in the set.
 * The function is used only for non-empty sets  
 * Input is a string.
 * Output is a boolean. 
 */
bool isValidNum(char* num){

	//Whitespace should not matter - so strip leading and trailing whitespace
	char* trimmedNum = removeWhitespace(num);

	//Checking for empty string - empty string is not valid when multiple comma separated integers are present in the set
	if(*trimmedNum == 0) return false;

	//Checking that there are no whitespaces, decimal points or special characters in the trimmedNum
	char* trimmedLast;
	trimmedLast = trimmedNum + strlen(trimmedNum) - 1;
	while(trimmedNum <= trimmedLast){
		if(!(IS_POSITIVE_INTEGER(atoi(*trimmedNum)))){
			return false;
		trimmedNum++;
	
	return true;
}

/* This function removes leading and trailing whitespace.
 * Returns a new, trimmed string.
 * Returns original string if it is just a whitespace character. 
 * Note: This is adapted from stack overflow. 
 */
char* removeWhitespace(char* input){
	
	//Pointer for last character of input string 
	char* last;
	last = input + strlen(input) - 1; 
  
	//Return original string if all whitespaces
  	if(*input == 0)
    		return input;
  
	// Moving pointer to first non-whitespace character, removes leading whitespace characters of input string
  	while(isspace((unsigned char)*input)) 
	  	input++;

  	// Moving last pointer to last non-whitespace character of input string
	while(last > input && isspace((unsigned char)*last)) last--;

  	// Write new null terminator character
  	last[1] = '\0';

  return input;
}

/*************************** END OF INPUT PARSING ***************************/
