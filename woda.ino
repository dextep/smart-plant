#include <OneWire.h>
#include <DallasTemperature.h>

#define PIN_Humidity A0
#define ODSTEP 900000
#define ONE_WIRE_BUS 12
#define PIN_LDR A1
#define PIN_PUMP 5


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

// informacje o odczytach
int suma_odczytow = 0;
int liczba_odczytow = 0;
unsigned long czas_poprzedni = 0;

float getTemp(int samples, int timeBetween, DeviceAddress deviceAddress){
  int sum = 0;
  for( int i=0; i<samples; i++){
    sensors.requestTemperatures();
    sum += sensors.getTempC(deviceAddress);
    delay (timeBetween);
  }
  return (sum / samples);
}

float getHumidity(int samples, int timeBetween, int pin){
    int sum = 0;
    for( int i=0; i<samples; i++){
      sum += analogRead(pin);
      delay (timeBetween);
    }
    return  ((1023 - (sum / samples)) / 1023.0) * 100;
}

void setup() {
  sensors.begin();
  sensors.getAddress(insideThermometer, 0);
  sensors.setResolution(insideThermometer, 9);
  
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  pinMode(PIN_Humidity, INPUT);
  digitalWrite(5, HIGH);
  Serial.begin(9600); 
}

void loop() {
  // czas liczony od momentu wlaczenia urzadzenia 
  unsigned long czas_aktualny = millis(); 
  // put your main code here, to run repeatedly:
  float x = getHumidity(10, 100, PIN_Humidity);
  Serial.print( x );
  Serial.print( " " );
  Serial.print( analogRead( PIN_LDR )/10 );
  Serial.print(" ");
  sensors.requestTemperatures();
  Serial.print( sensors.getTempC(insideThermometer) );
  Serial.print(" ");
  Serial.println( "0" );
  
  // sprawdza czy uplynal juz okreslony czas miedzy pomiarami
  if(czas_aktualny - czas_poprzedni > ODSTEP) {
    czas_poprzedni = czas_aktualny;
    if( x < 50 ){
      Serial.print( x );
      Serial.print( " " );
      Serial.print( analogRead( PIN_LDR )/10 );
      Serial.print(" ");
      Serial.print( sensors.getTempC(insideThermometer) );
      Serial.print(" ");
      Serial.println( "1" );
      digitalWrite(PIN_PUMP, LOW);    // turn the LED off by making the voltage LOW
      delay(500);
      digitalWrite(PIN_PUMP, HIGH);   // turn the LED on (HIGH is the voltage level)
    }else{

    }

  }
  delay( 100 );
}
