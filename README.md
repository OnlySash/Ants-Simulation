# Simulación de Colonia de Hormigas sobre Estructura de Árbol

## Descripción

Este proyecto implementa una simulación simplificada de una colonia de hormigas que exploran una estructura de datos tipo árbol con el objetivo de localizar y consumir recursos (comida) ubicados en los nodos hoja.

El sistema modela el comportamiento de múltiples hormigas que recorren el árbol desde la raíz hasta alcanzar hojas, donde consumen unidades de alimento disponibles.


## Modelo del sistema

### Árbol (`tree`)
- Estructura generada aleatoriamente.
- Cada nodo puede tener múltiples hijos.
- Los nodos hoja contienen alimento.

### Nodo (`node`)
- Identificador único.
- Lista dinámica de hijos.
- Cantidad de comida (solo en hojas).

### Hormiga (`ant`)
- Identificador único.
- Contador de comida recolectada.
- Semilla para generación pseudoaleatoria.

### Simulación (`simulation`)
- Contiene el árbol y el conjunto de hormigas.
- Controla la ejecución del sistema.
- Administra el estado global de comida.

## Funcionamiento

1. Se genera un árbol aleatorio con un número definido de nodos.
2. Se asigna una cantidad fija de comida a cada nodo hoja.
3. Se inicializa un conjunto de hormigas.
4. Cada hormiga:
   - Parte desde la raíz del árbol.
   - Selecciona hijos de forma aleatoria.
   - Continúa hasta alcanzar una hoja.
   - Consume una unidad de comida si está disponible.
5. El proceso se repite hasta que se agota toda la comida.

## Implementacion de Paralelizacion

El paralelismo se implementará asignando cada hormiga a un hilo independiente, permitiendo que múltiples recorridos del árbol ocurran simultáneamente. Cada hilo ejecutará la lógica de desplazamiento desde la raíz hasta una hoja y el consumo de comida, mientras que el acceso a recursos compartidos osea la comida en los nodos hoja y el contador global, será controlado mediante mecanismos de sincronización (por ejemplo, mutex) para evitar condiciones de carrera. De esta forma, se busca mejorar el rendimiento de la simulación y modelar un comportamiento concurrente más cercano al de una colonia real de hormigas