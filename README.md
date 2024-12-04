# TareaSO-3 - Simulador de Paginación en Memoria
### Creadores: **Alex Marambio y Diego Muñoz**

Este programa simula el mecanismo de **paginación de memoria** utilizado por los sistemas operativos para gestionar la memoria RAM y la memoria virtual (swap). Implementa funcionalidades clave como creación y eliminación de procesos, acceso a direcciones virtuales y manejo de *page faults* usando la política de reemplazo **FIFO**.
- Se recomienda ejecutar en Linux, aunque funciona para windows sin problema.
Para compilar y ejecutar el programa se ocupa (dentro del directorio donde se descargue o clone el repositorio), los siguientes comandos por consola:
-   compilar
``` 
g++ -o nombre_ejecutable T3-alex2024.cpp
```
- ejecutar
``` 
./nombre_ejecutable
```

---

## **Inputs del Programa**

Al iniciar la simulación, se solicita:

1. **Tamaño de la RAM:**

   - Ingresar un valor entero en MB. Ejemplo: `64`.

2. **Tamaño de cada página:**

   - Ingresar un valor entero en KB. Ejemplo: `2046`. (Para que sean 2 MB)

---

## **Outputs del Programa**

El programa genera mensajes en la consola indicando:

1. **Estado de la simulación:**

   - Mensajes sobre la creación de procesos y la asignación de páginas.
     ```
     Proceso 1 (5.12 MB) creado.
     ```
   - Accesos a memoria y *page faults.*
     ```
     Página 3 no encontrada en la RAM. Buscando en la memoria virtual...
     Página 3 encontrada en la memoria virtual (swap).
     ```

2. **Estado de la memoria:**

   - RAM y memoria virtual con información de las páginas cargadas:
     ```
     RAM: [ (pfn: 0, pid: 1), (pfn: 1, pid: 2) ]
     SWAP: [ (vpn: 3, pid: 1), (vpn: 4, pid: 3) ]
     ```
     Donde `pid` es el id del proceso cargado en esa página, `pfn` es el numero de marco físico y `vpn` numero de pagina virtual.
     Al realizar un swap el numero en el `vpn` será el mismo que quedará posteriormente en `pfn` y viceversa: Esto para simplificar la busqueda del cambio realizado y ver efectivamente que funciona el swapping tanto de memoria vitual -> RAM como RAM -> memoria virtual.

3. **Finalización del programa:**

   - Si la memoria se llena:
     ```
     Memoria insuficiente. Terminando simulación.
     Ultima RAM y SWAP:
     RAM: [ (pfn: 0, pid: 1), (pfn: 1, pid: 2) ]
     SWAP: [ (vpn: 3, pid: 1), (vpn: 4, pid: 3) ]
     Información de la simulación:
     Tamaño de la RAM 64 MB
     Multiplicador (cuantas veces la memoria virtual es más grande que la RAM): 2.5
     Tamaño memoria virtual: 160 MB
     ```

---

## **Funcionamiento del Código**

1. **Inicialización:**

   - El programa solicita al usuario ingresar:
     - El tamaño de la memoria RAM en MB.
     - El tamaño de cada página en **KB**.
   - Genera el tamaño de la memoria virtual como un valor aleatorio entre 1.5 y 4.5 veces la memoria RAM.
   - Calcula cuántas páginas pueden caber en RAM y en la memoria virtual.

2. **Creación de procesos:**

   - Cada 2 segundos, se crea un proceso con un tamaño aleatorio entre **1 MB y 10 MB**.
   - Las páginas del proceso se asignan primero a la RAM si hay espacio. Si no, se alojan en la memoria virtual.
   - Si no queda espacio en RAM ni en memoria virtual, la simulación termina con un mensaje de error.

3. **Acceso a memoria:**

   - Cada 5 segundos, el programa genera un acceso aleatorio a una dirección virtual (número de página).
   - Si la página solicitada no está en RAM, ocurre un *page fault* y se mueve la página desde la memoria virtual a la RAM.
    Para esto se emplea el algoritmo **FIFO**, osea que el primer proceso en entrar a la RAM será el primero en irse. También para una visualización más fácil de qe realmente se mueve la pagina a la RAM, el valor de esta no cambia. Como se verá a continuación si en el swap tenia numero de pagina vitual `6`, el numero de marco físico tambien será `6` y se podrá ver en la ayuda visual de la RAM y SWAP que implementamos junto con el proceso al que le pertenece (pid: id del proceso).

4. **Eliminación de procesos:**

   - Cada 5 segundos, un proceso aleatorio es eliminado.
   - Todas las páginas asociadas al proceso se eliminan tanto de la RAM como de la memoria virtual.

5. **Visualización:**

   - Cada 10 segundos, se muestra el estado actual de la RAM y la memoria virtual, incluyendo el identificador de proceso (PID) y el número de página como se mencionó más arriba para ver visualmente como se ejecuta el swapping.

6. **Colores en consola:**

   - Diferentes colores se usan para identificar eventos más facilmente:
     - **Verde:** Accesos exitosos a RAM o SWAP.
     - **Amarillo:** *Page faults* y errores al buscar una página.
     - **Rojo:** Finalización por falta de memoria.
     - **Azul y Morado:** Para representar visualmente la RAM y SWAP.
