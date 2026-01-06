# Architecture

**Not Updated**
This document describes the architecture for SceneComposer.

## Code Map

A section that briefly explains the different directories of the software.

&nbsp;

**`shaders/`**

-   A pair of Vertex shader and Fragment shader used to output colors, textures, lights...

-   A pair of Vertex shader and Fragment shader used to for 3D Picking...

&nbsp;

**`src/`**  
Main directory that contains most of the source code.

&nbsp;

**`src/core`**  
The core of the software.

-   The `Engine` class takes care of initializing the application, starting with libraries like GLFW and GLAD (but not limited to).  
    It then moves forward to initialize custom classes owned by the engine, such as `Camera`, `Shader`, `Picking`.

-   The `Camera` class is responsible for the movement of the scene, and the general perspective of the camera.  
    As stated before, the camera is owned by the engine, but it actually is logically independent (meaning it doesn't really need the engine to work).

-   The `Picking` class is used to take informations from the frame buffer object (fbo).
    More precisely, the fbo can be seen as a grid of 0, and models ID. Where a pixel of a model is drawn, instead of the color, its ID is saved. When we click on the screen we get the coordinates of the click and check if on that specific coordinates on the grid field an ID is saved.

&nbsp;

**`src/input`**

-   The `Keyboard` static class takes care of handling long presses correctly, as it is not provided by OpenGL.

-   The `Mouse` static class takes care of handling mouse position and movemente correctly.

&nbsp;

**`src/renderer`**

Contains classes related to the rendering of objects.

-   The `Mesh` class is a loading-library-indipendent class that takes care of initializing buffers and drawing the final mesh to
    output. Because of how **_AssImp_** works, a single 3d model will get split in multiple meshes depending on their material.

-   The `Model` class is a container of multiple related meshes. Its only job is to correctly store and draw said meshes.
    Each model has its own unique ID used in the 3D picking.

-   The `Shader` class takes care of the Vertex shader and Fragment shader.
    It also exposes a set of functions, like `set_mat4()` to set uniform variables.

-   The `Texture` class load, stores, and puts a single texture into memory. Said texture is stored in a single `Mesh` and can/will be used in the rendering loop.

&nbsp;

**`src/utils`**

Set of classes not strictly necessary for the software to work.

-   The `Time` static class provides an easy way to access the delta time.

-   The `ModelLoader` static class internally uses **_AssImp_** to get the all the meshes from a given filepath.  
    It then creates and returns a `Model` object containing an array of `Mesh`.

&nbsp;

**`src/main.cpp`**

The entry point of the app. An object of class `Engine` is initialized and the main loop starts.

The whole pipeline is:

1. `Engine` is initialized.
2. `ModelLoader` loads a model.
3. The rendering loops starts.
4. The function `Engine::begin_frame()` takes care of basic OpenGL boilerplate stuff, camera (mouse and keyboard) movement, and updating each shader.
5. Models are loaded in the frame buffer objects.
6. Models are rendered on the screen.
7. The UI is drawn.
8. The engine listen for a click and see if it matches with a model ID.
9. End the frame, some boilerplate ImGui and OpenGL stuff.
