
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

const int R1 = 7;
const int R2 = 6;
const int R3 = 5;
const int R4 = 4;
const int D1 = 2;
const int D2 = 3;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  Serial.write("READY\n");
  // reserve 200 bytes for the inputString:
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  inputString.reserve(200);
  
}

void loop() {
  // print the string when a newline arrives:
  int pin = 0;
  String state = "";
  if (stringComplete) {
    //Serial.write((char)inputString[2]);
    if (inputString[0] == '?') {
      switch (inputString[1]) {
        case '1':
          state = (String)digitalRead(D1);
          break;
        case '2':
          state = (String)digitalRead(D2);
          break;
        case 'A':
          switch (inputString[2]) {
            case '0':
              state = (String)(((float)analogRead(A0) * 5) / 1023);
              break;
            case '1':
              state = (String)(((float)analogRead(A1) * 5) / 1023);
              break;
            case '2':
              state = (String)(((float)analogRead(A2) * 5) / 1023);
              break;
            case '3':
              state = (String)(((float)analogRead(A3) * 5) / 1023);
              break;
            case '4':
              state = (String)(((float)analogRead(A4) * 5) / 1023);
              break;
            case '5':
              state = (String)(((float)analogRead(A5) * 5) / 1023);
              break;
            default:
              Serial.print("ERR\n");
            break;
          }
          break;
        default:
          Serial.print("ERR\n");
          break;
      }
      Serial.print(state);
      Serial.print('\n');
    }
    else if (inputString[0] == 'C') {
      Serial.print("READY\n");
    }
    else {
      //int state_int = (int)inputString[1];
      //Serial.println(inputString[1]);
      switch (inputString[0]) {
        case '1':
          pin = R1;
          break;
        case '2':
          pin = R2;
          break;
        case '3':
          pin = R3;
          break;
        case '4':
          pin = R4;
          break;
        default:
          Serial.print("ERR\n");
          break;
      }
      switch (inputString[1]) {
        case '1':
          digitalWrite(pin, HIGH);
          Serial.print("ACK\n");
          break;
        case '0':
          digitalWrite(pin, LOW);
          Serial.print("ACK\n");
          break;
        default:
          Serial.print("ERR\n");
          break;
      }
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
