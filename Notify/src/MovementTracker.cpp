#include "MovementTracker.h"

MovementTracker::MovementTracker() {
    lastPattern = STATE_IDLE;
    movementStartFloor = -1;
    movementEndFloor = -1;
}

int MovementTracker::trackMovement(int currentPattern, float currentFloor) {
    static int trajectory = -1;

    if (lastPattern == STATE_IDLE && currentPattern == STATE_MOVING) {
        movementStartFloor = round(currentFloor);
        // Serial.print("Movement started at floor: ");
        // Serial.println(movementStartFloor);
    } 
    else if (lastPattern == STATE_MOVING && currentPattern == STATE_IDLE) {
        movementEndFloor = round(currentFloor);
        // Serial.print("Movement ended at floor: ");
        // Serial.println(movementEndFloor);

        Serial.print("Movement: from floor ");
        Serial.print(movementStartFloor);
        Serial.print(" to floor ");
        Serial.println(movementEndFloor);

        trajectory = ((movementEndFloor & 0x0F) << 4) | (movementStartFloor & 0x0F);
        movementStartFloor = -1;
        movementEndFloor = -1;
    }

    lastPattern = currentPattern;
    return trajectory;
}
