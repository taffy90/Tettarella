//ughv

#define pot_1 A1
#define pot_2 A2
#define pot_3 A3
#define gear_f 12
#define gear_r 13
#define left 2
#define right 3
#define jsb A3


#define for_1 6
#define rev_2 7
#define dir_1 8
#define pwm_1 9
#define dir_2 10
#define pwm_2 11
#define warning 5
#define reset 4

int reading_3 = 0; //this is gear
int reading_4 = 0; //this is gear
int reading_5 = 0; //this is steer
int reading_6 = 0; //this is steer
int reading_7 = 0; //this is jsb

unsigned long start_gear = 0;
unsigned long stop_throttle = 0;
unsigned long start_gear_forward = 0;
unsigned long start_gear_reverse = 0;
unsigned long start_gear_f = 0;
unsigned long start_gear_r = 0;
unsigned long throttle_hold = 0;
unsigned long smonitor = 1000;

int forward = 1;  
int neutral = 0;//this is gear remember
int reverse = 1;
int lastp012 = 1;
int reset_status = 0;
int throttle_hold_done = 0;
const long atwdelay1 = 3500; //start shifting
const long atwdelay2 = 7000; //delay for shifting stop & delay  for throttle &
const long serialdelay = 1000; //
     
void setup() {
    // define pin name
    digitalWrite(reset, HIGH);

pinMode(for_1, OUTPUT);
pinMode(rev_2, OUTPUT);
pinMode(dir_1, OUTPUT);
pinMode(dir_2, OUTPUT);
pinMode(pwm_1, OUTPUT);
pinMode(pwm_2, OUTPUT);
pinMode(warning, OUTPUT);
pinMode(reset, OUTPUT);
pinMode(pot_1,INPUT);
pinMode(pot_2,INPUT);
pinMode(gear_f,INPUT);
pinMode(gear_r,INPUT);
pinMode(left,INPUT);
pinMode(right,INPUT);
pinMode(jsb,INPUT_PULLUP);
Serial.begin(9600);
}
void loop() 

{
unsigned long currentMillis = millis();

if (currentMillis>=serialdelay+smonitor)
{
  Serial.println(" ");
     Serial.println("test");
     //Serial.println("reset_status");
    Serial.println(reset_status);
    // Serial.println("forward");
    Serial.println(forward);
   // Serial.println("neutral");
    Serial.println(neutral);
    // Serial.println("reverse");
    Serial.println(reverse);
     //Serial.println("lastp012");
    Serial.println(lastp012);
       Serial.println(reading_4);
     
     Serial.println(reading_7);
    //delay(250);
    smonitor=currentMillis;
}
    
reading_3= digitalRead(gear_f); //First lets check if the engine is in Neutral before we start ;)
    reading_4= digitalRead(gear_r);
    if (((reading_3!=HIGH)||(reading_4!=HIGH))&&(reset_status==0))
    {
      digitalWrite(warning, HIGH);
      analogWrite(dir_1, 255); //go to lowest throttle
      digitalWrite(pwm_1, HIGH);
      delay(1000);
      digitalWrite(reset, LOW);
    }
    if ((reading_3==HIGH)&&(reading_4==HIGH)&&(reset_status==0))
    {
      reset_status = 1;
      analogWrite(dir_1, 255); //go to lowest throttle
      digitalWrite(pwm_1, HIGH);
      delay(3000);
    }
       
//Throttle
    int reading_1 = 0;
    if ((forward==0)||(reverse==0)&&(throttle_hold_done==0)) //move when value <> and in gear
      {
        
        digitalWrite(warning, LOW);
      int output_1 = 0;
      for (int i=0;i<5;i++)reading_1+= analogRead(pot_1);
      reading_1/=5;
      output_1=reading_1>>2; //convert 10-bit to 8-bit
      analogWrite(dir_1,output_1);
      if ((output_1>=120) && (output_1<=140))
      {digitalWrite(pwm_1,LOW);}
      else {
      Serial.println("gas op die lollie");
      digitalWrite(pwm_1,HIGH);}
      }
   
   if (((neutral==2)||(neutral==3)||(neutral==4)||(neutral==5)||(forward==2)||(forward==3)||(reverse==2)||(reverse==3))&&(throttle_hold_done==0)) //auto throttle down when shifting gear
   {
  Serial.println("Throttle hold");
  digitalWrite(warning, HIGH);
   analogWrite(dir_1, 255); //go to lowest throttle
   digitalWrite(pwm_1, HIGH);
   throttle_hold = currentMillis;
   throttle_hold_done = 1;    
       }
       
       if ((currentMillis >=  throttle_hold + atwdelay1+500) && ((neutral==2)||(neutral==3)||(neutral==4)||(neutral==5)||(forward==2)||(forward==3)||(reverse==2)||(reverse==3))&&(throttle_hold_done==1))
       {
          Serial.println("Throttle hold done");
  digitalWrite(warning, HIGH);
   analogWrite(dir_1, 255); //go to lowest throttle
   digitalWrite(pwm_1, LOW);
   throttle_hold_done = 2;   
   
}   
    
//Steering joystick
  //  int reading_2 = 0; //this is steering
  //  int output_2 = 0;
  //  for (int i=0;i<5;i++)reading_2+= analogRead(pot_2); reading_2/=5;
  //  output_2=reading_2>>2; //convert 10-bit to 8-bit
  //  analogWrite(dir_2,output_2);
  //  delay(30);
  //Serial.println("DIR_2");
  //Serial.println(output_2);
  //  if ((output_2>=120) && (output_2<=140))
  //  {digitalWrite(pwm_2,LOW);}
  //  else 
  //  {
  //    digitalWrite(pwm_2,HIGH);
  //    Serial.println("steering");
  //  }

//Steering buttons
 reading_5= digitalRead(left);
    reading_6= digitalRead(right);
 
if ((reading_5== LOW) && (reading_6==LOW))
{
  digitalWrite(pwm_2, LOW);
}
if ((reading_5== LOW) && (reading_6==HIGH))
{
  digitalWrite(pwm_2, HIGH);
  digitalWrite(dir_2, HIGH);
  Serial.println("left");
}
if ((reading_5== HIGH) && (reading_6==LOW))
{
  digitalWrite(pwm_2, HIGH);
  digitalWrite(dir_2, LOW );
    Serial.println("right");
}
  
//Gears
    
    reading_3= digitalRead(gear_f);
    reading_4= digitalRead(gear_r);
    reading_7= digitalRead(jsb);
    delay(30);
      
  //Warning when set to neutral but not in gear to start from
   // if ((reading_3==HIGH)&&(reading_4==HIGH)&&((neutral==0)||(neutral==)
  //  {
   //   digitalWrite(warning, 1)
  //  }
  //  if ((reading_3==HIGH)&&(reading_4==HIGH)&&(neutral!=0)
  //  {
  //    digitalWrite(warning, 1)
   // }
    
    //all the way to forward
      if ((forward==1)&& (reading_3== LOW) && (reading_4==HIGH)&&(reading_7==HIGH))
      {Serial.println("Forward_1");
        forward = 2;
        neutral = 1;
        reverse = 1;
        reset_status = 1;
        digitalWrite(rev_2, LOW);
        
        start_gear_forward = currentMillis;        
       }
       
       if (((currentMillis >=  start_gear_forward + atwdelay1)||((throttle_hold_done==2)&&(reading_7==HIGH))) && (forward == 2)&& (reading_3== LOW) && (reading_4==HIGH))
       {Serial.println("Forward_2");
           digitalWrite(for_1, HIGH);
        lastp012 = 3;
        forward = 3;
        neutral = 0;
        start_gear_forward = currentMillis; 
          }
        
        if  ((currentMillis >=  start_gear_forward + atwdelay2) && (forward == 3))
          {Serial.println("Forward_3");
           digitalWrite(for_1, LOW);
            forward = 0;
                neutral = 1;
                reverse = 1;
                lastp012 = 2;
                throttle_hold_done = 0;
        }             
       
      
    // all the way to reverse
      if ((reading_3==HIGH) && (reading_4==LOW) && (reverse==1)&&(reading_7==HIGH))
      {Serial.println("Reverse_1");
        forward = 1;
        neutral = 1;
        reverse = 2;
        reset_status = 1;
        digitalWrite(for_1, LOW);
        start_gear_reverse = currentMillis;        
       }

               if (((currentMillis >=  start_gear_reverse + atwdelay1)||((throttle_hold_done==2)&&(reading_7==HIGH))) && (reverse == 2)&& (reading_3==HIGH) && (reading_4==LOW))
          {Serial.println("Reverse_2");
           digitalWrite(rev_2, HIGH);
           digitalWrite(for_1, LOW); //shut down other direction
       lastp012 = 3;
        neutral = 0;
        reverse = 3;
        start_gear_reverse = currentMillis;   
          }
        
        if  ((currentMillis >=  start_gear_reverse + atwdelay2) && (reverse == 3))
          {Serial.println("Reverse_3");
           digitalWrite(rev_2, LOW);
            forward = 1;
        neutral = 1;
        reverse = 0;
        lastp012 = 0;
        throttle_hold_done = 0;
                                             }             
  
  //Neutral
 
  //Neutral from forward
      if ((reading_3== HIGH)&&(reading_7==HIGH) && (reading_4==HIGH) && (neutral==1) && ((forward==0)||(lastp012 == 2)))
      {Serial.println("Neutral from forward_1");
         forward = 1;
        neutral = 2;
        reverse = 1;
        start_gear_f = currentMillis;        
       }

               if (((currentMillis >=  start_gear_f + atwdelay1)||((throttle_hold_done==2)&&(reading_7==HIGH))) && (neutral == 2) && (reading_3== HIGH) && (reading_4==HIGH))
          {Serial.println("Neutral from forward_2");
           digitalWrite(rev_2, HIGH);
                      digitalWrite(for_1, LOW); //shut down other direction
        lastp012 = 3;
         neutral = 3;
        start_gear_f = currentMillis;   
          }
        
        if  ((currentMillis >=  start_gear_f + atwdelay1) && (neutral == 3)&& (reading_3== HIGH) && (reading_4==HIGH))
          {Serial.println("Neutral from forward_3");
           digitalWrite(rev_2, LOW);
            forward = 1;
        neutral = 0;
        reverse = 1;
        lastp012 = 1;
       }
       
      // Neutral from reverse
       if ((reading_3== HIGH) && (reading_4==HIGH) && (neutral==1)&&(reading_7==HIGH) && ((lastp012 == 0)||reverse==0))
      {Serial.println("Neutral from reverse_1");
         forward = 1;
        neutral = 4;
        reverse = 1;
        
        start_gear_r = currentMillis;        
       }

               if (((currentMillis >=  start_gear_r + atwdelay1)||((throttle_hold_done==2)&&(reading_7==HIGH))) && (neutral == 4) && (reading_3== HIGH) && (reading_4==HIGH))
          {Serial.println("Neutral from reverse_2");
           digitalWrite(for_1, HIGH);
                      digitalWrite(rev_2, LOW); //shut down other direction

        neutral = 5;
        lastp012 = 3;
        start_gear_r = currentMillis;   
          }
        
        if  ((currentMillis >=  start_gear_r + atwdelay1) && (neutral == 5)&& (reading_3== HIGH) && (reading_4==HIGH))
          {Serial.println("Neutral from reverse_3");
           digitalWrite(for_1, LOW);
            forward = 1;
        neutral = 0;
        reverse = 1;
        lastp012 = 1;
       }

       //stay in neutral
       if ((reading_3== HIGH) && (reading_4==HIGH) && (neutral==1)&&(lastp012==1) && (((forward==1)&&(reverse==2))||((forward==2)&&(reverse==1))))
 {
  forward = 1;
  neutral = 0;
        reverse = 1;
        
    }
    }
