#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// =======================================================
// CLASE LIBRO
// =======================================================
class Libro {
private:
    std::string titulo;
    std::string autor;
    std::string isbn;
    bool disponible;

public:
    Libro(const std::string& t, const std::string& a, const std::string& i)
        : titulo(t), autor(a), isbn(i), disponible(true) {}

    std::string getTitulo() const { return titulo; }
    std::string getAutor() const { return autor; }
    std::string getIsbn() const { return isbn; }
    bool estaDisponible() const { return disponible; }

    void setDisponible(bool estado) { disponible = estado; }

    void mostrar() const {
        std::cout << "ISBN: " << isbn
                  << " | Titulo: " << titulo
                  << " | Autor: " << autor
                  << " | Estado: "
                  << (disponible ? "Disponible" : "Prestado") << std::endl;
    }
};

// =======================================================
// CLASE SOCIO
// =======================================================
class Socio {
private:
    std::string nombre;
    int id;
    std::vector<std::string> librosPrestados;

public:
    Socio(const std::string& n, int i) : nombre(n), id(i) {}

    int getId() const { return id; }
    std::string getNombre() const { return nombre; }

    void prestarLibro(const std::string& isbn) {
        librosPrestados.push_back(isbn);
    }

    bool tieneLibro(const std::string& isbn) const {
        return std::find(librosPrestados.begin(),
                         librosPrestados.end(),
                         isbn) != librosPrestados.end();
    }

    void devolverLibro(const std::string& isbn) {
        auto it = std::find(librosPrestados.begin(),
                            librosPrestados.end(),
                            isbn);
        if (it != librosPrestados.end())
            librosPrestados.erase(it);
    }

    void mostrar() const {
        std::cout << "ID: " << id
                  << " | Nombre: " << nombre
                  << " | Libros: ";
        if (librosPrestados.empty())
            std::cout << "Ninguno";
        else
            for (const auto& l : librosPrestados)
                std::cout << l << " ";
        std::cout << std::endl;
    }
};

// =======================================================
// CLASE BIBLIOTECA
// =======================================================
class Biblioteca {
private:
    // ******* MANEJO DINÁMICO DE MEMORIA *******
    // Vectores que almacenan PUNTEROS a objetos creados dinámicamente
    std::vector<Libro*> catalogo;
    std::vector<Socio*> socios;

    // ******* USO DE PUNTEROS *******
    Libro* buscarLibro(const std::string& isbn) {
        for (Libro* l : catalogo) {
            if (l->getIsbn() == isbn)
                return l;
        }
        return nullptr;
    }

    // ******* USO DE PUNTEROS *******
    Socio* buscarSocio(int id) {
        for (Socio* s : socios) {
            if (s->getId() == id)
                return s;
        }
        return nullptr;
    }

public:
    // ******* LIBERACIÓN DE MEMORIA DINÁMICA *******
    // Destructor que libera la memoria reservada con new
    ~Biblioteca() {
        for (Libro* l : catalogo)
            delete l;

        for (Socio* s : socios)
            delete s;
    }

    void agregarLibro() {
        std::string t, a, i;
        std::cin.ignore();
        std::cout << "Titulo: "; getline(std::cin, t);
        std::cout << "Autor: "; getline(std::cin, a);
        std::cout << "ISBN: "; getline(std::cin, i);

        if (buscarLibro(i)) {
            std::cout << "ERROR: ISBN duplicado\n";
            return;
        }

        // ******* ASIGNACIÓN DINÁMICA DE MEMORIA *******
        Libro* nuevoLibro = new Libro(t, a, i);
        catalogo.push_back(nuevoLibro);

        std::cout << "Libro agregado correctamente\n";
    }

    void registrarSocio() {
        std::string n;
        int id;
        std::cin.ignore();
        std::cout << "Nombre: "; getline(std::cin, n);
        std::cout << "ID: "; std::cin >> id;

        if (buscarSocio(id)) {
            std::cout << "ERROR: ID duplicado\n";
            return;
        }

        // ******* ASIGNACIÓN DINÁMICA DE MEMORIA *******
        Socio* nuevoSocio = new Socio(n, id);
        socios.push_back(nuevoSocio);

        std::cout << "Socio registrado correctamente\n";
    }

    void prestarLibro() {
        std::string isbn;
        int id;
        std::cin.ignore();
        std::cout << "ISBN: "; getline(std::cin, isbn);
        std::cout << "ID Socio: "; std::cin >> id;

        Libro* l = buscarLibro(isbn);
        Socio* s = buscarSocio(id);

        if (!l || !s) {
            std::cout << "ERROR: Libro o socio no existe\n";
            return;
        }

        if (!l->estaDisponible()) {
            std::cout << "Libro no disponible\n";
            return;
        }

        l->setDisponible(false);
        s->prestarLibro(isbn);

        std::cout << "Prestamo exitoso\n";
    }

    void devolverLibro() {
        std::string isbn;
        int id;
        std::cin.ignore();
        std::cout << "ISBN: "; getline(std::cin, isbn);
        std::cout << "ID Socio: "; std::cin >> id;

        Libro* l = buscarLibro(isbn);
        Socio* s = buscarSocio(id);

        if (!l || !s) {
            std::cout << "ERROR: Libro o socio no existe\n";
            return;
        }

        // ******* ROBUSTEZ / MANEJO DE ERRORES *******
        if (!s->tieneLibro(isbn)) {
            std::cout << "ERROR: El socio no tiene ese libro\n";
            return;
        }

        l->setDisponible(true);
        s->devolverLibro(isbn);

        std::cout << "Devolucion registrada correctamente\n";
    }

    void mostrarLibros() const {
        for (Libro* l : catalogo)
            l->mostrar();
    }

    void mostrarSocios() const {
        for (Socio* s : socios)
            s->mostrar();
    }
};

// =======================================================
// MAIN – INTERFAZ DE USUARIO POR CONSOLA
// =======================================================
int main() {
    Biblioteca biblio;
    int opcion;

    // ******* INTERFAZ DE USUARIO (CUERPO DEL SISTEMA) *******
    do {
        std::cout << "\n--- MENU BIBLIOTECA ---\n";
        std::cout << "1. Agregar libro\n";
        std::cout << "2. Registrar socio\n";
        std::cout << "3. Prestar libro\n";
        std::cout << "4. Devolver libro\n";
        std::cout << "5. Mostrar libros\n";
        std::cout << "6. Mostrar socios\n";
        std::cout << "0. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1: biblio.agregarLibro(); break;
            case 2: biblio.registrarSocio(); break;
            case 3: biblio.prestarLibro(); break;
            case 4: biblio.devolverLibro(); break;
            case 5: biblio.mostrarLibros(); break;
            case 6: biblio.mostrarSocios(); break;
            case 0: std::cout << "Saliendo del sistema...\n"; break;
            default: std::cout << "Opcion invalida\n";
        }
    } while (opcion != 0);

    return 0;
}
