#include "postgres.h"
#include "fmgr.h"
#include "utils/hashutils.h"
#include <stdlib.h>
#include <stdint.h>

PG_MODULE_MAGIC;

/* functions that check the validity of int sets */



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

/* This function removes leading and trailing whitespace.
 * Returns a new, trimmed string.
 * Returns original string if it is just a whitespace character. 
 * Note: This is adapted from stack overflow. 
 */
char* removeWhitespace(char* input){
	
	//Pointer to last character of input string 
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

/* This function removes preceeding 0's from a string of numbers, if present 
 * Input is a string.
 * Output is a string with preceeding 0's removed
 */
char* removeZeros(char* input){
	//Pointer to the last character of the input string 
	char* last;
	last = input + strlen(input) - 1;

	while(input <= last){
                if((*input) == '0'){
                        return false;
                trimmedNum++;



/* Function to check the validity of a number in the set.
 * The function is used only for non-empty sets  
 * Input is a string.
 * Returns an int: -1 for failure, the valid int for success. 
 */
int isValidNum(char* num){

	//Whitespace should not matter - so strip leading and trailing whitespace
	char* trimmedNum = removeWhitespace(num);
	
	//Converting char* to int to check if final number is a positive integer within range
	int full_num = atoi(trimmedNum);

	//Checking for empty string - empty string is not valid when multiple comma separated integers are present in the set
	if(*trimmedNum == 0) return -1;

	//Checking that there are no whitespaces, decimal points or special characters in the trimmedNum - checking every character
        char* trimmedLast;
        trimmedLast = trimmedNum + strlen(trimmedNum) - 1;
        while(trimmedNum <= trimmedLast){
                if(!isdigit((unsigned char)*trimmedNum)){
                        return -1;
                trimmedNum++;	
	
	//Check if whole number is between 0 and INT_MAX
	if(!(full_num >= 0 && full_num <= INT_MAX)) return -1;

	return full_num;
}

/* Function to check the validity of a set
 * Input is a string.
 * Output is a boolean.
 */

bool isValidSet(char* input){
        //Remove leadng and trailing spaces
        char* trimmedInput = removeWhitespace(input);

        //Check if the first character is a '{'
        char* first;
        first = trimmedInput;
        if(*first != '{') return false;

        //Check if the last character is a '}'
        char* last;
        last = first + strlen(first) - 1;
        if(*last != '}') return false;

        //Get substring between '{' and '}'
        first = first + 1; last = last - 1;

        //Split into comma separated values and store in an array
        int size = 0;

        //Case 1 - only one element in the set
        //If there is only one element in the set, check if the set is empty
        //If non-empty set, remove preceeding 0's if present and check isValidNum

        //Case 2 - more than one element in the set
        //Remove trailing 0's, check if valid num
        //If more than one comma separated value is present, none of those values can be whitespace - isValidNum is set up to reflect this.

        //Remove duplicates and sort array
        //Make newIntSet


}

/*************************** END OF INPUT PARSING ***************************/
