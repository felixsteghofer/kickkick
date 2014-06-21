/*

Compile:
    gcc -o oKicker oKicker.c -lwiringPi
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <math.h>
#include <inttypes.h>
#include <sys/time.h>

// Use GPIO Pin 17, which is Pin 0 for wiringPi library

#define BUTTON_PIN 0


volatile int goals = 0;

volatile long long lastGoalTime;

long long currentTimestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

void goal(void) {
   if (goals == 0) {
     lastGoalTime = currentTimestamp();
     printf("game initilized\n");
   } else if ((currentTimestamp() - lastGoalTime) < 1000) {
     printf("this interrupt should be no new goal..\n");
     return;
   }
   lastGoalTime = currentTimestamp();
   printf("%lu\n", lastGoalTime);
   goals++;
}


int main(void) {
  // sets up the wiringPi library
  if (wiringPiSetup () < 0) {
      fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
      return 1;
  }

  // set Pin 17/0 generate an interrupt on high-to-low transitions
  // and call goal() on interrupt
  if ( wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, &goal) < 0 ) {
      fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
      return 1;
  }
  while(1) {
    printf("%d\n", goals);
    delay(2000);
  }

  return 0;
}
