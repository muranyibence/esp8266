const int pin = 7;

void toggle() {
  static int state = 0;
  state = !state;
  digitalWrite(BUILTIN_LED, state);
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(pin, INPUT);
  attachInterrupt(pin, toggle, CHANGE);
  toggle();
}

void loop() {}
