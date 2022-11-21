#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);


#define PERIODO 300
#define ALTAVOZ 11
#define INTERRUPTOR 2

float notas[25] = {391.99, 391.99, 440.0, 391.99, 523.25, 493.88, 391.99, 391.99, 440.0, 391.99, 587.33, 523.25, 523.25, 659.26, 783.98, 659.26, 523.25, 493.88, 440.0, 698.46, 698.46, 659.26, 523.25, 587.25, 523.25};
float tiempo[25] = {0.5, 0.5, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 2};
int contadorLeds = 5;
int ciclo = 0;
int contadorTiempo = 0;
boolean inicioPrograma = false;
long tiempo_anterior = 0;
long diferencia = 0;




void setup() {
  display_init();
  Serial.begin(9600);
  for (int i = 5; i < 10; i++) { //definimos pines para los LED (del 5 al 9) --> en protoboard, de dcha a izq va de menor a mayor
    pinMode(i, OUTPUT);
  }

  pinMode(INTERRUPTOR, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPTOR), inicio_programa, FALLING);
}

void loop() {
      if(inicioPrograma==true){
         display.clearDisplay();
         display.display();
        funcionalidad_proyecto();
        inicioPrograma=false;
      }
}

void manejadorDeLeds(int periodo, int off_on) {

  if (contadorLeds == 10) {
    if (off_on == LOW) ++ciclo;
    for (int i = 5; i < 10; i++) {
      digitalWrite(i, off_on);
    }
  } else {
    digitalWrite(contadorLeds, off_on);
  }
}

void ajustarContadorLeds() {
  ++contadorLeds;
  if (contadorLeds > 10) {
    if (ciclo == 3) {
      contadorLeds = 10;
    } else {
      contadorLeds = 5;
    }
  }
}

void sonarNotaMusical (int frecuencia, int periodo,int i) {
  tone (ALTAVOZ, frecuencia, periodo);
  manejadorDeLeds(periodo, HIGH);
 
  delay(periodo);
  noTone(ALTAVOZ);
  manejadorDeLeds(periodo, LOW);
  ajustarContadorLeds();
}

void display_init() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //inicializamos display en la direcion 0x3C
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(11, 0);

}

void dibujar_recta (int desvio_inferior, int centro, int desvio_superior, int fila) {
  {
    display.drawPixel(centro-desvio_inferior, fila, WHITE);
    display.drawPixel(centro+desvio_superior,fila,WHITE);
    display.display();
  }
}

void inicio_programa() {
  Serial.println("Se incicia programa");
  diferencia = millis() - tiempo_anterior;
  Serial.println(diferencia);
  if (diferencia > 7000) {
    Serial.println("Estamos dentro del if");
    inicioPrograma=true;
    
  }
  tiempo_anterior = millis();
}

void funcionalidad_proyecto() {
  Serial.println("Estamos en la funcionalidad");
  for (int i = 0; i < sizeof(notas) / sizeof(notas[0]); i++) {
    sonarNotaMusical(notas[i], PERIODO * tiempo[i],i);
    dibujarCorazon(i);
    
  }
  contadorLeds = 5;
  ciclo = 0;
}
void dibujarCorazon(int i){
   switch (i) {
      case 5:
        display.setCursor(11,0);
        display.println("FELICIDADES");
        display.display();
        break;
      case 10:
        display.setCursor(28, 20);
        display.println("¡TE QUEREMOS!");
        display.display();
        break;
      case 11:
        dibujar_recta (0, 64, 0, 58);
        break;
      case 12:
        dibujar_recta (1, 64, 1, 57);
        break;
      case 13:
        dibujar_recta (2, 64, 2, 56);
        break;
      case 14:
        dibujar_recta (3, 64, 3, 55);
        break;
      case 15:
        dibujar_recta (4, 64, 4, 54);
        break;
      case 16:
        dibujar_recta (5, 64, 5, 53);
        break;
      case 17:
        dibujar_recta (6, 64, 6, 52);
        break;
      case 18:
        dibujar_recta (6, 64, 6, 51);
        break;
      case 19:
        dibujar_recta (6, 64, 6, 50);
        dibujar_recta(0,64,0,50);
        break;
      case 20:
        dibujar_recta (2, 64 - 3, 2, 49);
        dibujar_recta (2, 64 + 3, 2, 49);
        break;
      case 21:
        dibujar_recta (1, 64 - 3, 1, 48);
        dibujar_recta (1, 64 + 3, 1, 48);
        break;
    }
}
