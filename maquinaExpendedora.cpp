#include <iostream>
#include <string>
#include <iomanip> // Para setw y left

using namespace std;

const int MAX_TAM = 100;

struct Producto {
    int posicionProducto;
    string nombre;
    int precio;
    int disponibilidad;
};

// Mostrar menús
void mostrarMenuPrincipal() {
    cout << "==== MAQUINA EXPENDEDORA ====" << endl;
    cout << "1. Eres el duenio de la maquina" << endl;
    cout << "2. Eres comprador" << endl;
    cout << "3. Salir" << endl;
}

void mostrarMenuDuenio() {
    cout << "==== MENU DUENIO ====" << endl;
    cout << "1. Reponer o actualizar productos" << endl;
    cout << "2. Recoger dinero" << endl;
    cout << "3. Salir" << endl;
}

void mostrarMenuComprador() {
    cout << "==== MENU COMPRADOR ====" << endl;
    cout << "1. Ver los productos" << endl;
    cout << "2. Insertar dinero" << endl;
    cout << "3. Comprar" << endl;
    cout << "4. Salir" << endl;
}

// Buscar producto por posición
int buscarProductoPorPosicion(const Producto productos[], int tam, int posicion) {
    for (int i = 0; i < tam; i++) {
        if (productos[i].posicionProducto == posicion) return i;
    }
    return -1; // No encontrado
}

// Reponer productos o añadir nuevos
void reponerProductos(Producto productos[], int &tam) {
    cout << "Cuantos productos quieres anadir o actualizar? ";
    int cantidad;
    cin >> cantidad;
    if (cantidad <= 0 || cantidad + tam > MAX_TAM) {
        cout << "Cantidad invalida o se excede el maximo permitido." << endl;
        return;
    }

    for (int i = 0; i < cantidad; i++) {
        int pos;
        cout << "Producto " << i + 1 << ":" << endl;
        cout << "Ingrese posicion unica del producto: ";
        cin >> pos;

        int j = buscarProductoPorPosicion(productos, tam, pos);
        if (j == -1) {
            // Nuevo producto
            j = tam;
            tam++;
        } else {
            cout << "Producto existente, se actualizaran sus datos." << endl;
        }

        productos[j].posicionProducto = pos;
        cout << "Ingrese nombre: ";
        cin.ignore(); // limpiar buffer
        getline(cin, productos[j].nombre);
        cout << "Ingrese precio: ";
        cin >> productos[j].precio;
        cout << "Ingrese disponibilidad: ";
        cin >> productos[j].disponibilidad;
    }
}

// Mostrar productos disponibles (mejorado visualmente)
void verProductos(const Producto productos[], int tam) {
    if (tam == 0) {
        cout << "No hay productos disponibles." << endl;
        return;
    }

    cout << "\n=== LISTA DE PRODUCTOS ===" << endl;
    cout << left << setw(6) << "Pos"
         << setw(22) << "Nombre"
         << setw(10) << "Precio"
         << setw(15) << "Disponibilidad" << endl;
    cout << "----------------------------------------------------------" << endl;

    for (int i = 0; i < tam; i++) {
        cout << left << setw(6) << productos[i].posicionProducto
             << setw(22) << productos[i].nombre
             << setw(10) << productos[i].precio
             << setw(15) << productos[i].disponibilidad << endl;
    }
}

// Insertar dinero (comprador)
void insertarDinero(int &saldoComprador) {
    int dinero;
    cout << "Cuanto dinero deseas insertar? ";
    cin >> dinero;
    if (dinero > 0) {
        saldoComprador += dinero;
        cout << "Dinero insertado. Saldo actual del comprador: " << saldoComprador << endl;
    } else {
        cout << "Cantidad invalida." << endl;
    }
}

// Recoger dinero (dueño)
void recogerDinero(int &saldoCaja) {
    cout << "Saldo en la caja: " << saldoCaja << endl;
    int cantidad;
    do {
        cout << "Cuanto dinero quieres recoger? ";
        cin >> cantidad;
        if (cantidad > saldoCaja || cantidad < 0) {
            cout << "Cantidad no valida. Intenta de nuevo." << endl;
        }
    } while (cantidad > saldoCaja || cantidad < 0);

    saldoCaja -= cantidad;
    cout << "Has recogido " << cantidad << ". Saldo restante en la caja: " << saldoCaja << endl;
}

// Comprar productos
void comprarProductos(Producto productos[], int tam, int &saldoComprador, int &saldoCaja) {
    if (tam == 0) {
        cout << "No hay productos para comprar." << endl;
        return;
    }
    if (saldoComprador <= 0) {
        cout << "No tienes saldo para comprar. Inserta dinero primero." << endl;
        return;
    }

    while (true) {
        verProductos(productos, tam);

        int posCompra;
        cout << "Introduce la posicion del producto que quieres comprar (0 para salir): ";
        cin >> posCompra;

        if (posCompra == 0) {
            cout << "Terminando compra. Devolviendo saldo restante al comprador: " << saldoComprador << endl;
            saldoComprador = 0;
            break;
        }

        int j = buscarProductoPorPosicion(productos, tam, posCompra);
        if (j == -1) {
            cout << "Producto no encontrado." << endl;
            continue;
        }

        Producto &prod = productos[j];

        if (prod.disponibilidad <= 0) {
            cout << "Producto agotado." << endl;
            continue;
        }

        if (saldoComprador < prod.precio) {
            cout << "Saldo insuficiente para comprar este producto." << endl;
            cout << "Ingresa más dinero." << endl;
            insertarDinero(saldoComprador);
            continue;
        }

        // Compra exitosa:
        prod.disponibilidad--;
        saldoComprador -= prod.precio;
        saldoCaja += prod.precio;
        cout << "Has comprado " << prod.nombre << ". Saldo restante del comprador: " << saldoComprador << endl;

        char seguir;
        cout << "Quieres comprar otro producto? (s/n): ";
        cin >> seguir;
        if (seguir != 's' && seguir != 'S') {
            cout << "Compra finalizada. Devolviendo saldo restante al comprador: " << saldoComprador << endl;
            saldoComprador = 0;
            break;
        }
    }
}

// Función para dueño
void menuDuenio(Producto productos[], int &tam, int &saldoCaja) {
    int opcion;
    do {
        mostrarMenuDuenio();
        cout << "Ingrese opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1:
                reponerProductos(productos, tam);
                break;
            case 2:
                recogerDinero(saldoCaja);
                break;
            case 3:
                cout << "Saliendo del menu duenio." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 3);
}

// Función para comprador
void menuComprador(Producto productos[], int tam, int &saldoComprador, int &saldoCaja) {
    int opcion;
    do {
        mostrarMenuComprador();
        cout << "Ingrese opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1:
                verProductos(productos, tam);
                break;
            case 2:
                insertarDinero(saldoComprador);
                break;
            case 3:
                comprarProductos(productos, tam, saldoComprador, saldoCaja);
                break;
            case 4:
                cout << "Saliendo del menu comprador." << endl;
                if (saldoComprador > 0) {
                    cout << "Devolviendo saldo restante al comprador: " << saldoComprador << endl;
                    saldoComprador = 0;
                }
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 4);
}

int main() {
    Producto productos[MAX_TAM];
    int tam = 0;
    int saldoCaja = 0;      // Dinero para dueño
    int saldoComprador = 0; // Dinero que ha insertado comprador (crédito)

    int opcion;
    do {
        mostrarMenuPrincipal();
        cout << "Ingrese opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                menuDuenio(productos, tam, saldoCaja);
                break;
            case 2:
                menuComprador(productos, tam, saldoComprador, saldoCaja);
                break;
            case 3:
                cout << "Gracias por usar la maquina expendedora. Hasta luego!" << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 3);

    return 0;
}
