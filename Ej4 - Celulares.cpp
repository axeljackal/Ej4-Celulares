#include <iostream>
#include <string>

void clearScreen(); // Prototipo de la función para limpiar la pantalla

// Función para limpiar la pantalla
void clearScreen() { 
        #if defined(_WIN32) // Windows
            system("cls"); 
        #elif defined(__linux__) || defined(__APPLE__) // Linux o Mac
            system("clear");
        #else
            std::cout << "Sistema operativo no reconocido. No se pudo limpiar la pantalla." << std::endl;
        #endif
}

class Celular {
private:
    float bateria;
    std::string modelo;

public:
    Celular(const std::string& modelo) : bateria(100), modelo(modelo) {}

    virtual void realizarLlamada(int duracion) = 0;

    void recargar() {
        bateria = 100;
        std::cout << "Celular " << modelo << " recargado. Bateria al 100%" << std::endl;
    }

    bool estaApagado() const {
        return bateria <= 0;
    }

    void setBateria(float newBateria) {
        bateria = newBateria;
    }

    float getBateria() const {
        return bateria;
    }

    const std::string& getModelo() const {
        return modelo;
    }
};

class SamsungS21 : public Celular {
public:
    SamsungS21() : Celular("Samsung S21") {}

    void realizarLlamada(int duracion) override {
        if (estaApagado()) {
            std::cout << "No puedes hacer una llamada. El celular " << getModelo() << " esta apagado." << std::endl;
            return;
        }

        setBateria(getBateria() - 5);
        std::cout << "Llamada con " << getModelo() << " por " << duracion << " minutos." << std::endl;
    }
};

class iPhone : public Celular {
public:
    iPhone() : Celular("iPhone") {}

    void realizarLlamada(int duracion) override {
        if (estaApagado()) {
            std::cout << "No puedes hacer una llamada. El celular " << getModelo() << " esta apagado." << std::endl;
            return;
        }

        setBateria(getBateria() - 0.1 * duracion);
        std::cout << "Llamada con " << getModelo() << " por " << duracion << " minutos." << std::endl;
    }
};

class CompaniaTelefonica {
private:
    float costoLlamada;

public:
    CompaniaTelefonica(float costo) : costoLlamada(costo) {}

    virtual float calcularCosto(int duracion) const = 0;

    float getCostoLlamada() const {
        return costoLlamada;
    }
};

class Movistar : public CompaniaTelefonica {
public:
    Movistar() : CompaniaTelefonica(60) {}

    float calcularCosto(int duracion) const override {
        return getCostoLlamada() * duracion;
    }
};

class Claro : public CompaniaTelefonica {
public:
    Claro() : CompaniaTelefonica(50) {}

    float calcularCosto(int duracion) const override {
        return getCostoLlamada() * duracion * 1.21;
    }
};

class Personal : public CompaniaTelefonica {
public:
    Personal() : CompaniaTelefonica(70) {}

    float calcularCosto(int duracion) const override {
        if (duracion <= 10) {
            return getCostoLlamada() * duracion;
        } else {
            return getCostoLlamada() * 10 + 40 * (duracion - 10);
        }
    }
};

class Persona {
private:
    Celular* celular;
    CompaniaTelefonica* compania;
    float totalCostoLlamadas;

public:
    Persona(Celular* c, CompaniaTelefonica* ct) : celular(c), compania(ct), totalCostoLlamadas(0) {}

    ~Persona() {
        delete celular;
        delete compania;
    }

    void llamar(Persona& p, int duracion) {
        if (estaCelularApagado()) {
            std::cout << "No puedes hacer una llamada. Tu celular esta apagado." << std::endl;
            return;
        }

        if (p.estaCelularApagado()) {
            std::cout << "No puedes llamar a " << p.getCelular()->getModelo() << ". Su celular esta apagado." << std::endl;
            return;
        }

        celular->realizarLlamada(duracion);
        totalCostoLlamadas += compania->calcularCosto(duracion);
    }

    void recargarCelular() {
        celular->recargar();
    }

    bool estaCelularApagado() const {
        return celular->estaApagado();
    }

    float getTotalCostoLlamadas() const {
        return totalCostoLlamadas;
    }

    Celular* getCelular() const {
        return celular;
    }
};

void mostrarEstado(const Persona& juliana, const Persona& catalina) {
    std::cout << "Bateria del " << juliana.getCelular()->getModelo() << " de Juliana: " << juliana.getCelular()->getBateria() << "%" << std::endl;
    std::cout << "Bateria del " << catalina.getCelular()->getModelo() << " de Catalina: " << catalina.getCelular()->getBateria() << "%" << std::endl;
    std::cout << "Costo total de llamadas de Juliana: $" << juliana.getTotalCostoLlamadas() << std::endl;
    std::cout << "Costo total de llamadas de Catalina: $" << catalina.getTotalCostoLlamadas() << std::endl;
}

int main() {
    SamsungS21 s21;
    iPhone iphone;
    Movistar movistar;
    Personal personal;

    Persona juliana(&s21, &personal);
    Persona catalina(&iphone, &movistar);

    while (true) {
        mostrarEstado(juliana, catalina);
        std::string action;
        std::cout << "\nElige una accion (llamar, recargar, salir): ";
        std::cin >> action;

        if (action == "salir") {
            break;
        } else if (action == "llamar") {
            std::string caller, receiver;
            int duration;
            std::cout << "Ingresa el nombre del emisor de la llamada (Juliana, Catalina): ";
            std::cin >> caller;
            std::cout << "Ingresa el nombre del receptor de la llamada (Juliana, Catalina): ";
            std::cin >> receiver;
            std::cout << "Elige la duracion de la llamada: ";
            std::cin >> duration;

            if (caller == "Juliana" && receiver == "Catalina") {
                juliana.llamar(catalina, duration);
            } else if (caller == "Catalina" && receiver == "Juliana") {
                catalina.llamar(juliana, duration);
            }
        } else if (action == "recargar") {
            std::string person;
            std::cout << "Elige la persona a la que se va a recargar (Juliana, Catalina): ";
            std::cin >> person;

            if (person == "Juliana") {
                juliana.recargarCelular();
            } else if (person == "Catalina") {
                catalina.recargarCelular();
            }
        }

        clearScreen();
    }

    return 0;
}
