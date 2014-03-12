#include <stdlib.h>
#include <stdio.h>

#include "linux_input.h"

typedef struct input_event input_event;

void process_event(input_event event);

int main(int argc, char** argv)
{
  if (argc < 3) {
    printf("Usage: read /path/to/evdev output\n");
    return 1;
  }

  FILE* event_device;
  event_device = fopen(argv[1], "r");
  if (event_device == NULL) {
    printf("Failed to open %s for reading\n", argv[1]);
    return 1;
  }

  FILE* out_file;
  out_file = fopen(argv[2], "w");
  if (out_file == NULL) {
    printf("Failed to open %s for writing\n", argv[2]);
    return 1;
  }

  struct input_event event;
  struct timeval tv;
  struct timeval new_tv;


  while (1) {
    gettimeofday(&tv, NULL);
    fread(&event, sizeof(event), 1, event_device);
    gettimeofday(&new_tv, NULL);
    fprintf(out_file, "raw %hd %hd %d\n", event.type, event.code, event.value);
    int time_msec = ((new_tv.tv_sec - tv.tv_sec) * 1000) +
                    ((new_tv.tv_usec - tv.tv_usec) / 1000);
    fprintf(out_file, "sleep %d\n", time_msec);
    fflush(out_file);
  }

  return 0;
}


