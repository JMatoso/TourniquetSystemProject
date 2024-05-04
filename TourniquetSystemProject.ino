#define LED_PIN_IN 33
#define LED_PIN_OUT 27
#define SWITCH_PIN_IN 23
#define SWITCH_PIN_OUT 22

volatile int numberOfPeopleInside;
volatile int numberOfPeopleWhoGotIn;
volatile int numberOfPeopleWhoGotOut;
volatile bool canSendDataToComputer;

void handleInInterrupt() {
  if (!canSendDataToComputer) {
    numberOfPeopleInside++;
    numberOfPeopleWhoGotIn++;
    canSendDataToComputer = true;
    digitalWrite(LED_PIN_IN, HIGH);
  }
}

void handleOutInterrupt() {
  if (numberOfPeopleInside > 0 && !canSendDataToComputer) {
    numberOfPeopleInside--;
    numberOfPeopleWhoGotOut++;
    canSendDataToComputer = true;
    digitalWrite(LED_PIN_OUT, HIGH);
  }
}

void sendDataToComputer() {
  Serial.println("\n------ Update");
  Serial.println("Number of People Inside the Building: " + String(numberOfPeopleInside));
  Serial.println("Number of People Who Got Inside the Building: " + String(numberOfPeopleWhoGotIn));
  Serial.println("Number of People Who Got Out Of the Building: " + String(numberOfPeopleWhoGotOut));
  canSendDataToComputer = false;
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN_IN, OUTPUT);
  pinMode(LED_PIN_OUT, OUTPUT);
  pinMode(SWITCH_PIN_IN, INPUT_PULLUP);
  pinMode(SWITCH_PIN_OUT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN_IN), handleInInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN_OUT), handleOutInterrupt, FALLING);
}

void loop() {
  if (canSendDataToComputer) {
    delay(350);
    sendDataToComputer();
    digitalWrite(LED_PIN_IN, LOW);
    digitalWrite(LED_PIN_OUT, LOW);
  }
}
 