#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[] )
{
  FILE *fp;
  char path[1024], space[1024], exec_trackpad[1024];
  char *trackpad_name = "Touchpad";
  char *trackpad_regex = "Touchpad        	id=%d %s";
  char *parsed_result;
  char *trackpad_arg;
  int trackpad_id = 0;
  long is_turn_on = 0;
  int found_trackpad = 0;

  if (argc != 2) {
    printf("Need to tell trackpad to turn on or off with one param!\n");
    printf("./trackpad_change 1 -- ON\n");
    printf("./trackpad_change 0 -- OFF\n");
    return -1;
  }
  else {
    // Get 1 or 0 argument
    is_turn_on = strtol(argv[1], &trackpad_arg, 10);
  }
  
  // Execute 'xinput' command to list connected devices with read permission
  fp = popen("xinput list", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  // Read the output of system command line by line
  // Check for the trackpad
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    // Check if 'Touchpad' is the device being listed
    if ((parsed_result = strstr(path, trackpad_name)) != NULL){
      // Get its device id
      if (sscanf(parsed_result, trackpad_regex, &trackpad_id, space) != 0) {
        // Put together system command to change trackpad status
        sprintf(exec_trackpad, "xinput set-prop %d \"Device Enabled\" %ld", 
                trackpad_id, is_turn_on);
        found_trackpad = 1;
        system(exec_trackpad);
      }
    }
  }

  if (!found_trackpad) {
    printf("Couldn't find trackpad, \'%s\' in listed devices:\n", trackpad_name);
    fp = popen("xinput list", "r");
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
      printf("%s\n", path);
    }
  }
  else {
    if (is_turn_on) {
      printf("Trackpad Turned ON\n");
    }
    else printf("Trackpad Turned OFF\n");
  }

  pclose(fp);

  return 0;
}
