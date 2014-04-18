
//------------------------- Define function of each I/O pin -------------------------------------------------

#define LEFTMOTORBRAKE_pin       4  //  D4 - left  motor brake        control    pin    HIGH = Brake 
#define LEFTMOTORDIRECTION_pin   2  //  D2 - left  motor direction    control    pin    HIGH = Forward   Low = Reverse
#define LEFTMOTORPWM_pin         3  //  D3 - left  motor pulse width  modulation pin    0 - 255          Speed and Brake 

#define RIGHTMOTORBRAKE_pin      9  //  D9 - right motor brake        control    pin    HIGH = Brake 
#define RIGHTMOTORDIRECTION_pin 10  // D10 - right motor direction    control    pin    HIGH = Forward   Low = Reverse
#define RIGHTMOTORPWM_pin       11  // D11 - right motor pulse width  modulation pin    0 - 255          Speed and Brake 

#define LEFTMOTORCURRENT_pin     6  //  A6 - left  motor current      monitor    pin    0 - 1023         -20A to +20A   
#define RIGHTMOTORCURRENT_pin     7  //  A7 - right motor current      monitor    pin    0 - 1023         -20A to +20A   
#define VOLTAGE_pin     3  //  A3 - battery voltage          1V = 33.57        30V = 1007

#define XAXIS_pin     0  //  A0 - accelerometer X-axis
#define YAXIS_pin     1  //  A1 - accelerometer Y-axis
#define ZAXIS_pin     2  //  A2 - accelerometer Z-axis

#define IR_pin 000
