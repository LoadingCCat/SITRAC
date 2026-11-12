#include <iostream>
#include <string>
#include <limits>

using namespace std;

// ==========================================
// 1. ESTRUCTURAS PROPIAS (Nodos 100% Manuales)
// ==========================================

// --- MÓDULO PASAJEROS ---
struct NodoPasajero {
    int id;
    string nombre;
    NodoPasajero* sig;
};

// --- MÓDULO COLA ---
struct NodoViaje {
    string origen;
    string destino;
    NodoViaje* sig;
};

struct ColaViajes {
    NodoViaje* frente;
    NodoViaje* final;
};

// --- MÓDULO GRAFO (Ciudades y Rutas) ---
struct NodoRuta {
    string destino;
    int distancia;
    NodoRuta* sig;
};

struct NodoCiudad {
    string nombre;
    NodoRuta* rutas; // Sub-lista de rutas (adyacencias)
    NodoCiudad* sig; 
};

// ==========================================
// 2. VALIDADOR DE ENTRADAS (CP-06)
// ==========================================
int leerEnteroSeguro() {
    int valor;
    cin >> valor;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1; // CP-06: Recupera el menú sin ciclo infinito
    }
    return valor;
}

// ==========================================
// 3. FUNCIONES DE PASAJEROS
// ==========================================
void insertarPasajero(NodoPasajero*& cabeza, int id, string nombre) {
    NodoPasajero* actual = cabeza;
    bool duplicado = false;
    while (actual != nullptr && !duplicado) {
        if (actual->id == id) {
            duplicado = true;
            cout << "-> ERROR: El ID " << id << " ya esta registrado a nombre de '" << actual->nombre << "'.\n";
        }
        actual = actual->sig;
    }

    if (!duplicado) {
        NodoPasajero* nuevo = new NodoPasajero;
        nuevo->id = id;
        nuevo->nombre = nombre;
        nuevo->sig = cabeza;
        cabeza = nuevo;
        cout << "-> Pasajero registrado exitosamente.\n";
    }
}

void eliminarPasajero(NodoPasajero*& cabeza, int idBusqueda) {
    if (cabeza == nullptr) {
        cout << "-> ERROR [CP-02]: La lista esta vacia. No se encontro el ID " << idBusqueda << ".\n";
        return;
    }

    if (cabeza->id == idBusqueda) {
        NodoPasajero* temp = cabeza;
        cabeza = cabeza->sig;
        delete temp;
        cout << "-> Pasajero " << idBusqueda << " eliminado correctamente.\n";
    } 
    else {
        NodoPasajero* actual = cabeza;
        bool encontrado = false;
        
        while (actual->sig != nullptr && !encontrado) {
            if (actual->sig->id == idBusqueda) {
                NodoPasajero* temp = actual->sig;
                actual->sig = temp->sig;
                delete temp;
                encontrado = true;
                cout << "-> Pasajero " << idBusqueda << " eliminado correctamente.\n";
            } else {
                actual = actual->sig;
            }
        }
        if (!encontrado) {
            cout << "-> ERROR [CP-02]: El pasajero con ID " << idBusqueda << " NO existe en el sistema (No rompe la lista).\n";
        }
    }
}

// ==========================================
// 4. FUNCIONES DE COLA
// ==========================================
void encolarViaje(ColaViajes& cola, string origen, string destino) {
    NodoViaje* nuevo = new NodoViaje;
    nuevo->origen = origen;
    nuevo->destino = destino;
    nuevo->sig = nullptr;

    if (cola.frente == nullptr) {
        cola.frente = nuevo;
        cola.final = nuevo;
    } else {
        cola.final->sig = nuevo;
        cola.final = nuevo;
    }
    cout << "-> Solicitud de viaje encolada (" << origen << " -> " << destino << ").\n";
}

void atenderViaje(ColaViajes& cola) {
    if (cola.frente == nullptr) {
        cout << "-> ERROR [CP-03]: Subdesbordamiento. La cola de viajes esta vacia.\n";
    } else {
        NodoViaje* temp = cola.frente;
        cout << "-> ATENDIENDO VIAJE: " << temp->origen << " -> " << temp->destino << "\n";
        
        cola.frente = cola.frente->sig;
        delete temp;
        
        if (cola.frente == nullptr) cola.final = nullptr;
    }
}

// ==========================================
// 5. FUNCIONES DE GRAFO (CIUDADES Y RUTAS)
// ==========================================
void registrarCiudad(NodoCiudad*& listaCiudades, string nombre) {
    NodoCiudad* actual = listaCiudades;
    bool existe = false;
    
    while (actual != nullptr && !existe) {
        if (actual->nombre == nombre) existe = true;
        actual = actual->sig;
    }

    if (existe) {
        cout << "-> ERROR [CP-01]: La ciudad '" << nombre << "' ya esta registrada.\n";
    } else {
        NodoCiudad* nuevo = new NodoCiudad;
        nuevo->nombre = nombre;
        nuevo->rutas = nullptr; // Inicializamos su lista de adyacencia vacía
        nuevo->sig = listaCiudades;
        listaCiudades = nuevo;
        cout << "-> Ciudad '" << nombre << "' registrada exitosamente.\n";
    }
}

void agregarRuta(NodoCiudad* grafo, string origen, string destino, int distancia) {
    // Validación CP-04: Peso inválido
    if (distancia < 0) {
        cout << "-> ERROR [CP-04]: Distancia negativa (" << distancia << ") rechazada. Peso invalido.\n";
        return;
    }

    // Buscamos la ciudad de origen
    NodoCiudad* actual = grafo;
    bool origenEncontrado = false;
    
    while (actual != nullptr && !origenEncontrado) {
        if (actual->nombre == origen) {
            origenEncontrado = true;
            // Insertamos la ruta en la sub-lista de la ciudad origen
            NodoRuta* nuevaRuta = new NodoRuta;
            nuevaRuta->destino = destino;
            nuevaRuta->distancia = distancia;
            nuevaRuta->sig = actual->rutas;
            actual->rutas = nuevaRuta;
            cout << "-> Ruta creada: " << origen << " -> " << destino << " (" << distancia << " km).\n";
        }
        actual = actual->sig;
    }
    
    if (!origenEncontrado) {
        cout << "-> ERROR: La ciudad de origen no existe.\n";
    }
}

// Función auxiliar para CP-05: Limpiar las referencias hacia una ciudad eliminada
void eliminarRutasHacia(NodoCiudad* ciudad, string destinoBorrar) {
    if (ciudad->rutas == nullptr) return;

    // Si la referencia a borrar está en la cabeza de la sub-lista
    while (ciudad->rutas != nullptr && ciudad->rutas->destino == destinoBorrar) {
        NodoRuta* temp = ciudad->rutas;
        ciudad->rutas = ciudad->rutas->sig;
        delete temp;
    }

    // Si la referencia está más adelante en la sub-lista
    NodoRuta* actual = ciudad->rutas;
    while (actual != nullptr && actual->sig != nullptr) {
        if (actual->sig->destino == destinoBorrar) {
            NodoRuta* temp = actual->sig;
            actual->sig = temp->sig;
            delete temp;
        } else {
            actual = actual->sig;
        }
    }
}

void eliminarCiudad(NodoCiudad*& grafo, string nombre) {
    if (grafo == nullptr) {
        cout << "-> ERROR: El grafo esta vacio.\n";
        return;
    }

    // PASO 1 (CP-05): Recorrer TODO el grafo y borrar las rutas que apuntaban a esta ciudad
    NodoCiudad* actual = grafo;
    while (actual != nullptr) {
        if (actual->nombre != nombre) {
            eliminarRutasHacia(actual, nombre);
        }
        actual = actual->sig;
    }

    // PASO 2: Eliminar la ciudad y liberar sus propias rutas
    if (grafo->nombre == nombre) {
        NodoCiudad* tempCiudad = grafo;
        grafo = grafo->sig;
        
        // Destruimos sus rutas (adyacencias)
        while (tempCiudad->rutas != nullptr) {
            NodoRuta* tempRuta = tempCiudad->rutas;
            tempCiudad->rutas = tempCiudad->rutas->sig;
            delete tempRuta;
        }
        delete tempCiudad;
        cout << "-> [CP-05] EXITO: Ciudad '" << nombre << "' eliminada sin dejar referencias colgantes.\n";
    } 
    else {
        actual = grafo;
        bool encontrado = false;
        while (actual->sig != nullptr && !encontrado) {
            if (actual->sig->nombre == nombre) {
                NodoCiudad* tempCiudad = actual->sig;
                actual->sig = tempCiudad->sig;
                
                while (tempCiudad->rutas != nullptr) {
                    NodoRuta* tempRuta = tempCiudad->rutas;
                    tempCiudad->rutas = tempCiudad->rutas->sig;
                    delete tempRuta;
                }
                delete tempCiudad;
                encontrado = true;
                cout << "-> [CP-05] EXITO: Ciudad '" << nombre << "' eliminada sin dejar referencias colgantes.\n";
            } else {
                actual = actual->sig;
            }
        }
        if (!encontrado) {
            cout << "-> ERROR: La ciudad no existe.\n";
        }
    }
}

// ==========================================
// 6. MENÚ PRINCIPAL
// ==========================================
int main() {
    NodoPasajero* listaPasajeros = nullptr;
    ColaViajes colaSolicitudes = {nullptr, nullptr};
    NodoCiudad* grafoCiudades = nullptr;
    
    int opcionPrincipal = -1;

    cout << "INICIANDO SISTEMA SITRAC...\n";
    
    // --- PRECARGA DE DATOS PARA FACILITAR PRUEBAS ---
    cout << "Precargando datos base...\n";
    registrarCiudad(grafoCiudades, "Medellin");
    registrarCiudad(grafoCiudades, "Bogota");
    registrarCiudad(grafoCiudades, "Villavicencio");
    agregarRuta(grafoCiudades, "Bogota", "Villavicencio", 120);
    agregarRuta(grafoCiudades, "Bogota", "Medellin", 400);
    insertarPasajero(listaPasajeros, 1, "Julian");
    // ------------------------------------------------

    while (opcionPrincipal != 0) {
        cout << "\n====================================\n";
        cout << "    MENU PRINCIPAL - SITRAC 2026    \n";
        cout << "====================================\n";
        cout << "1. Ciudades\n";
        cout << "2. Rutas\n";
        cout << "3. Pasajeros\n";
        cout << "4. Cola de Viajes\n";
        cout << "0. Salir\n";
        cout << "Seleccione: ";
        
        opcionPrincipal = leerEnteroSeguro();

        if (opcionPrincipal == 1) { // CIUDADES
            int opcCiudad = -1;
            while (opcCiudad != 0) {
                cout << "\n--- SUBMENU CIUDADES ---\n";
                cout << "1. Registrar Ciudad (Prueba CP-01)\n";
                cout << "2. Eliminar Ciudad Conectada (Prueba CP-05)\n";
                cout << "0. Volver\nOpcion: ";
                opcCiudad = leerEnteroSeguro();
                
                if (opcCiudad == 1) {
                    string nomCiudad;
                    cout << "Nombre de la ciudad: ";
                    cin >> ws; getline(cin, nomCiudad);
                    registrarCiudad(grafoCiudades, nomCiudad);
                } 
                else if (opcCiudad == 2) {
                    string nomCiudad;
                    cout << "Ciudad a eliminar: ";
                    cin >> ws; getline(cin, nomCiudad);
                    eliminarCiudad(grafoCiudades, nomCiudad);
                }
                else if (opcCiudad != 0) cout << "Opcion invalida.\n";
            }
        } 
        else if (opcionPrincipal == 2) { // RUTAS
            int opcRuta = -1;
            while (opcRuta != 0) {
                cout << "\n--- SUBMENU RUTAS ---\n";
                cout << "1. Crear Ruta (Prueba CP-04)\n0. Volver\nOpcion: ";
                opcRuta = leerEnteroSeguro();
                
                if (opcRuta == 1) {
                    string ori, des; int dist;
                    cout << "Origen: "; cin >> ws; getline(cin, ori);
                    cout << "Destino: "; cin >> ws; getline(cin, des);
                    cout << "Distancia en km: "; dist = leerEnteroSeguro();
                    agregarRuta(grafoCiudades, ori, des, dist);
                } else if (opcRuta != 0) cout << "Opcion invalida.\n";
            }
        }
        else if (opcionPrincipal == 3) { // PASAJEROS
            int opcPasajero = -1;
            while (opcPasajero != 0) {
                cout << "\n--- SUBMENU PASAJEROS ---\n";
                cout << "1. Insertar\n2. Eliminar (Prueba CP-02)\n0. Volver\nOpcion: ";
                opcPasajero = leerEnteroSeguro();
                
                if (opcPasajero == 1) {
                    int id; string nom;
                    cout << "ID numérico: "; id = leerEnteroSeguro();
                    cout << "Nombre: "; cin >> ws; getline(cin, nom);
                    insertarPasajero(listaPasajeros, id, nom);
                } 
                else if (opcPasajero == 2) {
                    int idBusqueda;
                    cout << "ID a eliminar: "; idBusqueda = leerEnteroSeguro();
                    eliminarPasajero(listaPasajeros, idBusqueda);
                } 
                else if (opcPasajero != 0) cout << "Opcion invalida.\n";
            }
        }
        else if (opcionPrincipal == 4) { // COLA
            int opcCola = -1;
            while (opcCola != 0) {
                cout << "\n--- SUBMENU COLA VIAJES ---\n";
                cout << "1. Encolar Viaje\n2. Atender Siguiente (Prueba CP-03)\n0. Volver\nOpcion: ";
                opcCola = leerEnteroSeguro();
                
                if (opcCola == 1) {
                    string ori, des;
                    cout << "Origen: "; cin >> ws; getline(cin, ori);
                    cout << "Destino: "; cin >> ws; getline(cin, des);
                    encolarViaje(colaSolicitudes, ori, des);
                } 
                else if (opcCola == 2) {
                    atenderViaje(colaSolicitudes);
                } 
                else if (opcCola != 0) cout << "Opcion invalida.\n";
            }
        }
        else if (opcionPrincipal == 0) cout << "Cerrando SITRAC...\n";
        else cout << "-> ERROR: Entrada invalida. Por favor digite un numero del menu.\n";
    }
    return 0;
}
