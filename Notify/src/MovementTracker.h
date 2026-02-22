#ifndef MOVEMENT_TRACKER_H
#define MOVEMENT_TRACKER_H

#include <Arduino.h>

#define STATE_IDLE 1
#define STATE_MOVING 2

class MovementTracker {
private:
    int lastPattern;
    int movementStartFloor;
    int movementEndFloor;

public:
    MovementTracker();
    int trackMovement(int currentPattern, float currentFloor);
};

#endif
