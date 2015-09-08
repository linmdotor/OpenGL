# OpenGL

Proyectos realizados con el motor gráfico OpenGL.
Además se ha hecho uso de la API auxiliar "FreeGLUT" para la gestión de ventanas, de eventos y del bucle principal.

Las soluciones (archivos .sln) han sido generadas mediante Visual Studio 2013.


Descripción
-----------
Exiten dos proyectos con gráficos en 2D y otros dos con gráficos en 3D:

- El directorio "1-arbol_pitagoras", en 2D, crea en tiempo de ejecución un árbol de pitágoras. Trata elementos como geometrías básicas, área visible de escena y puerto de vista.
- En "2-movimiento_y_colisiones" se plantea una escena 2D, con geometrías básicas, a las que se le aplica movimiento y colisionan entre sí. Para ello se hace uso de puntos, vectores y operaciones entre ellos. Además se emplean algoritmos de intersección en 2D, y se generan máscaras para los distintos objetos.
- En "3-mallas_y_movimiento_3D" se hace uso de geoetrías en 3D pintando una escena de un satélite, que orbita alreedor de un planeta, que sigue una trayectoria en espiral. Para ello se crean mallas de polígonos mediante código (manualmente, por revolución, por extrusion...) y objetos cuádricos, y se mueven en torno a varias referencias, siguiendo un modelo jerárquico.
- En "4-camara_luces_y_textura_3D" se plantea una escena un poco más compleja, con una mayor jerarquía, en la que se hacen distintas pruebas con tipos de luces, proyecciones y movimientos de la cámara, y texturas aplicadas a objetos básicos.


Objetivos
---------
El objetivo principal es el de realizar distintas pruebas con OpenGL, e ir aprendiendo de forma práctica algunos conceptos de informática gráfica.
