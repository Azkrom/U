#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>

using namespace std;

class Empleado {
private:
    string nombre;
    int edad;
    int antiguedad;
    int ID;
    string area;
    string puesto;
    double salario;
    int c_seguridad[3];
public:
    Empleado(string nombre, int edad, int antiguedad, int ID, string area, string puesto, double salario, int c_seguridad[]) {
        this->nombre = nombre;
        this->edad = edad;
        this->antiguedad = antiguedad;
        this->ID = ID;
        this->area = area;
        this->puesto = puesto;
        this->salario = salario;
        for (int i = 0; i < 3; i++) {
            this->c_seguridad[i] = c_seguridad[i];
        }
    }
    string getNombre() {
        return this->nombre;
    }
    int getEdad() {
        return this->edad;
    }
    int getAntiguedad() {
        return this->antiguedad;
    }
    int getID() {
        return this->ID;
    }
    string getArea() {
        return this->area;
    }
    string getPuesto() {
        return this->puesto;
    }
    double getSalario() {
        return this->salario;
    }
    int getCodigoSeguridad(int i) {
        return this->c_seguridad[i];
    }
    void setNombre(string nombre) {
        this->nombre = nombre;
    }
    void setEdad(int edad) {
        this->edad = edad;
    }
    void setAntiguedad(int antiguedad) {
        this->antiguedad = antiguedad;
    }
    void setID(int ID) {
        this->ID = ID;
    }
    void setArea(string area) {
        this->area = area;
    }
    void setPuesto(string puesto) {
        this->puesto = puesto;
    }
    void setSalario(double salario) {
        this->salario = salario;
    }
    void setCodigoSeguridad(int c_seguridad, int i) {
        this->c_seguridad[i] = c_seguridad;
    }
};

unordered_map<int, Empleado*> mapa;

int obtenerUltimaID(unordered_map<int, Empleado*>& mapa) {
    int ultimaID = 20230;
    for (const auto& entry : mapa) {
        if (entry.first > ultimaID) {
            ultimaID = entry.first;
        }
    }
    return ultimaID;
}


void crearEmpleado(string nombre, int edad, int antiguedad, string area, string puesto, double salario, int c_seguridad[], unordered_map<int, Empleado*>& mapa) {
    int nuevaID = obtenerUltimaID(mapa) + 1;
    Empleado* nuevoEmpleado = new Empleado(nombre, edad, antiguedad, nuevaID, area, puesto, salario, c_seguridad);
    mapa[nuevaID] = nuevoEmpleado;
}


void guardarEmpleados(string nombreArchivo) {
    ofstream archivo(nombreArchivo + ".txt");
    if (archivo.is_open()) {
        for (auto const& par : mapa) {
            Empleado* empleado = par.second;
            archivo << empleado->getNombre() << ",";
            archivo << empleado->getEdad() << ",";
            archivo << empleado->getAntiguedad() << ",";
            archivo << empleado->getID() << ",";
            archivo << empleado->getArea() << ",";
            archivo << empleado->getPuesto() << ",";
            archivo << empleado->getSalario() << ",";
            archivo << empleado->getCodigoSeguridad(0) << ",";
            archivo << empleado->getCodigoSeguridad(1) << ",";
            archivo << empleado->getCodigoSeguridad(2) << endl;
        }
        archivo.close();
        cout << "Datos guardados en el archivo " << nombreArchivo << endl;
    } else {
        cout << "No se pudo abrir el archivo " << nombreArchivo << endl;
    }
}

void leerEmpleado(int ID, unordered_map<int, Empleado*>& mapa) {
    if (mapa.find(ID) != mapa.end()) {
        Empleado* empleado = mapa[ID];
        cout << "Empleado con ID " << ID << " encontrado" << endl;
        cout << "Nombre: " << empleado->getNombre() << endl;
        cout << "Edad: " << empleado->getEdad() << " anos" << endl;
        cout << "Antiguedad: " << empleado->getAntiguedad() << " anos" << endl;
        cout << "ID: " << empleado->getID() << endl;
        cout << "Area: " << empleado->getArea() << endl;
        cout << "Puesto: " << empleado->getPuesto() << endl;
        cout << "Salario: " << empleado->getSalario() << " pesos" << endl;
    } else {
        cout << "Empleado con ID " << ID << " no encontrado" << endl;
    }
}

void actualizarEmpleado(string nombre, int edad, int antiguedad, int ID, string area, string puesto, double salario, int c_seguridad[], unordered_map<int, Empleado*>& mapa) {
    if (mapa.find(ID) != mapa.end()) {
        Empleado* empleado = mapa[ID];
        empleado->setNombre(nombre);
        empleado->setEdad(edad);
        empleado->setAntiguedad(antiguedad);
        empleado->setArea(area);
        empleado->setPuesto(puesto);
        empleado->setSalario(salario);
        for (int i = 0; i < 3; i++) {
            empleado->setCodigoSeguridad(c_seguridad[i], i);
        }
        cout << "Empleado actualizado" << endl;
    } else {
        cout << "Empleado con ID " << ID << " no encontrado" << endl;
    }
}

void borrarEmpleado(int ID, unordered_map<int, Empleado*>& mapa) {
    if (mapa.find(ID) != mapa.end()) {
        delete mapa[ID];
        mapa.erase(ID);
        cout << "Empleado con ID " << ID << " borrado" << endl;
    } else {
        cout << "Empleado con ID " << ID << " no encontrado" << endl;
    }
}

void mostrarPromedioSalarios(const unordered_map<int, Empleado*>& mapa, const vector<int>& listaIDs) {
    if (mapa.empty()) {
        cout << "No hay empleados en el sistema" << endl;
        return;
    }

    double sumaSalarios = 0;
    int contadorEmpleados = 0;
    for (int ID : listaIDs) {
        auto it = mapa.find(ID);
        if (it != mapa.end()) {
            sumaSalarios += it->second->getSalario();
            contadorEmpleados++;
        }
    }
    if (contadorEmpleados > 0) {
        double promedioSalarios = sumaSalarios / contadorEmpleados;
        cout << "El promedio de los salarios de los empleados seleccionados es: " << promedioSalarios << endl;
    } else {
        cout << "No se encontraron empleados con los IDs proporcionados" << endl;
    }
}


void mostrarPromedioSalariosGeneral(const unordered_map<int, Empleado*>& mapa) {
    if (mapa.empty()) {
        cout << "No hay empleados en el sistema" << endl;
        return;
    }

    double sumaSalarios = 0;
    int contadorEmpleados = 0;
    for (const auto& par : mapa) {
        sumaSalarios += par.second->getSalario();
        contadorEmpleados++;
    }
    double promedioSalarios = sumaSalarios / contadorEmpleados;
    cout << "\nEl promedio de los salarios de todos los empleados es: " << promedioSalarios << endl;
}

int main() {
    string nombreNuevoEmpleado;
    int edadNuevoEmpleado;
    int antiguedadNuevoEmpleado;
    int IDNuevoEmpleado;
    int IDEmpleadoExistente;
    string areaNuevoEmpleado;
    string puestoNuevoEmpleado;
    double salarioNuevoEmpleado;
    int c_seguridadNuevoEmpleado[3] = {};


    int c_seguridad1[] = {2, 9, 4};
    Empleado e1("Juan", 25, 5, 20231, "Ventas", "Gerente", 5000, c_seguridad1);
    crearEmpleado("Juan", 25, 5, "Ventas", "Gerente", 5000, c_seguridad1, mapa);
    cout << e1.getNombre() << endl;
    cout << e1.getEdad() << " anos de edad" << endl;
    cout << e1.getAntiguedad() << " anos" << endl;
    cout << "ID: " << e1.getID() << endl;
    cout << "Area: " << e1.getArea() << endl;
    cout << "Puesto: " << e1.getPuesto() << endl;
    cout << "Salario: " << "$" << e1.getSalario() << " pesos" << endl;
    cout << "Codigo de Seguridad: " << e1.getCodigoSeguridad(0) << e1.getCodigoSeguridad(1) << e1.getCodigoSeguridad(2) << endl;
    
    cout << "\n";

    int c_seguridad2[] = {3, 9, 8};
    Empleado e2("Pedro", 39, 7, 20232, "Almacen", "Gerente", 3500, c_seguridad2);
    crearEmpleado("Pedro", 39, 7, "Almacen", "Gerente", 3500, c_seguridad2, mapa);
    cout << e2.getNombre() << endl;
    cout << e2.getEdad() << " anos de edad" << endl;
    cout << e2.getAntiguedad() << " anos" << endl;
    cout << "ID: " << e2.getID() << endl;
    cout << "Area: " << e2.getArea() << endl;
    cout << "Puesto: " << e2.getPuesto() << endl;
    cout << "Salario: " << "$" << e2.getSalario() << " pesos" << endl;
    cout << "Codigo de Seguridad: " << e2.getCodigoSeguridad(0) << e2.getCodigoSeguridad(1) << e2.getCodigoSeguridad(2) << endl;

    cout << "\n";
    
    int c_seguridad3[] = {0, 1, 3};
    Empleado e3("Lorenzo", 27, 3, 20233, "Recursos Humanos", "Jefe de RH", 9000, c_seguridad3);
    crearEmpleado("Lorenzo", 27, 3, "Recursos Humanos", "Jefe de RH", 9000, c_seguridad3, mapa);
    cout << e3.getNombre() << endl;
    cout << e3.getEdad() << " anos de edad" << endl;
    cout << e3.getAntiguedad() << " anos" << endl;
    cout << "ID: " << e3.getID() << endl;
    cout << "Area: " << e3.getArea() << endl;
    cout << "Puesto: " << e3.getPuesto() << endl;
    cout << "Salario: " << "$" << e3.getSalario() << " pesos" << endl;
    cout << "Codigo de Seguridad: " << e3.getCodigoSeguridad(0) << e3.getCodigoSeguridad(1) << e3.getCodigoSeguridad(2) << endl;
    
    cout << "\n";

    while (true) {
        cout << "\nIngrese un comando (crear, leer, actualizar, borrar, guardar, promedio_general, promedio, salir): ";
        string comando;
        cin >> comando;

        if (comando == "crear") {
            cout << "Ingresa nuevo empleado\n";
            cout << "Ingresa el nombre del nuevo empleado\n";
            cin >> nombreNuevoEmpleado;
            cout << "Ingresa edad de nuevo empleado\n";
            cin >> edadNuevoEmpleado;
            cout << "Ingresa antiguedad de nuevo empleado\n";
            cin >> antiguedadNuevoEmpleado;
            cout << "Ingresa area de nuevo empleado\n";
            cin >> areaNuevoEmpleado;
            cout << "Ingresa puesto del nuevo empleado\n";
            cin >> puestoNuevoEmpleado;
            cout << "Ingresa salario del nuevo empleado\n";
            cin >> salarioNuevoEmpleado;
            cout << "Ingresa primer digito de seguridad del nuevo empleado\n";
            cin >> c_seguridadNuevoEmpleado[0];
            cout << "Ingresa segundo digito de seguridad del nuevo empleado\n";
            cin >> c_seguridadNuevoEmpleado[1];
            cout << "Ingresa tercer digito de seguridad del nuevo empleado\n";
            cin >> c_seguridadNuevoEmpleado[2];
            crearEmpleado(nombreNuevoEmpleado, edadNuevoEmpleado, antiguedadNuevoEmpleado, areaNuevoEmpleado, puestoNuevoEmpleado, salarioNuevoEmpleado, c_seguridadNuevoEmpleado, mapa);
        
        } else if (comando == "leer") {
            cout << "Ingresa el empleado a leer\n";
            cout << "Ingresa el ID del empleado\n";
            cin >> IDEmpleadoExistente;
            leerEmpleado(IDEmpleadoExistente, mapa);
        
        } else if (comando == "actualizar") {
            cout << "Ingresa los del empleado a actualizar\n";
            cout << "Ingresa la ID del empleado a actualizar\n";
            cin >> IDNuevoEmpleado;
            cout << "Ingresa el nuevo nombre del empleado\n";
            cin >> nombreNuevoEmpleado;
            cout << "Ingresa la nueva edad del empleado\n";
            cin >> edadNuevoEmpleado;
            cout << "Ingresa la nueva antiguedad del empleado\n";
            cin >> antiguedadNuevoEmpleado;
            cout << "Ingresa la nueva area del empleado\n";
            cin >> areaNuevoEmpleado;
            cout << "Ingresa la nuevo puesto del empleado\n";
            cin >> puestoNuevoEmpleado;
            cout << "Ingresa el nuevo salario del empleado\n";
            cin >> salarioNuevoEmpleado;
            cout << "Ingresa el nuevo primer digito de seguridad del empleado\n";
            cin >> c_seguridadNuevoEmpleado[0];
            cout << "Ingresa el nuevo segundo digito de seguridad del empleado\n";
            cin >> c_seguridadNuevoEmpleado[1];
            cout << "Ingresa el nuevo tercer digito de seguridad del empleado\n";
            cin >> c_seguridadNuevoEmpleado[2];
            actualizarEmpleado(nombreNuevoEmpleado, edadNuevoEmpleado, antiguedadNuevoEmpleado, IDNuevoEmpleado, areaNuevoEmpleado, puestoNuevoEmpleado, salarioNuevoEmpleado, c_seguridadNuevoEmpleado, mapa);
        
        } else if (comando == "borrar") {
            cout << "Ingresa el empleado a borrar\n";
            cout << "Ingresa el ID del empleado\n";
            cin >> IDEmpleadoExistente;
            borrarEmpleado(IDEmpleadoExistente, mapa);
        } else if (comando == "promedio_general") {
            mostrarPromedioSalariosGeneral(mapa);
        } else if (comando == "promedio"){
            cout << "Ingresa una lista de IDs separados por espacios: ";
            cin.ignore();
            string input;
            getline(cin, input);
            istringstream iss(input);
            vector<int> listaIDs;
            int ID;
            while (iss >> ID) {
                listaIDs.push_back(ID);
            }
            mostrarPromedioSalarios(mapa, listaIDs);
        } else if (comando == "guardar") {
          cout << "Ingrese el nombre del archivo donde se guardarán los datos: ";
          string nombreArchivo;
          cin >> nombreArchivo;
          guardarEmpleados(nombreArchivo);
        } else if (comando == "salir") {
            break;
         } else {
            cout << "Comando no valido" << endl;
        }
    }

    return 0;
}