# Pinout

| Input            | PIN |
| ---------------- | --- |
<!-- upper nibble of motor port -->
| Hot Switch       | A.4 |
| Warm Switch      | A.5 |
| Cold Switch      | A.6 |
| Door Open Switch | A.7 |
<!-- put start button on unique port -->
| Start Pushbutton | K.0 |

| Output           | PORT |
| ---------------- | ---- |
<!-- lower nibble to reuse stepper motor code -->
| Motor 1          | A.0  |
| Motor 2          | A.1  |
| Motor 3          | A.2  |
| Motor 4          | A.3  |
<!-- all LEDs on same port -->
| Wash Done LED    | C.0  |
| Agitate LED      | C.1  |
| Spin LED         | C.2  |
<!-- skip C.3 for buffer LED -->
| Drain Valve      | C.4  |
| Hot Water Valve  | C.5  |
| Cold Water Valve | C.6  |