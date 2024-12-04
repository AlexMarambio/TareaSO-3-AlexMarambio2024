#include <iostream>
#include <unistd.h>
#include <random> 
#include <vector>
#include <queue>
#include <ctime>
#include <cmath>
using namespace std;

const string colorFINISH = "\033[31m";
const string green = "\033[32m";
const string yellow = "\033[33m";
const string colorRAM = "\033[34m";
const string colorSWAP = "\033[38;5;129m";
const string reset = "\033[0m";

//##################### VARIABLES Y ESTRUCTURAS ######################//

unsigned int virtualPages;
unsigned int ramPages;
int nextProcessID = 1;
float VirMemCoef; // Virtual Memory Coeficent (multiplicador de la memoria)
float pageSize;
int ramSize;

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

vector<Page> VirtualMem(virtualPages);
vector<Process> processes; // Lista de procesos creados
queue<Page> RAM;

//##################### FUNCIONES Y LÓGICA ######################//

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
}

void watchRAMusage()
{
    cout << colorRAM <<"RAM: [ ";
    queue<Page> tempRAM = RAM;
    while (!tempRAM.empty()) {
        cout << "(pfn: " << tempRAM.front().pageNumber << ", pid: " << tempRAM.front().processID << "), ";
        tempRAM.pop();
    }
    cout << "]\n"<<reset;
}
// Tanto el de arriba como abajo es para fines de debuggin: imprime las paginas de la ram y el proceso que las ocupa
void watchSWAPusage()
{
    cout<<colorSWAP<<"SWAP: [ ";
    for(unsigned int i=0;i<VirtualMem.size();i++)
    {
        cout<<"(vpn: "<<VirtualMem[i].pageNumber<<", pid: "<<VirtualMem[i].processID<<"),";
    }
    cout<<" ]\n"<<reset;
}

int selectRandomProcess()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, processes.size() - 1);
    
    return dist(gen);
}

void printProcesses() {
    for (const auto& p : processes) {
        cout << "Proceso ID: " << p.id << ", Tamaño: " << p.size << " MB\n";
    }
}

Process createProcess() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> sizeDist(1.0, 10.0); // Tamaño del proceso entre 1 MB y 10 MB

    Process newProcess;
    newProcess.id = nextProcessID++;
    newProcess.size = sizeDist(gen);

    // Ceil aproxima al entero mayor
    int pagesNeeded = ceil(newProcess.size / pageSize);

    // Chequear el espacio en RAM o memoria virtual
    int rs = RAM.size();
    int vmem = VirtualMem.size(); 
    for (int i = 0; i < pagesNeeded; ++i) {
        if (RAM.size() < ramPages) {
            Page newPage = {newProcess.id, rs+i, true};
            RAM.push(newPage); // Insertamos la página en la RAM (FIFO)
            newProcess.pages.push_back(newPage);

        } else if (VirtualMem.size() < virtualPages) {
            Page newPage = {newProcess.id, vmem+i, false};
            VirtualMem.push_back(newPage);
            newProcess.pages.push_back(newPage);

        } else {
            cout << colorFINISH << "Memoria insuficiente. Terminando simulación.\n"<< reset;
            watchRAMusage();
            watchSWAPusage();
            
            cout << "Tamaño de la RAM: " << ramSize << " MB (" << ramPages << " páginas)" << endl;
            cout << "Multiplicador para memoria virtual aleatorio: " << VirMemCoef << endl;
            cout << "Tamaño de la memoria virtual: " << (ramSize * VirMemCoef) << " MB (" << virtualPages << " páginas)"<< endl;
            exit(1);
        }
    }
    cout<<"Proceso "<<newProcess.id<<" ("<<newProcess.size<<" MB) creado.\n";
    return newProcess;
}

void terminateProcess(int processID) {
    queue<Page> tempRAM;
    while (!RAM.empty()) {
        Page currentPage = RAM.front();
        RAM.pop();
        if (currentPage.processID != processID) {
            tempRAM.push(currentPage);  // Mantiene las páginas de otros procesos
        }
    }
    cout<<"Proceso: "<<processID<<" eliminado.\n";
    RAM = tempRAM;
}

void accessMemory() {
    // Genera una dirección virtual aleatoria (número de página)
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, VirtualMem.size() - 1);
    int virtualPageNumber = dist(gen);

    // Verifica si la página está en la RAM
    bool pageFoundInRAM = false;
    queue<Page> tempRAM = RAM;
    while (!tempRAM.empty()) {
        if (tempRAM.front().pageNumber == virtualPageNumber) {
            pageFoundInRAM = true;
            break;
        }
        tempRAM.pop();
    }

    if (pageFoundInRAM) {
        cout << "Página " << virtualPageNumber << " encontrada en la RAM.\n";
    } else {
        // Page Fault: la página no está en la RAM, buscarla en la memoria virtual
        cout << yellow <<"Page Fault: Página " << virtualPageNumber << " no encontrada en la RAM. Buscando en la memoria virtual...\n"<<reset;

        // Buscar la página en la memoria virtual
        Page pageInSwap;
        bool pageFoundInSwap = false;
        for (auto& page : VirtualMem) {
            if (page.pageNumber == virtualPageNumber) {
                pageInSwap = page;
                pageFoundInSwap = true;
                break;
            }
        }

        if (pageFoundInSwap) {
            // Página encontrada en la memoria virtual (swap), ahora la movemos a la RAM
            cout << green <<"Página " << virtualPageNumber << " encontrada en la memoria virtual (swap).\n"<<reset;

            // Si la RAM está llena, usamos FIFO para reemplazar la página más antigua
            if (RAM.size() >= ramPages) {
                // Reemplazar página más antigua en RAM usando FIFO
                Page oldestPage = RAM.front();
                cout << "Reemplazando página más antigua en la RAM: Proceso " << oldestPage.processID << ", Página " << oldestPage.pageNumber << endl;

                if (VirtualMem.size() < virtualPages) {
                    VirtualMem.push_back(oldestPage);
                } 

                RAM.pop();  // Elimina la página más antigua

                // Añadir la página que venía del swap a la RAM
                pageInSwap.isInRAM = true;  // Marcamos la página como estando en RAM
                RAM.push(pageInSwap);  // La añadimos a la RAM
                cout << "Página " << pageInSwap.pageNumber << " del proceso " << pageInSwap.processID << " añadida a la RAM.\n";
            } else {
                // Si hay espacio en RAM, simplemente insertamos la página directamente
                pageInSwap.isInRAM = true;  // Marcamos la página como estando en RAM
                RAM.push(pageInSwap);  // La añadimos a la RAM
                cout << "Página " << pageInSwap.pageNumber << " del proceso " << pageInSwap.processID << " añadida a la RAM.\n";
            }
        } else {
            cout <<  yellow <<"Página " << virtualPageNumber << " no encontrada en la memoria virtual (swap).\n"<<reset;
        }
    }
}


int main() {
    initializeRAM();
    time_t startTime = time(0);

    while (true)
    {

        if ((time(0) - startTime) % 2 == 0)
        {
            Process newProcess = createProcess();
            processes.push_back(newProcess);
        }

        if ((time(0) - startTime) % 5 == 0 && processes.size()>3)
        {
            int randomProcessIndex = selectRandomProcess();
            int processID = processes[randomProcessIndex].id;
            terminateProcess(processID);
            processes.erase(processes.begin() + randomProcessIndex);
        }

        if ((time(0) - startTime) % 5 == 0 && VirtualMem.size()!=0)
        {
            accessMemory();
        }

        if ((time(0) - startTime) % 10 == 0)
        {
            watchRAMusage();
            watchSWAPusage();
        }
        sleep(1);
    }

    return 0;
}
