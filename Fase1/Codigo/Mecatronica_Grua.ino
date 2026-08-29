void inicializarMecatronica() {
    // Configuración de pines de motores X, Z y electroimán
}

void ejecutarFSM_Grua() {
    if (trabajosPendientes > 0) {
        TrabajoGrua trabajoActual = colaGrua[0];
        
        // Rutina de posicionamiento referenciado
        if (!posicionAlcanzada(trabajoActual.posicionDestino)) {
            generarPulsosMotor(EJE_X, ADELANTE);
            return; // Retorna para mantener concurrencia
        }
        
        // Validar inconsistencia física vs inventario lógico antes de actualizar
        if (leerAlturaFisica() != alturaLogicaInventario) {
            Serial.println("ERR: POSICIÓN FÍSICA INCONSISTENTE. BLOQUEANDO CELDA.");
            bloquearCelda(trabajoActual.posicionDestino);
            abortarTrabajoActual();
            return;
        }
        
        // Si el agarre es exitoso, actualizar inventario
        if (confirmarAgarreElectroiman()) {
            actualizarMatrizInventario();
            desplazarCola(); // Elimina el trabajo completado (FIFO)
        }
    }
}