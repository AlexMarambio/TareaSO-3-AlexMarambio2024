# TareaSO-3 - Simulador de Paginación en Memoria

Este programa simula el mecanismo de **paginación de memoria** utilizado por los sistemas operativos para gestionar la memoria RAM y la memoria virtual (swap). Implementa funcionalidades clave como creación de procesos, acceso a direcciones virtuales y manejo de *page faults* con políticas de reemplazo FIFO.

---

## **Funcionamiento del Código**

1. **Inicialización:**

   - El programa solicita al usuario ingresar:
     - El tamaño de la memoria RAM en MB.
     - El tamaño de cada página en KB.
   - Genera el tamaño de la memoria virtual como un valor aleatorio entre 1.5 y 4.5 veces la memoria RAM.
   - Calcula cuántas páginas pueden caber en RAM y en la memoria virtual.

2. **Creación de procesos:**

   - Cada 2 segundos, se crea un proceso con un tamaño aleatorio entre 1 MB y 10 MB.
   - Las páginas del proceso se asignan primero a la RAM si hay espacio. Si no, se alojan en la memoria virtual.
   - Si no queda espacio en RAM ni en memoria virtual, la simulación termina con un mensaje de error.

3. **Acceso a memoria:**

   - Cada 5 segundos, el programa genera un acceso aleatorio a una dirección virtual (número de página).
   - Si la página solicitada no está en RAM, ocurre un *page fault* y se mueve la página desde la memoria virtual a la RAM.
   - Si la RAM está llena, se utiliza la política **FIFO** (First-In-First-Out) para reemplazar la página más antigua en RAM.

4. **Eliminación de procesos:**

   - Cada 5 segundos, un proceso aleatorio es eliminado.
   - Todas las páginas asociadas al proceso se eliminan tanto de la RAM como de la memoria virtual.

5. **Visualización:**

   - Cada 10 segundos, se muestra el estado actual de la RAM y la memoria virtual, incluyendo el identificador de proceso (PID) y el número de página.

---

## **Inputs del Programa**

Al iniciar la simulación, se solicita:

1. **Tamaño de la RAM:**

   - Ingresar un valor entero en MB. Ejemplo: `1024`.

2. **Tamaño de cada página:**

   - Ingresar un valor entero en KB. Ejemplo: `4`.

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

3. **Finalización del programa:**

   - Si la memoria se llena:
     ```
     Memoria insuficiente. Terminando simulación.
     Ultima RAM y SWAP:
     RAM: [ (pfn: 0, pid: 1), (pfn: 1, pid: 2) ]
     SWAP: [ (vpn: 3, pid: 1), (vpn: 4, pid: 3) ]
     ```

---

## **Aspectos Importantes del Código**

1. **Estructuras principales:**

   - `Page`: Representa una página con su identificador de proceso, número de página y estado (si está en RAM o swap).
   - `Process`: Representa un proceso con un identificador, tamaño y sus páginas asociadas.

2. **RAM y memoria virtual:**

   - Se utiliza una cola (`queue`) para la RAM, implementando la política FIFO.
   - Se utiliza un vector (`vector`) para la memoria virtual.

3. **Colores en consola:**

   - Diferentes colores se usan para identificar eventos:
     - Verde: Accesos exitosos a RAM.
     - Amarillo: *Page faults.*
     - Rojo: Finalización por falta de memoria.

4. **Política de reemplazo:**

   - FIFO reemplaza la página más antigua en RAM cuando esta está llena.

---

## **Ejemplo de Ejecución**

**Inputs:**

```
Ingrese el tamaño de la RAM en MB: 1024
Ingrese el tamaño de cada página en KB: 4
```

**Outputs:**

```
Proceso 1 (3.5 MB) creado.
RAM: [ (pfn: 0, pid: 1), (pfn: 1, pid: 1) ]
SWAP: [ ]
Page Fault: Página 3 no encontrada en la RAM. Buscando en la memoria virtual...
Página 3 encontrada en la memoria virtual (swap).
Reemplazando página más antigua en la RAM: Proceso 1, Página 0
```

aa
