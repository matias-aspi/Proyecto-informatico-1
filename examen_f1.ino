#define TRIG 			3
#define ECHO 			4
#define PIR 			5
#define BOTON 			2
#define POTENCIOMETRO 	7
#define BUZZER 			8
#define LED_R 			9
#define LED_G 			10
#define LED_B 			11
#define TMP   			A2
#define luz   			A3

int estadoSistema = 0;
int ultimoBoton = 0;
int lecturaBoton = 0;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(TMP, INPUT);
  pinMode(luz, INPUT);
  pinMode(POTENCIOMETRO, INPUT);
  pinMode(ECHO, INPUT);
  pinMode(PIR, INPUT);
  pinMode(BOTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  Serial.begin(9600);
}

long medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duracion = pulseIn(ECHO, HIGH);
  long distancia = duracion * 0.034 / 2;
  return distancia;
}

void setLED(boolean r, boolean g, boolean b) {
  digitalWrite(LED_R, r);
  digitalWrite(LED_G, g);
  digitalWrite(LED_B, b);
}

void loop() {
  lecturaBoton = digitalRead(BOTON);
  if (lecturaBoton == LOW && ultimoBoton == HIGH) {
    estadoSistema++;
    if (estadoSistema > 3) estadoSistema = 0;
    delay(200);
  }
  ultimoBoton = lecturaBoton;

  long distancia = medirDistancia();
  int movimiento = digitalRead(PIR);

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm - PIR: ");
  if (movimiento == HIGH) Serial.print("Movimiento");
  else Serial.print("Sin movimiento");
  Serial.print(" - Estado Sistema: ");
  if (estadoSistema == 0) Serial.println("DESACTIVADO");
  else Serial.print("ACTIVADO Modo "); Serial.println(estadoSistema);

  if (estadoSistema == 0) {
    if (movimiento == HIGH) {
      setLED(HIGH, HIGH, LOW); // Amarillo
      Serial.println("Movimiento detectado con alarma desactivada");
    } else {
      setLED(LOW, LOW, HIGH); // Azul
    }
    digitalWrite(BUZZER, LOW);
  } 
  else if (estadoSistema == 1) {
    if (distancia > 200) {
      setLED(LOW, HIGH, LOW); // Verde
      digitalWrite(BUZZER, LOW);
    }
  } 
  else if (estadoSistema == 2) {
    if (distancia <= 200 && distancia >= 80) {
      setLED(HIGH, HIGH, LOW); // Naranja/Amarillo
      digitalWrite(BUZZER, HIGH);
      delay(200);
      digitalWrite(BUZZER, LOW);
      delay(200);
      Serial.println("Advertencia: objeto aproximandose");
    }
  } 
  else if (estadoSistema == 3) {
    if (distancia < 80 && movimiento == HIGH) {
      setLED(HIGH, LOW, LOW); // Rojo
      digitalWrite(BUZZER, HIGH);
      delay(35);
      digitalWrite(BUZZER, LOW);
      delay(35);
      Serial.println("ALARMA CRiTICA");
    }
  }
  delay(100);
}