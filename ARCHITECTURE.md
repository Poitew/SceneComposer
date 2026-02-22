# Architecture

This document describes the architecture for SceneComposer.

## Code Map

A section that briefly explains the different directories of the software.

&nbsp;

**`shaders/`**

- `icon_vs` and `icon.vs` are used to ouput png icons, and are used by the `IconModel` class. The models with this pair of shaders are unaffected by lightning and discard parts of textures where the alpha value is below 1.

- `mesh.vs` and `mesh.fs` are used to output colors, textures, lights...

- `picking.vs` and `picking.fs` are used for 3D Picking.

- `skybox.vs` and `skybox.fs` are used to display a cubemap texture onto a skybox, and move it with the camera.

- `to_cubemap.vs` and `to_cubemap.fs` are used to convert an HDRI to a cubemap.

&nbsp;

**`src/`**  
Main directory that contains most of the source code.

&nbsp;

**`src/core`**  
The core of the software.

- The `Engine` class takes care of initializing libraries, classes, shaders...
  It also holds functions related to the rendering loop such as GUI drawing, meshes rendering...

- The `Camera` class is responsible for the movement of the scene, and the general perspective of the camera.
  Its function `Camera::get_base_transform()` returns a view mat4. If the software is opened in vr mode this mat4 is subject to
  more operations. On the other hand, in a desktop app the view mat4 is the final matrix used to simulate a camera.

- The `Picking` class is used to take information from the frame buffer object (fbo).
  More precisely, the fbo can be seen as a grid of 0 and models ID. Where a pixel of a model is drawn, instead of the color we draw its ID. When we click on the screen we get the coordinates of the click and check if an ID is saved at those specific coordinates. We can then use this ID to operate on a specific model.

- The `VRContext` class is used to prepare OpenXR to init a vr environment. It also handles the view mat4 transformation.

&nbsp;

**`src/input`**

- The `Keyboard` static class takes care of handling long presses correctly, as it is not provided by OpenGL.

- The `Mouse` static class takes care of handling mouse position and movement correctly.

&nbsp;

**`src/renderer`**

Contains classes related to the rendering of objects.

- The `Mesh` class is a loading-library-independent class that takes care of initializing buffers and drawing the final mesh to
  output. Because of how **_AssImp_** works, a single 3d model will get split in multiple meshes depending on their material.

- The `Model` class is a container of multiple related meshes. Its only job is to correctly store and draw said meshes.
  Each model has its own unique ID, and a name.

- The `Shader` class takes care of handling a pair of Vertex shader and Fragment shader.
  It also exposes a set of functions, like `set_mat4()` to set uniform variables.

- The `Skybox` class is used to render a cube and display a cubemap on it (While also converting the HDRI input file into a cubemap).

- The `Texture` class loads, stores, and puts a single texture into memory. Said texture is stored in a single `Mesh` and can/will be used in the rendering loop.

&nbsp;

**`src/utils`**

Set of classes not strictly related to the software logic.

- The `EngineTime` static class provides an easy way to access the delta EngineTime.

- The `ModelLoader` static class internally uses **_AssImp_** to get the all the meshes from a given filepath.  
  It then creates and returns a `Model`.

- The `Logger` static class is used to log errors or some operations status.

- The `IconModel` class displays a face with a texture that rotates toward the camera. It is used as a placeholder of where invisible objects, such as the main light source, are.

- The `Scene` class holds a `map<id, model>`, and a set of functions to operate on said data structure.

&nbsp;

**`src/main.cpp`**

The entry point of the app. An object of class `Engine` and one of class `Scene` are initialized and the main loop starts.

&nbsp;

Rendering pipeline:

1. `Engine` and `Scene` are initialized.
    - If `vr_mode` is **true**, `VRContext` is initializated.
2. The rendering loop starts.
3. `Engine::begin_frame()` is called.
4. Models are loaded in the frame buffer objects.
5. Models and skybox are rendered on the screen.
6. The engine listens for a click and sees if it matches with a model ID.
7. The UI is drawn.
8. Loads a new model or skybox if a change in the input variables is detected.
9. `Engine::end_frame()` is called.
