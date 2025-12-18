#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

//=================================================
// FECHA
//=================================================
struct Fecha {
    int dia;
    int mes;
    int anio;
};

bool leerFecha(Fecha &f) {
    char s1, s2;
    cin >> f.dia >> s1 >> f.mes >> s2 >> f.anio;
    return (s1 == '/' && s2 == '/' &&
            f.dia >= 1 && f.dia <= 31 &&
            f.mes >= 1 && f.mes <= 12 &&
            f.anio >= 2025);
}

bool fechaMayor(const Fecha& e, const Fecha& s) {
    if (e.anio != s.anio) return e.anio > s.anio;
    if (e.mes != s.mes)   return e.mes > s.mes;
    return e.dia > s.dia;
}

//=================================================
// CLIENTE 
//=================================================
class Cliente {
private:
    int id;
    string nombre;
    string telefono;
public:
    Cliente(int i, const string& n, const string& t)
    : id(i), nombre(n), telefono(t) {}

    int getId() const { return id; }
    string getNombre() const { return nombre; }
    string getTelefono() const { return telefono; }

    void mostrar() const {
        cout << "ID: " << id
        << " | Nombre: " << nombre
        << " | Tel: " << telefono
        << endl;
    }
};

//=================================================
// HABITACION
//=================================================
class Habitacion {
private:
    int numero;
    string tipo;
    double precio;
    bool disponible;

public:
    Habitacion(int n, const string& t, double p)
    : numero(n), tipo(t), precio(p), disponible(true) {}

    int getNumero() const { return numero; }
    string getTipo() const { return tipo; }
    double getPrecio() const { return precio; }
    bool estaDisponible() const { return disponible; }
    void setDisponible(bool e){ disponible = e;}

    void mostrar() const {
        cout << "Hab " << numero
        << " | Tipo: " << tipo
        << " | $" << precio
        << " | Estado: " << (disponible?"Libre":"Ocupada")
        << endl;
    }
};

//=================================================
// RESERVA
//=================================================
class Reserva {
private:
    int idCliente;
    int numHab;
    Fecha entrada;
    Fecha salida;

public:
    Reserva(int c, int h, Fecha en, Fecha sa)
    : idCliente(c), numHab(h), entrada(en), salida(sa) {}

    int getHab() const { return numHab; }
    int getIdCliente() const { return idCliente; }

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

//=================================================
// HOTEL
//=================================================
class Hotel {
private:
    vector<Habitacion*> habitaciones;
    vector<Cliente*> clientes;
    vector<Reserva*> reservas;

public:
    Hotel() { cargarHabitacionesIniciales(); }

    ~Hotel() {
        for(auto x: habitaciones) delete x;
        for(auto x: clientes) delete x;
        for(auto x: reservas) delete x;
    }

    Habitacion* buscarHab(int n){
        for(auto h: habitaciones)
            if(h->getNumero()==n) return h;
        return nullptr;
    }

    Cliente* buscarCliente(int id){
        for(auto c: clientes)
            if(c->getId()==id) return c;
        return nullptr;
    }

    //=================================================
    // CARGAS INICIALES (CONSTRUCTOR)
    //=================================================
    void cargarHabitacionesIniciales() {
        habitaciones.push_back(new Habitacion(101,"Simple",100));
        habitaciones.push_back(new Habitacion(102,"Simple",100));
        habitaciones.push_back(new Habitacion(201,"Doble",200));
        habitaciones.push_back(new Habitacion(202,"Doble",200));
        habitaciones.push_back(new Habitacion(301,"Suite",350));
        habitaciones.push_back(new Habitacion(302,"Suite",350));
        cargarHabitaciones();
        cargarClientes();
        cargarReservas();
    }

    //=================================================
    // MOSTRAR
    //=================================================
    void mostrarHabitaciones(){
        for(auto x: habitaciones) x->mostrar();
    }

    void mostrarClientes(){
        for(auto x: clientes) x->mostrar();
    }

    void mostrarReservas(){
        for(auto x: reservas) x->mostrar();
    }

    //=================================================
    // REGISTRO CLIENTE + GUARDADO
    //=================================================
    void registrarCliente() {
        string nom, tel;
        int id;

        cin.ignore();
        cout<<"Nombre: "; getline(cin, nom);
        cout<<"Telefono: "; getline(cin, tel);
        cout<<"ID: "; cin>>id;

        if(buscarCliente(id)){
            cout<<"ERROR ID existe\n";
            return;
        }

        Cliente* c = new Cliente(id,nom,tel);
        clientes.push_back(c);

        ofstream arch("clientes.txt", ios::app);
        arch<<id<<","<<nom<<","<<tel<<"\n";
        arch.close();

        cout<<"Cliente registrado.\n";
    }

    //=================================================
    // RESERVA + GUARDADO
    //=================================================
    void reservar() {
        int id, num;
        Fecha fe, fs;

        cout<<"ID cliente: "; cin>>id;
        Cliente* c = buscarCliente(id);

        if(!c){ cout<<"No existe\n"; return; }

        mostrarHabitaciones();

        cout<<"Num Hab: "; cin>>num;
        Habitacion* h = buscarHab(num);

        if(!h || !h->estaDisponible()){
            cout<<"No disponible\n";
            return;
        }

        cout<<"Entrada dd/mm/aaaa: ";
        if(!leerFecha(fe)) return;

        cout<<"Salida dd/mm/aaaa: ";
        if(!leerFecha(fs)) return;

        if(fechaMayor(fe,fs)){
            cout<<"Error fechas\n";
            return;
        }

        Reserva* r = new Reserva(id,num,fe,fs);
        reservas.push_back(r);
        h->setDisponible(false);

        guardarHabitaciones();

        ofstream arch("reservas.txt", ios::app);
        arch<<id<<","<<num<<","
        <<fe.dia<<"/"<<fe.mes<<"/"<<fe.anio<<","
        <<fs.dia<<"/"<<fs.mes<<"/"<<fs.anio<<"\n";
        arch.close();

        cout<<"Reserva OK\n";
    }

    //=================================================
    // CANCELAR
    //=================================================
    void cancelar() {
        int num;
        cout<<"Num hab cancelar: ";
        cin>>num;

        auto it = find_if(reservas.begin(), reservas.end(),
        [num](Reserva* r){ return r->getHab()==num; });

        if(it==reservas.end()){
            cout<<"NO existe\n"; return;
        }

        buscarHab(num)->setDisponible(true);
        delete *it;
        reservas.erase(it);
        guardarHabitaciones();

        cout<<"Cancelada.\n";
    }

    //=================================================
    // ARCHIVOS
    //=================================================
    void cargarClientes(){
        ifstream arch("clientes.txt");
        if(!arch.is_open()) return;

        string linea;
        while(getline(arch,linea)){
            string nom,tel;
            int id;

            stringstream ss(linea);

            ss>>id;
            ss.ignore();
            getline(ss,nom,',');
            getline(ss,tel);

            clientes.push_back(new Cliente(id,nom,tel));
        }
    }

    void guardarHabitaciones(){
        ofstream arch("habitaciones.txt");
        for(auto h: habitaciones){
            arch<<h->getNumero()<<","<<h->estaDisponible()<<"\n";
        }
    }

    void cargarHabitaciones(){
        ifstream arch("habitaciones.txt");
        if(!arch.is_open()) return;

        int n, d;
        char c;

        while(arch>>n>>c>>d){
            Habitacion* h = buscarHab(n);
            if(h) h->setDisponible(d);
        }
    }

    void cargarReservas(){
        ifstream arch("reservas.txt");
        if(!arch.is_open()) return;

        string linea;
        while(getline(arch,linea)){

            stringstream ss(linea);

            int id, num;
            Fecha fe,fs;
            char c1,c2,c3,c4;

            ss>>id>>c1>>num>>c2
            >>fe.dia>>c3>>fe.mes>>c4>>fe.anio
            >>c1
            >>fs.dia>>c3>>fs.mes>>c4>>fs.anio;

            reservas.push_back(new Reserva(id,num,fe,fs));
            buscarHab(num)->setDisponible(false);
        }
    }
};

//=================================================
// MAIN
//=================================================
int main() {
    Hotel h;
    
    int op;
    do{
        cout<<"\n--- HOTEL MENU ---\n";
        cout<<"1 Registrar Cliente\n";
        cout<<"2 Mostrar Clientes\n";
        cout<<"3 Mostrar Habitaciones\n";
        cout<<"4 Reservar\n";
        cout<<"5 Cancelar Reserva\n";
        cout<<"6 Mostrar Reservas\n";
        cout<<"0 Salir\nOpcion: ";
        cin>>op;

        switch(op){
            case 1: h.registrarCliente(); break;
            case 2: h.mostrarClientes(); break;
            case 3: h.mostrarHabitaciones(); break;
            case 4: h.reservar(); break;
            case 5: h.cancelar(); break;
            case 6: h.mostrarReservas(); break;
            case 0: cout<<"datos guardados \n"; break;
            default: cout<<"Error\n";
        }

    }while(op!=0);
}
