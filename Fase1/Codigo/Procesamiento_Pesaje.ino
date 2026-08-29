const int NUM_MUESTRAS = 15;
long bufferMuestras[NUM_MUESTRAS];
volatile int indiceMuestra = 0;
volatile bool pesajeListo = false;

void ISR_ProcesarPesaje() {
    // Lectura cruda simultánea de los pines de datos (24, 26, 28, 30)
    // omitida por brevedad. Se inserta el valor total en el buffer:
    
    long pesoActual = leerBalanzaHardware(); 
    
    // Detección de meseta de 1.4 segundos
    if (pesoActual > 5000) { 
        bufferMuestras[indiceMuestra] = pesoActual;
        indiceMuestra++;
        if (indiceMuestra >= NUM_MUESTRAS) {
            pesajeListo = true;
            indiceMuestra = 0;
        }
    }
}

void evaluarToleranciaPesaje() {
    if (pesajeListo) {
        // Ordenamiento burbuja y extracción de mediana
        long medianaPeso = calcularMediana(bufferMuestras, NUM_MUESTRAS);
        
        // Comparación con manifiesto
        if (medianaPeso > toleranciaMaxima) {
            // Accionar aguja desviadora hacia el ramal
            moverAguja(RAMAL_RETENIDOS);
        } else {
            moverAguja(RUTA_NORMAL);
        }
        pesajeListo = false;
    }
}