#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

MFRC522 lectorGarita(53, 49);
MFRC522 lectorSalida(48, 47);
Servo barreraGarita;

unsigned long tiempoGarita = 0;
int estadoGarita = 0; // 0: Espera, 1: Validando

void inicializarAccesos() {
    SPI.begin();
    lectorGarita.PCD_Init();
    lectorSalida.PCD_Init();
    barreraGarita.attach(4);
    barreraGarita.write(0);
}

void ejecutarFSM_Garita() {
    if (estadoGarita == 0 && lectorGarita.PICC_IsNewCardPresent() && lectorGarita.PICC_ReadCardSerial()) {
        String uidLeido = obtenerUID(lectorGarita);
        
        // Buscar camión y validar manifiesto pendiente en tablaOperaciones
        if (validarIngreso(uidLeido)) {
            barreraGarita.write(90);
            tiempoGarita = millis();
            estadoGarita = 1;
        } else {
            Serial.println("RECHAZO: Sin manifiesto pendiente o camión no autorizado");
        }
    }
    
    // Cierre seguro basado en ultrasonido y tiempo
    if (estadoGarita == 1 && (millis() - tiempoGarita >= 4000)) {
        barreraGarita.write(0);
        estadoGarita = 0;
    }
}

bool validarIngreso(String uid) {
    for (int i = 0; i < 6; i++) {
        if (tablaOperaciones[i].rfidCamion == uid && tablaOperaciones[i].pendiente) {
            return true;
        }
    }
    return false;
}