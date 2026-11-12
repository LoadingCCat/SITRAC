# SITRAC - Sistema Inteligente de Transporte y Gestión de Rutas

**Asignatura:** Estructura de Datos
**Fecha:** 12 de Noviembre de 2026

Este repositorio contiene la primera versión ejecutable y modular del núcleo funcional de SITRAC, desarrollado completamente en C++ utilizando gestión manual de memoria y estructuras de datos propias (punteros y nodos), sin recurrir a la Standard Template Library (STL) como `std::list` o `std::queue`.

## ⚙️ Instrucciones de Compilación y Ejecución
El código está contenido en un único archivo principal para facilitar su revisión en el avance formativo.
1. Navegar a la carpeta `src/`.
2. Compilar el archivo principal utilizando un compilador estándar de C++ (MinGW/GCC) con el comando: 
   `g++ main.cpp -o sitrac`
3. Ejecutar el binario generado:
   `./sitrac`
*(Nota: También es totalmente compatible con la ejecución directa en IDEs como Visual Studio u OnlineGDB).*

---

## 🏛️ Arquitectura y Tabla de Estructuras

| Operación | Estructura Usada | Justificación Técnica |
| :--- | :--- | :--- |
| **Gestión de Pasajeros** | Lista Enlazada Simple propia | Se implementó con `NodoPasajero` y punteros manuales para permitir inserción y eliminación dinámica (O(N) por la validación de duplicados), cumpliendo la estricta restricción de no usar `std::list`. |
| **Solicitudes de Viaje** | Cola Enlazada (Frente/Final) | Se utilizó el modelo FIFO con `NodoViaje`. Usar punteros de frente y final permite encolar y atender en complejidad O(1), gestionando el subdesbordamiento sin depender de `std::queue`. |
| **Ciudades y Rutas** | Lista de Adyacencia manual | Se usó `NodoCiudad` conectado a una sub-lista `NodoRuta` para gestionar el grafo. Permite crear pesos (distancias) y desconectar ciudades eliminadas dinámicamente sin dejar referencias colgantes en la memoria. |
| **Validación de Menú** | `cin.fail()`, `cin.clear()`, `cin.ignore()` | Se implementó la función `leerEnteroSeguro()` que limpia el buffer si se ingresa texto (letras), evitando un desbordamiento del bucle infinito. |

---

## 📋 Estado de Funcionalidades (División del Trabajo)

### Funcionalidades Terminadas (Avance Sesión 1)
| Módulo / Funcionalidad | Estado | Responsable |
| :--- | :--- | :--- |
| Arquitectura del menú principal interactivo | Terminado | Julian |
| Validador de entradas (Prevención CP-06) | Terminado | Julian |
| Módulo Pasajeros (Inserción sin duplicados CP-01) | Terminado | Julian |
| Módulo Pasajeros (Eliminación segura CP-02) | Terminado | Julian |
| Módulo Cola de Viajes (Encolar y Atender CP-03) | Terminado | Julian |
| Módulo Grafo (Registro de ciudades, rutas y CP-04) | Terminado | Julian |
| Eliminación de nodos en grafo sin fugas (CP-05) | Terminado | Julian |

### Funcionalidades Pendientes (Próximas Sesiones)
| Módulo / Funcionalidad | Estado | Responsable Asignado |
| :--- | :--- | :--- |
| Menú: Historial de viajes | Pendiente | Julian |
| Menú: Búsquedas avanzadas | Pendiente | Julian |
| Menú: Reportes del sistema | Pendiente | Julian |
| Sistema de Guardar/Cargar Archivos (`.txt` o `.csv`) | Pendiente | Julian |
| Actualizar datos de un pasajero existente | Pendiente | Julian |

---

## ⚖️ Declaración de Integridad Académica y Uso de IA

Durante el desarrollo de esta sesión formativa, se utilizó inteligencia artificial estrictamente como copiloto técnico para los siguientes propósitos formales:
1. **Validación de buffer:** Estructurar la lógica robusta de la función `leerEnteroSeguro()` para manejar adecuadamente el vaciado del buffer (`cin.clear()` e `ignore()`) y cumplir con el **CP-06** sin usar la instrucción `break` (condición de diseño del equipo).
2. **Diseño de memoria manual:** Diseñar la arquitectura manual de punteros para la Cola FIFO y las Listas Enlazadas (simple y de adyacencia), asegurando el cumplimiento de la restricción de no utilizar `std::queue` ni `std::list`.

Toda la lógica fue revisada línea por línea, ejecutada paso a paso y adaptada por los autores para asegurar que el sistema maneje los casos de borde (CP-01, CP-02, CP-03, CP-04 y CP-05) utilizando estructuras iterativas consistentes, garantizando el aprendizaje de la gestión de punteros en C++.
