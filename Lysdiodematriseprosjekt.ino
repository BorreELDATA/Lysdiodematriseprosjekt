// #define tar ingen plass på minnet i Arduino. 
// Kompilereren bytter ut referansene med dets
// eksakte verdi når programmet kjøres.
// DATA, SHIFT, STORE er de respektive utgangene på Arduinoen.
#define DATA 2
#define SHIFT 4
#define STORE 6
#define amountOfLetters 7
#define amountOfParts 8 // En bokstav består av 8 deler.
#define interval 800 // 0.8 sekund.
unsigned long currentTime;
unsigned long previousTime;
int counter = 0;
// En todimensjonal array som fungerer som en tabell for bokstavene.
const int letters[amountOfLetters][amountOfParts] = { 
  {0, 68, 68, 68, 124, 68, 68, 68},
  {0, 0, 24, 36, 36, 60, 36, 36},
  {0, 112, 72, 72, 80, 96, 80, 72},
  {0, 0, 24, 36, 36, 60, 36, 36},
  {0, 32, 32, 32, 32, 32, 32, 60},
  {0, 48, 40, 36, 36, 40, 48, 0},
  {0, 0, 108, 146, 130, 68, 40, 16}
};

// Lagrer verdien i bitregisteret.
void store() {
  digitalWrite(STORE, HIGH);
  delayMicroseconds(1);
  digitalWrite(STORE, LOW);
  delayMicroseconds(1);
}

// Oppdaterer en individuel del av en bokstav og lagrer den.
void scan(int i, int part) {
  // LSBFIRST betyr Least Significant Bit First (høyre).
  // Bits byttes ut før verdien lagres til bitregisteret.
  shiftOut(DATA, SHIFT, LSBFIRST, ~part);
  shiftOut(DATA, SHIFT, LSBFIRST, 128 >> i);
  store();
}

// Oppdaterer bokstavens åtte deler, hele bokstaven.
void scanLetter(int letter) {
  for (int i = 0; i < amountOfParts; i++) {
    scan(i, letters[letter][i]);
  }
}

// Oppdaterer telleren. 
void updateCounter() {
  currentTime = millis();

  // En sjekk som venter til intervallet er
  // over før den gjør handlingen på nytt.
  if (currentTime < previousTime + interval) return;
  previousTime = currentTime;

  // Telleren øker med én. Om telleren blir lik eller
  // overstiger antallet bokstaver blir telleren satt til null.
  counter++;
  if (counter >= amountOfLetters) counter = 0;
}

void setup() {
  // Registrer de forskjellige pinmodusene til utganger.
  pinMode(DATA, OUTPUT);
  pinMode(SHIFT, OUTPUT);
  pinMode(STORE, OUTPUT);
  store();
}

void loop() {
  updateCounter();
  scanLetter(counter);
}
