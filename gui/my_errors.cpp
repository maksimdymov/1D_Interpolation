#include "my_errors.h"

// Prints error message by code of error and returns this code
int
PrintErrorMsgByCode (int code, char *msg)
{
  switch (code)
    {
    case SUCCESS:
      break;

    case MAIN_ARGS_ERROR:
      printf ("Usage: %s\n", msg);
      break;

    case FILE_OPENING_ERROR:
      printf ("Cannot open file %s\n", msg);
      break;

    case FILE_READING_ERROR:
      printf ("Cannot read from file %s\n", msg);
      break;

    case ALLOCATE_MEMORY_ERROR:
      printf ("Cannot allocate %s bytes\n", msg);
      break;

    case END_OF_FILE:
      printf ("Reached end of file %s, but arrays wasn't completely filled\n",
              msg);
      break;

    case TOO_MANY_INFO:
      printf ("Too many information in file %s\n", msg);
      break;

    case CANNOT_SOLVE:
      printf ("Cannot solve\n");
      break;

    default:
      printf (
          "Sorry, something went wrong...\nError code: %d\nError message: %s",
          code, msg);
      break;
    }
  return code;
}