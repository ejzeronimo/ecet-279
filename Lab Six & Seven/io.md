# IO Connections

| Inputs Hardware    | Controller In |
| ------------------ | ------------- |
| Start profile pb   | PORTA.4       |
| Stop PWM out       | PORTA.5       |
| Potentiometer      | PORTK.0       |
| Pot change mode sw | PORTA.0       |

| Outputs            | Controller Out      |
| ------------------ | ------------------- |
| Test LED           | PORTB.7             |
| DC motor           | Timer1 OC1A PORTB.5 |
| Internal 1ms delay | Timer 0             |