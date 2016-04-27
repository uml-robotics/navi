#define PIN_BUZZER 2
#define PIN_LIGHT_RELAY 3
#define BUFF_LEN 100
#define STATE_ON 0
#define STATE_OFF 1
#define STATE_TURNING_ON 2
#define STATE_TURNING_OFF 3



int state = STATE_OFF;
int buzzer_cnt = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LIGHT_RELAY, OUTPUT);
  digitalWrite(PIN_LIGHT_RELAY, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  char buffer[BUFF_LEN];
  char cur;
  int i = 0;
  
  if(Serial.available() && state != STATE_TURNING_ON && state != STATE_TURNING_OFF) {
    while(i < BUFF_LEN-1) {
      if((cur = Serial.read()) < 0) continue;
      if(cur == '\n') break;
      buffer[i++] = cur;
    }
    buffer[i] = '\0';
    if(isDigit(buffer[0])) { // atoi can only work if first char is number error returns 0 which is useless
      int val = atoi(buffer);
      // state 0 just turned it on
      // state 1 controller started/ logic power on
      // state 2-7 capacitor charging
      // state 8 power on
      // state 10 estopped
      // state 11 motor power off
      if(val > 1 &&  val < 10) {
        if(state != STATE_ON) {
          state = STATE_TURNING_ON;
          buzzer_cnt = 474;
        }
      } else {
        if(state != STATE_OFF) {
          state = STATE_TURNING_OFF;
          buzzer_cnt = 149;
        }
      }
    }
    Serial.println(buffer);  
  }
  
  if(state == STATE_TURNING_ON) {
    digitalWrite(PIN_LIGHT_RELAY, HIGH);
    if(buzzer_cnt > 0) {
      if(buzzer_cnt%100 < 75) 
        digitalWrite(PIN_BUZZER, HIGH);
      else
        digitalWrite(PIN_BUZZER, LOW);
      --buzzer_cnt;  
    } else {
      digitalWrite(PIN_BUZZER, LOW);
      state = STATE_ON;
    }
  } else if(state == STATE_TURNING_OFF) {
    if(buzzer_cnt > 0) {
        if(buzzer_cnt%50 < 25) 
          digitalWrite(PIN_BUZZER, HIGH);
        else
          digitalWrite(PIN_BUZZER, LOW);
        --buzzer_cnt;  
    } else {
        digitalWrite(PIN_BUZZER, LOW);
        digitalWrite(PIN_LIGHT_RELAY, LOW);
        state = STATE_OFF;
    }  
  }
  delay(10);        // delay in between reads for stability
}
