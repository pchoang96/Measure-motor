#define clkw        0
#define c_clkw      1
#define encodPinA1  3
#define M1_p        6
#define M1_l        7
#define encodPinA2  2
#define M2_p        5
#define M2_l        4
#define DEBUG       1
//double kp = 5 , ki = 1 , kd = 0.01 ,input = 0, output = 0, setpoint = 0;
int pwm=110, pulse=0, sample = 0, i=0, x= 0;
int sample_time, timer_value = sample_time;//(ms)
bool l_dir=clkw, r_dir=clkw;
double l_p=0,r_p=0;
long temp;
void setup() 
{
  Serial.begin(9600);  
  pinMode(M1_l,OUTPUT);
  pinMode(M2_l,OUTPUT);
  pinMode(encodPinA1, INPUT_PULLUP);                  // encoder input pin
  pinMode(encodPinA2, INPUT_PULLUP);
  attachInterrupt(0, encoder_1 , FALLING);               // update encoder position
  attachInterrupt(1, encoder_2 , FALLING);
  //--------------setup timer------------------------------------------ 
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  TCCR1B |= (1 << CS11) | (1 << CS10);    // prescale = 64 4us per pulse
  TCNT1 = 60535; //(5000*4)=20ms
  TIMSK1 |= (1 << TOIE1);                  // Overflow interrupt enable 
  sei();                                  // enable all interrupt
}
void loop() {
  }
void pwmOut(int out,int M1,int M2) 
{                                // to H-Bridge board
    analogWrite(M1, out);                             // drive motor CW    
    analogWrite(M2,0);    
  }
  
/**--------------------------------------------------------------**/
void encoder_1()
{
  l_p ++;
}
/*----------------------------------------------------------*/
void encoder_2()
{  
  r_p ++;
}
/**--------------------------------------------------------------**/

ISR(TIMER1_OVF_vect) 
{
  sample++;
  i++;
  Serial.println( (String) sample +" "+ pwm  + " " +l_p + "  "  +r_p);
  if (i==30&& sample<1000) 
  {
    pwm=random(40,250); 
    pwmOut(pwm,M1_p,M1_l);
    pwmOut(pwm,M2_p,M2_l);
    i=0;
  }
  l_p=0;
  r_p=0;
  TCNT1 = 60535;
   }