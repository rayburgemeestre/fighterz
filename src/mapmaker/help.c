#include "header.h"

char help_text[9200];
void init_help() {
  FILE *fp;

  if (!(fp = fopen("help.txt", "r"))) {
    // alert("Error:", "Could not open help.txt, help not available", "",
    //     "Ok", NULL, 0, 0);
    strcpy(help_text, "Help not available (could not find / open help.txt)");
    return;
  }
  // fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
  if (!fread(help_text, sizeof(char), sizeof(help_text), fp)) die("fread(); failed");
}
