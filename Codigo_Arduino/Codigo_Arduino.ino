// Definir el pin de control del relé y el sensor de humedad digital
const int relePin = 15;          // Pin donde está conectado el relé
const int sensorHumedadPin = 12;   // Sensor de humedad conectado a D12

// Variable para definir el modo de operación:
// false = Modo MANUAL, true = Modo AUTOMÁTICO
bool modoAutomatico = false;

void setup() {
  // Iniciar la comunicación serie
  Serial.begin(9600);

  // Configurar el pin del relé como salida
  pinMode(relePin, OUTPUT);
  // Asegurarse de que la válvula esté apagada al inicio
  digitalWrite(relePin, LOW);

  // Configurar el sensor de humedad como entrada digital
  pinMode(sensorHumedadPin, INPUT);

  // Mensaje de bienvenida e instrucciones
  Serial.println("Control de la válvula:");
  Serial.println("-> Escribe 'ON' para activar y 'OFF' para desactivar la válvula (modo MANUAL).");
  Serial.println("-> Escribe 'AUTO' para activar el control automático basado en el sensor de humedad.");
  Serial.println("-> Escribe 'MANUAL' para regresar al control manual.");
}

void loop() {
  // Procesamiento de comandos enviados por el monitor serial
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n'); // Lee el comando hasta un salto de línea
    comando.trim(); // Elimina espacios o saltos extra

    // Si se está en modo MANUAL, se procesan los comandos ON/OFF
    if (comando == "ON" && !modoAutomatico) {
      digitalWrite(relePin, HIGH);
      Serial.println("Válvula activada (modo MANUAL).");
    } 
    else if (comando == "OFF" && !modoAutomatico) {
      digitalWrite(relePin, LOW);
      Serial.println("Válvula desactivada (modo MANUAL).");
    } 
    else if (comando == "AUTO") {
      modoAutomatico = true;
      Serial.println("Modo AUTOMÁTICO activado. El sensor de humedad controlará la válvula.");
    } 
    else if (comando == "MANUAL") {
      modoAutomatico = false;
      Serial.println("Modo MANUAL activado. Ahora puedes controlar la válvula con ON/OFF.");
    } 
    else {
      Serial.println("Comando no reconocido. Usa ON, OFF, AUTO o MANUAL.");
    }
  }
  
  // Si el modo AUTOMÁTICO está activado, se lee el sensor digital de humedad y se actúa en consecuencia
  if (modoAutomatico) {
    int lecturaHumedad = digitalRead(sensorHumedadPin);
    
    // Se asume que:
    //   HIGH -> Suelo seco (se requiere riego)
    //   LOW  -> Suelo húmedo (no se requiere riego)
    Serial.print("Lectura sensor de humedad (digital): ");
    if (lecturaHumedad == HIGH) {
      Serial.println("Seco");
      digitalWrite(relePin, HIGH);  // Activa la válvula
      Serial.println("Suelo seco: válvula activada.");
    } else {
      Serial.println("Húmedo");
      digitalWrite(relePin, LOW);   // Desactiva la válvula
      Serial.println("Suelo húmedo: válvula desactivada.");
    }
    
    // Pausa para evitar saturar el monitor serie
    delay(2000);
  }
}