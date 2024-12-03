#include <iostream>
#include <unistd.h>
#include <random> 
#include <vector>
#include <cmath>
using namespace std;

//##################### VARIABLES Y ESTRUCTURAS ######################//

struct Page {
    int processID;
    int pageNumber;
    bool isInRAM;
};
struct Process {
    int id;
    float size;
    vector<Page> pages;
};

unsigned int virtualPages;
unsigned int ramPages;
int nextProcessID = 0;
float VirMemCoef; // Virtual Memory Coeficent (multiplicador de la memoria)
float pageSize;
int ramSize;

vector<Process> processes; // Lista de procesos creados
vector<Page> RAM(ramPages);
vector<Page> VirtualMem(virtualPages);

//##################### FUNCIONES Y LÓGICA ######################//

Process createProcess() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> sizeDist(1.0, 10.0); // Tamaño del proceso entre 1 MB y 10 MB

    Process newProcess;
    newProcess.id = nextProcessID++;
    newProcess.size = sizeDist(gen);

    // Ceil aproxima al entero mayor
    int pagesNeeded = ceil(newProcess.size / pageSize);

    cout << "Creando proceso " << newProcess.id << " de tamaño " << newProcess.size 
         << " MB (" << pagesNeeded << " páginas)\n";

    // Chekea el espacio en ram o memoria virtual 
    for (int i = 0; i < pagesNeeded; ++i) {
        if (RAM.size() < ramPages) {
            Page newPage = {newProcess.id, i, true};
            RAM.push_back(newPage);
            newProcess.pages.push_back(newPage);
        } else if (VirtualMem.size() < virtualPages) {
            Page newPage = {newProcess.id, i, false};
            VirtualMem.push_back(newPage);
            newProcess.pages.push_back(newPage);
        } else {
            cout << "Memoria insuficiente. Terminando simulación.\n";
            exit(1);
        }
    }

    return newProcess;
}

// Multiplicador aleatorio 1.5-4.5
float coeficentInitializer()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(1.5, 4.5);
    return dis(gen);
}

void initializeRAM()
{
    int pgSize;
    cout << "Ingrese el tamaño de la RAM en MB: ";
    cin >> ramSize;
    cout << "Ingrese el tamaño de cada página en KB: ";
    cin >> pgSize;

    VirMemCoef = coeficentInitializer();

    // Paso la pagina a MB
    pageSize = pgSize / 1024.0;

    // Calcula el número de páginas fisicas y virtuales
    ramPages = ceil(ramSize / pageSize);
    virtualPages = ramPages * VirMemCoef;

    cout << "Tamaño de la RAM: " << ramSize << " MB (" << ramPages << " páginas)" << endl;
    cout<< "Multiplicador para memoria virual aleatorio: "<<VirMemCoef<<endl;
    cout << "Tamaño de la memoria virtual: " << (ramSize * VirMemCoef) << " MB (" << virtualPages << " páginas)" << endl;

}

void printProcesses() {
    for (const auto& p : processes) {
        cout << "Proceso ID: " << p.id << ", Tamaño: " << p.size << " MB\n";
    }
}


int main() {
    initializeRAM();

    while (true)
    {
        Process newProcess = createProcess();
        processes.push_back(newProcess);
        printProcesses();
        sleep(2);
    }

    return 0;
}
