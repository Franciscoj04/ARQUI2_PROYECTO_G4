#include "ModelosDatos.h"
#include <TimerOne.h>

volatile bool paroEmergenciaActivo = false;
bool sistemaBloqueado = false;

void setup() {
    Serial.begin(115200);
    
    inicializarAccesos();
    inicializarMecatronica();
    
    // Configuración de Seguridad por hardware (INT0)
    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), ISR_ParoEmergencia, FALLING);
    
    // Timer1 para procesamiento de pesaje en segundo plano
    Timer1.initialize(10000); // 10ms
    Timer1.attachInterrupt(ISR_ProcesarPesaje);
    
    Serial.println("Terminal PORTUS Iniciada.");
}

void loop() {
    verificarSeguridad();
    
    if (!sistemaBloqueado) {
        ejecutarFSM_Garita();
        ejecutarFSM_Transferencia();
        ejecutarFSM_Grua();
        ejecutarFSM_Salida();
    }
}

void ISR_ParoEmergencia() {
    // Detención física de pulsos a los motores paso a paso
    digitalWrite(11, HIGH); // Pin ENABLE de drivers A4988
    paroEmergenciaActivo = true;
}

void verificarSeguridad() {
    if (paroEmergenciaActivo) {
        sistemaBloqueado = true;
        paroEmergenciaActivo = false;
        Serial.println("ERR: PARO DE EMERGENCIA ACTIVADO");
    }
    // Lógica de rearme con botón en Pin 3 conservando el último inventario
    if (sistemaBloqueado && digitalRead(3) == LOW) {
        sistemaBloqueado = false;
        digitalWrite(11, LOW); 
        Serial.println("SISTEMA REARMADO");
        delay(300);
    }
}