# Algoritmos de procesamiento de imágenes

Aplicación de escritorio desarrollada en **C++ y Qt Widgets** para aplicar y visualizar algoritmos básicos de procesamiento de imágenes. Las operaciones se implementan desde cero sobre `QImage`, sin utilizar bibliotecas externas de visión artificial como OpenCV.

El programa permite cargar dos imágenes de entrada (`P` y `Q`), seleccionar una de ellas para las operaciones individuales y mostrar el resultado en un tercer panel de salida.

## Funcionalidades

- Conversión a escala de grises.
- Binarización mediante un umbral ajustable.
- Cálculo automático del umbral a partir de los parámetros `K1` y `K2`.
- Inversión de la imagen (operación lógica NOT).
- Intersección de dos imágenes (operación lógica AND).
- Resta de intensidades entre dos imágenes.
- Desplazamiento horizontal y vertical mediante los valores `X` e `Y`.
- Etiquetado de componentes conexos con conectividad 4 y recorrido BFS.
- Filtrado de regiones según su superficie.
- Proceso compuesto: desplazamiento, resta, cálculo automático del umbral y binarización.
- Apertura y guardado de imágenes JPG, PNG y BMP.

> El etiquetado y el filtrado por superficie requieren una imagen binarizada. La resta y la intersección requieren dos imágenes con las mismas dimensiones.

## Demostración

Puedes consultar una grabación del funcionamiento de la aplicación en [usage_demo.mp4](usage_demo.mp4).

## Requisitos

- Un compilador compatible con C++11 o posterior.
- Qt con los módulos `Core`, `Gui` y `Widgets` (recomendado: Qt 5).
- `qmake` y una herramienta de compilación compatible, como MinGW, GCC o MSVC.

No se necesitan dependencias adicionales.

## Compilación

### Desde Qt Creator

1. Abre `image_processing_algorithms.pro` con Qt Creator.
2. Selecciona un kit de escritorio que incluya Qt Widgets.
3. Configura el proyecto y pulsa **Build** o **Run**.

### Desde la terminal

Con Qt y el compilador configurados en el `PATH`:

```bash
qmake image_processing_algorithms.pro
make
```

En Windows con MinGW, el último comando puede ser:

```powershell
mingw32-make
```

El ejecutable generado se llama `robots_hands_detection` (`robots_hands_detection.exe` en Windows).

## Uso

1. Pulsa **Open** bajo los paneles `P` y `Q` para cargar una o dos imágenes.
2. Marca `selected_p` o `selected_q` para elegir la imagen sobre la que se aplicarán las operaciones individuales.
3. Utiliza los botones y controles de la interfaz:
   - **Grayscale**, **Invert** y **Move** aplican una transformación directa.
   - El control **threshold** binariza la imagen seleccionada.
   - **Autothreshold** calcula un umbral usando `K1` y `K2` y muestra el resultado binarizado.
   - **Labelling** identifica y colorea los componentes conexos.
   - El control **filter** elimina regiones según su área relativa.
   - **Substraction** e **Intersection** operan sobre `P` y `Q`.
   - **Proces A** desplaza la imagen seleccionada, resta ambas versiones y aplica umbralización automática.
4. El resultado aparece en el panel `S (output)`.
5. Pulsa **Save** para exportarlo como JPG, PNG o BMP.

## Estructura del proyecto

```text
.
|-- Algoritmos.cpp/.hpp              # Implementación de los algoritmos
|-- interface.cpp/.h                 # Lógica y eventos de la interfaz
|-- interface.ui                     # Diseño de la interfaz Qt Widgets
|-- main.cpp                         # Punto de entrada de la aplicación
|-- utils.cpp/.hpp                   # Funciones auxiliares
|-- image_processing_algorithms.pro  # Configuración de qmake
|-- images/                          # Imágenes de ejemplo
`-- usage_demo.mp4                   # Vídeo de demostración
```

## Notas

- Las imágenes incluidas en `images/` pueden utilizarse para probar la aplicación.
- Para evitar resultados inesperados, selecciona solamente una imagen (`P` o `Q`) al ejecutar operaciones individuales.
- El proyecto tiene un propósito educativo: prioriza mostrar la implementación de los algoritmos sobre el uso de funciones ya preparadas por bibliotecas de visión artificial.
