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
IntSet newIntSet (int size, int* elements){
        IntSet *newSet;
        newSet = malloc(sizeof(IntSet) + sizeof(int)*(size-1));
        if(newSet) newSet->size = size;
	newSet->integers = elements;
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

	while(input < last){
                if((unsigned char)*input == '0'){
                        input++;
        return input;
}

/* Function to check the validity of a number in the set.
 * The function is used only for non-empty sets  
 * Input is a string.
 * Returns an int: -1 for failure, the valid int for success. 
 */
int isValidNum(char* num){

	//Whitespace should not matter - so strip leading and trailing whitespace
	char* trimmedNum = removeWhitespace(num);
	
	//Converting char* to int to check if final number is a positive integer within range
	//after removing preceeding 0's, if present 
	char* zerosRemNum = removeZeros(trimmedNum);
	int full_num = atoi(zerosRemNum);

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
 * Output is an IntSet.
 */

IntSet isValidSet(char* input){

	//Array to store set elements
	int* elements;

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

        //Split into comma separated values and store in an int array
        int size = 0; //size of int array, i.e. number of elements in the int set
	char delimiter[] = ",";
	char* p = strtok(first, delimiter);
	while(p != NULL){
		int valid_num;
		//Case 1 - only one element in the set 
		if(strcmp(p,first) == 0){
			//Check if the set is an empty-set
			if(*p == 0){
				elements[0] = "";
				return newIntSet (size, elements);
			}
			//If the set is non-empty, check if element isValidNum
			valid_num = isValidNum(p);
			else if(valid_num != -1){
				elements[0] = valid_num;
				size++; //int set has one element
				return newIntSet (size, elements);
			} 
			else return NULL;
		}		
		//Case 2 - more than one element in the set
		valid_num = isValidNum(p);
		if(valid_num != -1){
			elements[size] = valid_num;
			size++;
			p = strtok(NULL, delim);
		else return NULL;		
	}
	//TODO: Remove duplicates and sort elements array
	return newIntSet(size, elements);
}

/*************************** END OF INPUT PARSING ***************************/
