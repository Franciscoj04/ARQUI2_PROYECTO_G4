#ifndef MODELOS_DATOS_H
#define MODELOS_DATOS_H

#include <Arduino.h>

// Estructuras de datos locales para la terminal
struct Camion {
    String rfid;
    String placa;
    float tara;
    bool autorizado;
};

struct Contenedor {
    String idContenedor;
    float pesoDeclarado;
    int nivelPatio;
    int celdaPatio;
};

struct Manifiesto {
    String rfidCamion;
    String idContenedor;
    int tipoOperacion; // 1: Depósito, 2: Retiro
    float toleranciaPeso;
    bool pendiente;
};

struct TrabajoGrua {
    int tipoAccion; // 1: Mover a patio, 2: Mover a camión, 3: Remoción
    int posicionOrigen;
    int posicionDestino;
};

// Registro de 3 camiones simultáneos
Camion flota[3] = {
    {"A1B2C3D4", "C-123XYZ", 12000.0, true},
    {"E5F6G7H8", "C-456ABC", 11500.0, true},
    {"I9J0K1L2", "C-789DEF", 13000.0, true}
};

// Tabla de 6 operaciones precargadas combinando depósitos y retiros
Manifiesto tablaOperaciones[6] = {
    {"A1B2C3D4", "CONT-001", 1, 500.0, true}, // Depósito
    {"E5F6G7H8", "CONT-002", 2, 500.0, true}, // Retiro
    {"I9J0K1L2", "CONT-003", 1, 500.0, true}, // Depósito
    {"A1B2C3D4", "CONT-004", 2, 500.0, true}, // Retiro (requiere remoción)
    {"E5F6G7H8", "CONT-005", 1, 500.0, true}, // Depósito
    {"I9J0K1L2", "CONT-006", 2, 500.0, true}  // Retiro
};

// Cola determinista de trabajos para la grúa compartida
TrabajoGrua colaGrua[10];
int trabajosPendientes = 0;

#endif