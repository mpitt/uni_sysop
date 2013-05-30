#include <unistd.h>
#include <errno.h>  /* Standard error numbers */
#include <getopt.h>

#include "log.h"
#include "queue.h"
#include "raxor.h"
#include "threads.h"

void print_usage() {
/* Print help and usage. This function tells the user how this program works.
*/
  printf("Usage: raxor [OPTION] ...\n");
  printf("Encode and decode strings from standard input.\n");
  printf("Exit with 'quit'\n\n");
  printf("Options:\n-s, --sleep <time>\n");
  printf("\tSleep for <time> seconds before elaborating an item\n");
  printf("-v, -c, --version, --credits\n");
  printf("\tDisplay version informations, credits and quit\n");
  printf("-h, --help\n");
  printf("\tDisplay this message and quit\n");
}

void print_credits() {
/* Print credits. This function tells the user who made this program.
*/
  printf("raXor version 1.0\n");
  printf("by Mattia Larentis, Andrea Panizza, Michele Pittoni\n");
}

int main(int argc, char** argv) {
  int opt, long_index=0;
  char * eoa;
  
/* long_options sets all the parameters that the program allows in input */
  static struct option long_options[] = {
    {"help",      no_argument,       0,  'h' },
    {"sleep",     required_argument, 0,  's' },
    {"version",   no_argument,       0,  'v' },
    {"credits",   no_argument,       0,  'c' },
    {0,           0,                 0,  0   }
  };
  quit = 0;
  sleep_time = 0;
  
/* Parse all the options */
  while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1) {
    switch (opt) {
      case 'h' :
        print_usage();
        return 0;
      case 's' :
        if (optarg != NULL) {
          sleep_time = strtol(optarg, &eoa, 10);
          if (eoa == optarg || errno == ERANGE) {
            printf("Warning: supplied argument must start with an integer\n");
            printf("No valid sleep interval supplied, using %d\n", sleep_time);
          }
        }
        break;
      case 'v':
      case 'c':
        print_credits();
        return 0;
      default:
        break;
    }
  }

  log_init();
  init_queue(&q);
  
  semInit();
  threadInit();

  threadClose();
  semClose();

  log_close();

  return 0;
}
