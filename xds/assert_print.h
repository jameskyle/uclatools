# ifndef NDEBUG
# define _assert_print(ex,str)    {if (!(ex)){(void)fprintf(stderr,"Assertion failed: file \"%s\", line %d\nMessage: %s\n\n", __FILE__, __LINE__, (str));exit(1);}}
# define assert_print(ex, str)     _assert_print(ex, str)
# else
# define _assert_print(ex, str)
# define assert_print(ex, str)
# endif
