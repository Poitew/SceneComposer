# Architecture

This document describes the architecture for SceneComposer.

## Code Map

A section that briefly explains the different directories of the software.

&nbsp;

**`shaders/`**  
Vertex shader and Fragment shader used in the OpenGL pipeline.

&nbsp;

**`src/`**  
Main directory that contains most of the source code.

&nbsp;

**`src/core`**  
The core of the software.

-   The `Engine` class takes care of initializing the application, starting with libraries like GLFW and GLAD (but not limited to).  
    It then moves forward to initialize custom classes
    owned by the engine, such as `Camera` or `Shader`.

-   The `Camera` class is responsible for the movement of the scene, and the general perspective of the camera.  
    As stated before, the camera is owned by the engine, but it actually is logically independent (meaning it doesn't really need the engine to work).

&nbsp;

**`src/input`**

-   The `Keyboard` static class takes care of handling long presses correctly, as it is not provided by OpenGL.

&nbsp;

**`src/renderer`**

Contains classes related to the rendering of objects.

-   The `Mesh` class is a loading-library-indipendent class that takes care of initializing buffers and drawing the final mesh to output.

-   The `ModelLoader` static class internally uses **_AssImp_** to get the first mesh from a given filepath.  
    It then creates and returns a `Mesh` object with the vertices and indices of said mesh.

-   The `Shader` class takes care of the Vertex shader and Fragment shader.
    It also exposes a set of functions, like `set_mat4()` to set uniform variables. They are mostly used by the camera to set the projection and view matrices.

    The whole pipeline is:  
    Inside `Engine::begin_frame()` the function `Camera::move()` is called. The latter updates an internal **mat4** called **view**. Still inside `Engine::begin_frame()`, `Shader::set_mat4()` is called with the arguments `"view"` and `"Camera::get_view()"`.

&nbsp;

**`src/utils`**

Set of classes not strictly necessary for the software to work.

-   The `Time` static class provides an easy way to access the delta time.

&nbsp;

**`src/main.cpp`**

The entry point of the app. An object of class `Engine` is initialized and the main loop starts.
