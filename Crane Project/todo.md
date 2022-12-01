<!-- # Scratchpad

with a push of a button, the crane should move a weight from one position to another
    (triggered by serial/bt/button)

the route can be changed by going into recording mode
    (phone/ serial)

the route is previewed by pressing the start button then is saved to eeprom
    (phone / serial/ button) -->


## Record Mode
Entered via a button press OR **"Calibrate"** via the serial/bt terminal. Should have feedback

### In mode
- Can manually move the servos with a potentiometer and the stepper motor with buttons/encoder. Should have feedback
- A pushbutton can be pressed to save the location. Should have feedback
- Upon completetion save to EEPROM and send feedback.

<!-- ## Calibrate Mode
Entered via a command or at startup

### In mode
- Should home the servo motor and reset ticks
- Shoulc home the servos and reset ticks -->

## Action mode
Default mode after calibrate is complete

### In mode
- Press a button to run the recorded steps, should have feedback