/*
** Header:	@(#) symtab.c 1.8 90/12/18 16:30:44 baker penguin UCB/LBL
**
** Name:	symtab.c
**
*/

#include "symtab.h"



int
symCreate (char *prefix, char *filename)
{
    char		instring[BUFSIZ];
    char		*string;

    int			item_count;
    int			len, file_len;

    FILE		*infile;


    /*
    ** Open the input file.
    */
    if ((infile = fopen(filename, "r")) == NULL)
    	return(-1);

    /*
    ** Get the number of characters in the file.
    */
    for (file_len  = 0; fgets(instring, BUFSIZ-1, infile) != NULL;
	 file_len += strlen(instring) + 1);
    file_len++;

    /*
    ** Allocate temporary string storage.
    */
    if ((string = (char *) malloc(file_len)) == NULL)
	return(-1);

    /*
    ** Read file into string.
    */
    rewind(infile);
    for (len  = 0; fgets(string+len, BUFSIZ-1, infile) != NULL;
	 len += strlen(string+len) + 1)

    /*
    ** Append null string to end.
    */
    len++;
    strcat(string+len, "");

    /*
    ** Create symbol table from string.
    */
    item_count = symCreateString(prefix, string);

    /*
    ** Cleanup.
    */
    fclose(infile);
    free(string);

    return(item_count);
}


int
symAdd (char *prefix, char *filename)
{
    char		instring[BUFSIZ];
    char		*string;

    int			item_count;
    int			len, file_len;

    FILE		*infile;

    /*
    ** Open the input file.
    */
    if ((infile = fopen(filename, "r")) == NULL)
    	return(-1);

    /*
    ** Get the number of characters in the file.
    */
    for (file_len  = 0; fgets(instring, BUFSIZ-1, infile) != NULL;
	 file_len += strlen(instring) + 1);
    file_len++;

    /*
    ** Allocate temporary string storage.
    */
    if ((string = (char *) malloc(file_len)) == NULL)
	return(-1);

    /*
    ** Read file into string.
    */
    rewind(infile);
    for (len  = 0; fgets(string+len, BUFSIZ-1, infile) != NULL;
	 len += strlen(string+len) + 1)

    /*
    ** Append null string to end.
    */
    len++;
    strcat(string+len, "");

    /*
    ** Add string to symbol table.
    */
    item_count = symAddString(prefix, string);

    /*
    ** Cleanup.
    */
    fclose(infile);
    free(string);

    return(item_count);
}


int
symCreateString (char *prefix, char *instring)
{
    char		*key_ptr, *value_ptr, *string;

    int			item_count, number_of_entries, number_of_bytes;
    int			sym_len, in_len, str_len, pre_len, len;

    ENTRY		item;


    /*
    ** Count the number of charaters in the input string.
    */
    pre_len = strlen(prefix);
    in_len  = 0;
    sym_len = 0;
    while (strcmp(instring+in_len, "") != 0)
    {
	len      = strlen(instring+in_len) + 1;
	in_len  += len;
	sym_len += len + pre_len;
    }

    /*
    ** Create a hash table.
    */
    number_of_bytes   = sym_len + 1;
    number_of_entries = number_of_bytes / 10;
    if (hcreate(number_of_entries) == 0)
    {
	errno = ENOBUFS;
	return(-1);
    }

    /*
    ** Allocate string tables.
    */
    if ((symString     = (char *) malloc(number_of_bytes)) == NULL)
	return(-1);
    if ((symKeyTable   = (char *) malloc(number_of_bytes)) == NULL)
	return(-1);
    if ((symValueTable = (char *) malloc(number_of_bytes)) == NULL)
	return(-1);

    /*
    ** Copy input string to symbol string.
    */
    pre_len = strlen(prefix);
    in_len  = 0;
    sym_len = 0;
    while (strcmp(instring+in_len, "") != 0)
    {
	strcpy(symString+sym_len, prefix);
	sym_len += pre_len;

	strcat(symString+sym_len, instring+in_len);
	len      = strlen(instring+in_len) + 1;
 	in_len  += len;
	sym_len += len;
    }

    /*
    ** Append null string to end.
    */
    (void) strcat(symString+number_of_bytes, "");

    /*
    ** Place entries from symbol string into hash table.
    */
    key_ptr    = symKeyTable;
    value_ptr  = symValueTable;
    item_count = 0;
    for (sym_len  = 0; strcmp(string = symString+sym_len, "") != 0;
	 sym_len += strlen(string) + 1)
    {
	(void) sscanf(string, "%[^: \t]", key_ptr);
        (void) strcpy(value_ptr, string + strlen(key_ptr) + 1);
        (void) sscanf(key_ptr, "%s", key_ptr);
	item.key   = key_ptr;
	item.data  = value_ptr;
	key_ptr   += strlen(key_ptr) + 1;
	value_ptr += strlen(value_ptr) + 1;
        item_count++;

	if (hsearch(item, ENTER) == NULL)
        {
	    errno = ENFILE;
	    return(-1);
	}
    }

    return(item_count);
}


int
symAddString (char *prefix, char *instring)
{
    char		*key_ptr, *value_ptr, *string;
    char		*old_symString, *old_symKeyTable, *old_symValueTable;

    int			item_count, number_of_entries, number_of_bytes;
    int			sym_len, in_len, str_len, pre_len, len;

    ENTRY		item;


    /*
    ** Check if symCreateString has been called.
    */
    if (symString == NULL)
    {
	errno = ENXIO;
	return(-1);
    }

    /*
    ** Copy old pointers.
    */
    old_symString     = symString;
    old_symKeyTable   = symKeyTable;
    old_symValueTable = symValueTable;

    /*
    ** Destroy old symbol table.
    */
    hdestroy();

    /*
    ** Count the number of charaters in the input string.
    */
    pre_len = strlen(prefix);
    in_len  = 0;
    sym_len = 0;
    while (strcmp(instring+in_len, "") != 0)
    {
	len      = strlen(instring+in_len) + 1;
	in_len  += len;
	sym_len += len + pre_len;
    }

    /*
    ** Count the number of charaters in the symbol string.
    */
    for (str_len  = 0; strcmp(symString+str_len, "") != 0;
	 str_len += strlen(symString+str_len) + 1);

    /*
    ** Create a hash table.
    */
    number_of_bytes   = sym_len + str_len + 1;
    number_of_entries = number_of_bytes / 10;
    if (hcreate(number_of_entries) == 0)
    {
	errno = ENOBUFS;
	return(-1);
    }

    /*
    ** Allocate string tables.
    */
    if ((symString     = (char *) malloc(number_of_bytes)) == NULL)
	return(-1);
    if ((symKeyTable   = (char *) malloc(number_of_bytes)) == NULL)
	return(-1);
    if ((symValueTable = (char *) malloc(number_of_bytes)) == NULL)
	return(-1);

    /*
    ** Copy old symbol string to new symbol string.
    */
    (void) memcpy(symString, old_symString, str_len);

    /*
    ** Copy input string to new symbol string.
    */
    pre_len = strlen(prefix);
    in_len  = 0;
    sym_len = str_len;
    while (strcmp(instring+in_len, "") != 0)
    {
	strcpy(symString+sym_len, prefix);
	sym_len += pre_len;

	strcat(symString+sym_len, instring+in_len);
        len      = strlen(instring+in_len) + 1;
	in_len  += len;
	sym_len += len;
    }

    /*
    ** Append null string to end.
    */
    (void) strcat(symString+number_of_bytes, "");

    /*
    ** Place entries from symbol string into hash table.
    */
    key_ptr    = symKeyTable;
    value_ptr  = symValueTable;
    item_count = 0;
    for (sym_len  = 0; strcmp(string = symString+sym_len, "") != 0;
	 sym_len += strlen(string) + 1)
    {
	(void) sscanf(string, "%[^: \t]", key_ptr);
        (void) strcpy(value_ptr, string + strlen(key_ptr) + 1);
        (void) sscanf(key_ptr, "%s", key_ptr);
	item.key   = key_ptr;
	item.data  = value_ptr;
	key_ptr   += strlen(key_ptr) + 1;
	value_ptr += strlen(value_ptr) + 1;
        item_count++;

	if (hsearch(item, ENTER) == NULL)
        {
	    errno = ENFILE;
	    return(-1);
	}
    }

    /*
    ** Cleanup.
    */
    free(old_symString);
    free(old_symKeyTable);
    free(old_symValueTable);

    return(item_count);
}


void
symDestroy (void)
{
    if (symString != NULL)
	free(symString);

    (void) hdestroy();

    return;
}


int
symSearch (char *key, char *format, void *value)
{
    ENTRY		item, *found_item;


    /*
    ** Lookup keyword in hash table.  If not found, return error.
    */
    item.key = key;
    if ((found_item = hsearch(item, FIND)) == NULL)
    {
	errno = ENXIO;
	return(-1);
    }

    /*
    ** Convert entry to specified format.
    */
    return(sscanf(found_item->data, format, value));
}


int
symGetString (char **string)
{
    int			str_len;


    /*
    ** Check if symCreateString has been called.
    */
    if (symString == NULL)
    {
	errno = ENXIO;
	return(-1);
    }

    /*
    ** Count the number of charaters in the symbol string 
    ** including terminator.
    */
    for (str_len  = 0; strcmp(symString+str_len, "") != 0;
	 str_len += strlen(symString+str_len) + 1);
    str_len++;

    /*
    ** Allocate string to return to user.
    */
    if ((*string = (char *) malloc(str_len * sizeof(char))) == NULL)
	return(-1);
  
    /*
    ** Copy symbol string to user string.
    */
    (void) memcpy(*string, symString, str_len);

    return(str_len);
}
