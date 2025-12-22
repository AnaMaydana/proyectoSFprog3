#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;
// FECHA

// *** ATRIBUTOS ***
struct Fecha {
    int dia;
    int mes;
    int anio;
};

// *** FUNCIONES AUXILIARES ***
bool leerFecha(Fecha& f) {
    char s1, s2;
    cin >> f.dia >> s1 >> f.mes >> s2 >> f.anio;
    return (s1 == '/' && s2 == '/' &&
        f.dia >= 1 && f.dia <= 31 &&
        f.mes >= 1 && f.mes <= 12 &&
        f.anio >= 2025);
}

// *** FUNCIONES AUXILIARES ***
bool fechaMayor(const Fecha& e, const Fecha& s) {
    if (e.anio != s.anio) return e.anio > s.anio;
    if (e.mes != s.mes)   return e.mes > s.mes;
    return e.dia > s.dia;
}
// CLIENTE

class Cliente {
private:
    // *** ATRIBUTOS ***
    int id;
    string nombre;
    string telefono;

public:
    // *** CONSTRUCTORES ***
    Cliente(int i, const string& n, const string& t)
        : id(i), nombre(n), telefono(t) {
    }

    // *** GETTERS SETTERS ***
    int getId() const { return id; }// ENCAPSULAMIENTO 
    string getNombre() const { return nombre; }
    string getTelefono() const { return telefono; }

    // *** FUNCIONES AUXILIARES ***
    void mostrar() const {
        cout << "ID: " << id
            << " | Nombre: " << nombre
            << " | Tel: " << telefono
            << endl;
    }
};
// HABITACION
class Habitacion {
private:
    // *** ATRIBUTOS ***
    int numero;
    string tipo;
    double precio;
    bool disponible;

public:
    // *** CONSTRUCTORES ***
    Habitacion(int n, const string& t, double p)
        : numero(n), tipo(t), precio(p), disponible(true) {
    }

    // *** GETTERS SETTERS ***
    int getNumero() const { return numero; }// ENCAPSULAMIENTO 
    string getTipo() const { return tipo; }
    double getPrecio() const { return precio; }
    bool estaDisponible() const { return disponible; }
    void setDisponible(bool e) { disponible = e; }

    // *** FUNCIONES AUXILIARES ***
    void mostrar() const {
        cout << "Hab " << numero
            << " | Tipo: " << tipo
            << " | Bs" << precio
            << " | Estado: " << (disponible ? "Libre" : "Ocupada")
            << endl;
    }
};

// RESERVA
class Reserva {
private:
    // *** ATRIBUTOS ***
    int idCliente;
    int numHab;
    Fecha entrada;
    Fecha salida;

public:
    // *** CONSTRUCTORES ***
    Reserva(int c, int h, Fecha en, Fecha sa)
        : idCliente(c), numHab(h), entrada(en), salida(sa) {
    }

    // *** GETTERS SETTERS ***
    int getHab() const { return numHab; }// ENCAPSULAMIENTO
    int getIdCliente() const { return idCliente; }

    // *** FUNCIONES AUXILIARES ***
    void mostrar() const {
        cout << "Cliente: " << idCliente
            << " | Hab: " << numHab
            << " | Entrada: " << entrada.dia << "/"
            << entrada.mes << "/" << entrada.anio
            << " | Salida: " << salida.dia << "/"
            << salida.mes << "/" << salida.anio
            << endl;
    }
};

class Hotel {
private:
    // punteros
    vector<Habitacion*> habitaciones;
    vector<Cliente*> clientes;
    vector<Reserva*> reservas;

public:
    // *** CONSTRUCTORES ***
    Hotel() { cargarHabitacionesIniciales(); }

    // *** DESTRUCTORES ***
    ~Hotel() {
        for (auto x : habitaciones) delete x;
        for (auto x : clientes) delete x;
        for (auto x : reservas) delete x;
    }

    // *** FUNCIONES AUXILIARES ***
    Habitacion* buscarHab(int n) {
        for (auto h : habitaciones)
            if (h->getNumero() == n) return h;
        return nullptr;
    }

    // *** FUNCIONES AUXILIARES ***
    Cliente* buscarCliente(int id) {
        for (auto c : clientes)
            if (c->getId() == id) return c;
        return nullptr;
    }
    // CONSTRUCTORES y ASIGNACIÓN DINÁMICA DE MEMORIA
    void cargarHabitacionesIniciales() {
        habitaciones.push_back(new Habitacion(101, "Simple", 150));
        habitaciones.push_back(new Habitacion(102, "Simple", 150));
        habitaciones.push_back(new Habitacion(201, "Doble", 300));
        habitaciones.push_back(new Habitacion(202, "Doble", 300));
        habitaciones.push_back(new Habitacion(301, "Suite", 450));
        habitaciones.push_back(new Habitacion(302, "Suite", 450));
        cargarHabitaciones();
        cargarClientes();
        cargarReservas();
    }
    // *** FUNCIONES AUXILIARES ***
    void mostrarHabitaciones() {
        for (auto x : habitaciones) x->mostrar();
    }
    void mostrarClientes() {
        for (auto x : clientes) x->mostrar();
    }
    void mostrarReservas() {
        for (auto x : reservas) x->mostrar();
    } 
    void registrarCliente() {
        string nom, tel;
        int id;

        cin.ignore();
        cout << "Nombre: "; getline(cin, nom);
        cout << "Telefono: "; getline(cin, tel);
        cout << "ID: "; cin >> id;

        if (buscarCliente(id)) {
            cout << "ERROR ID existe\n";
            return;
        }
        Cliente* c = new Cliente(id, nom, tel);
        clientes.push_back(c);

        ofstream arch("clientes.txt", ios::app);//  ARCHIVO
        arch << id << "," << nom << "," << tel << "\n";//  ARCHIVO
        arch.close();

        cout << "Cliente registrado.\n";
    }
    void reservar() {
        int id, num;
        Fecha fe, fs;

        cout << "ID cliente: "; cin >> id;
        Cliente* c = buscarCliente(id);

        if (!c) { cout << "No existe\n"; return; }

        mostrarHabitaciones();

        cout << "Num Hab: "; cin >> num;
        Habitacion* h = buscarHab(num);

        if (!h || !h->estaDisponible()) {
            cout << "No disponible\n";
            return;
        }

        cout << "Entrada dd/mm/aaaa: ";
        if (!leerFecha(fe)) return;

        cout << "Salida dd/mm/aaaa: ";
        if (!leerFecha(fs)) return;

        if (fechaMayor(fe, fs)) {
            cout << "Error fechas\n";
            return;
        }

        Reserva* r = new Reserva(id, num, fe, fs);
        reservas.push_back(r);
        h->setDisponible(false);

        guardarHabitaciones();

        ofstream arch("reservas.txt", ios::app);//  ARCHIVO
        arch << id << "," << num << ","
            << fe.dia << "/" << fe.mes << "/" << fe.anio << ","
            << fs.dia << "/" << fs.mes << "/" << fs.anio << "\n";//  ARCHIVO
        arch.close();

        cout << "Reserva OK\n";
    }

    void cancelar() {
        int num;
        cout << "Num hab cancelar: ";
        cin >> num;

        auto it = find_if(reservas.begin(), reservas.end(),
            [num](Reserva* r) { return r->getHab() == num; });

        if (it == reservas.end()) {
            cout << "NO existe\n"; return;
        }

        buscarHab(num)->setDisponible(true);
        delete* it;// manejo dinamico de memoria
        reservas.erase(it);
        guardarHabitaciones();

        cout << "Cancelada.\n";
    }

    void cargarClientes() {
        ifstream arch("clientes.txt");//  ARCHIVO
        if (!arch.is_open()) return;

        string linea;
        while (getline(arch, linea)) {
            string nom, tel;
            int id;

            stringstream ss(linea);

            ss >> id;
            ss.ignore();
            getline(ss, nom, ',');
            getline(ss, tel);

            clientes.push_back(new Cliente(id, nom, tel));// ASIGNACIÓN DINÁMICA DE MEMORIA
        }
    }
    void guardarHabitaciones() {
        ofstream arch("habitaciones.txt");//  ARCHIVO
        for (auto h : habitaciones) {
            arch << h->getNumero() << "," << h->estaDisponible() << "\n";
        }
    }
    void cargarHabitaciones() {
        ifstream arch("habitaciones.txt");//  ARCHIVO
        if (!arch.is_open()) return;

        int n, d;
        char c;

        while (arch >> n >> c >> d) {
            Habitacion* h = buscarHab(n);
            if (h) h->setDisponible(d);
        }
    }
    void cargarReservas() {
        ifstream arch("reservas.txt");//  ARCHIVO
        if (!arch.is_open()) return;

        string linea;
        while (getline(arch, linea)) {

            stringstream ss(linea);

            int id, num;
            Fecha fe, fs;
            char c1, c2, c3, c4;

            ss >> id >> c1 >> num >> c2
                >> fe.dia >> c3 >> fe.mes >> c4 >> fe.anio
                >> c1
                >> fs.dia >> c3 >> fs.mes >> c4 >> fs.anio;

            reservas.push_back(new Reserva(id, num, fe, fs));// ASIGNACIÓN DINÁMICA DE MEMORIA
            buscarHab(num)->setDisponible(false);
        }
    }
};
// INTERFFAZ
int main() {
    Hotel h;

    int op;
    do {
        cout << "\n====================================\n";
        cout << "        SISTEMA \n";
        cout << "====================================\n";
        cout << "1. Registrar nuevo cliente\n";
        cout << "2. Ver lista de clientes\n";
        cout << "3. Ver habitaciones disponibles\n";
        cout << "4. Realizar una reserva\n";
        cout << "5. Cancelar una reserva\n";
        cout << "6. Ver reservas registradas\n";
        cout << "------------------------------------\n";
        cout << "0. Salir del sistema\n";
        cout << "====================================\n";
        cout << "Seleccione una opcion: ";
        cin >> op;
        system("cls"); //LIMPIA ANTES DE MOSTRAR RESULTADO


        switch (op) {
        case 1: h.registrarCliente(); break;
        case 2: h.mostrarClientes(); break;
        case 3: h.mostrarHabitaciones(); break;
        case 4: h.reservar(); break;
        case 5: h.cancelar(); break;
        case 6: h.mostrarReservas(); break;
        case 0: cout << "Datos guardados correctamente.\n"; break;
        default: cout << "Opcion invalida.\n";
        }
        system("pause"); // PARA QUE NO DESAPAREZCA RÁPIDO


    } while (op != 0);
}
