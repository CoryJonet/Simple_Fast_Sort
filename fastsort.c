///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Title:            fastsort.c
// Semester:         CS 537 Spring 2016
//
// Author:           Cory Jonet
// Email:            jonet@wisc.edu
// CS Login:         jonet
// Lecturer's Name:  Remzi
//
//////////////////////////// 80 columns wide //////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH   128

typedef struct {

  unsigned int length;
  char *line;

} line_t;

int compare_words_structs(const void *value_1, const void *value_2) {
  
  const line_t *line_struct_1 = (const line_t *) value_1;
  const line_t *line_struct_2 = (const line_t *) value_2;

  return strcmp(line_struct_1->line, line_struct_2->line);
}

int cmd_ret_usage() {

  fprintf(stderr, "Error: Bad command line parameters\n");
  exit(1);

}

int main (int argc, char *argv[]) {

  char *input_file = NULL; /* Input file pointer */
  char line[MAX_LINE_LENGTH + 2]; /* Each line from input file */
  char *dash = NULL; /* Parse "-" in command line */
  FILE *input_file_pointer; /* File pointer to input file */

  int line_iterator = 0; /* Iterator over lines */
  int num_lines = 0; /* Count number of lines in file */
  int this_char = 0; /* Char iterating over */
  int user_word_choice = 0; /* Word index */
  int k = 0; /* Pointer position */

  struct stat file_stat; /* File size */

  /* Check validity of input arguments */
  if ((argc < 2) || (argc > 3))
    cmd_ret_usage();

  /* Is word index a number? */
  if ((argc == 3) && !isdigit(argv[1][1]))
    cmd_ret_usage();

  /* Retrieve user word choice */
  if (argc == 3) {
   
    dash = argv[1];
    
    if (strtok(dash, "-") == NULL)
      cmd_ret_usage();
    
    else {
      
      dash = argv[1];
      if (*dash != '-')
	cmd_ret_usage();

      dash++;
      user_word_choice = atoi(dash);

      if (user_word_choice == 0)
	cmd_ret_usage();

    }
  }

  /* Retrieve file name */
  if (argc == 2)
    input_file = argv[1];

  else if (argc == 3)
    input_file = argv[2];

  /* Open up file in input_file */
  input_file_pointer = fopen(input_file, "r");
  
  /* Can't open up input_file */
  if (input_file_pointer == NULL) {

    fprintf(stderr, "Error: Cannot open file %s\n", input_file);
    exit(1);

  }

  /* Have a file, get size of file for malloc-ing */
  if (stat(input_file, &file_stat) != 0)
    exit(1);

  /* Need to know number of lines in file */
  while (!feof(input_file_pointer)) {
      
    this_char = fgetc(input_file_pointer);
    
    if (this_char == '\n')
      num_lines++; 
    
  }

  rewind(input_file_pointer);
  
  /* Struct pointer to store all lines */
  line_t text[num_lines];

  /* Put each line (line by line) into array of char pointers */
  while (fgets(line, sizeof(line), input_file_pointer) != NULL) {

    if (strlen(line) > MAX_LINE_LENGTH) {
      
      fprintf(stderr, "Line too long\n");
      exit(1);

    }
    
    text[line_iterator].line = malloc(strlen(line) + 1);
    text[line_iterator].length = strlen(line);

    if (text[line_iterator].line == NULL) {
      
      fprintf(stderr, "malloc failed\n");
      exit(1);

    }

    strcpy(text[line_iterator].line, line);

    //    printf("text[line_iterator].line: %s\n", text[line_iterator].line);
    line_iterator++;
  }

  /* We have a user word choice */
  if (user_word_choice > 0) {
    
    line_iterator = 0;
    char *prev_char = NULL; /* Previous char */
    int word_num = 0; /* Word number iterating at */
        
    /* Loop through number of lines */
    while (line_iterator < num_lines) {
          
      /* Record string length at lines index */
      text[line_iterator].length = strlen(text[line_iterator].line);
      
      /* While there is a word index left */
      while (*text[line_iterator].line != '\n') {
	
	if (*text[line_iterator].line != ' ') {
	  
	  if (prev_char == NULL || *prev_char == ' ')
	    word_num++;
	  
	  if (word_num == user_word_choice)
	    goto here;
	  
	  text[line_iterator].line++;
	  prev_char = text[line_iterator].line - 1;
	}
	
	else if (*text[line_iterator].line == ' ') {
	
	  text[line_iterator].line++;
	  prev_char = text[line_iterator].line - 1;
	
	}

      }
      
    here:

      /* Is there no word index here? */
      if (*text[line_iterator].line == '\n') { 
       
	while (*text[line_iterator].line != ' '){

	  text[line_iterator].line--;

	}
	
	/* Get rid of ' ' */
	text[line_iterator].line++;

      }
      
      word_num = 0;
      prev_char = NULL;
      line_iterator++;
    }
  }
  
  qsort(text, num_lines, sizeof(const line_t), compare_words_structs);
  
  line_iterator = 0;
 
  /* Loop through number of lines */
  while(line_iterator < num_lines) {
      
    /* "Backup" pointer location */
    while (k < (text[line_iterator].length - 
		strlen(text[line_iterator].line)))
      text[line_iterator].line--;
      
    printf("%s", text[line_iterator].line);
    
    line_iterator++;
  }

  line_iterator = 0;

  // I could not for the life of me free this array of structs 
  // with pointers that I allocated...
  /*
  while(line_iterator < num_lines)
    free((void *)text[line_iterator].line);
  */

  fclose(input_file_pointer);
  exit(0);
}
