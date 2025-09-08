# Renderer Vulkan stile PS1 (low-poly)
Confronto interattivo **Moderno vs PS1**: stessa scena, due pipeline selezionabili in runtime.
> Obiettivo: capire *perché* certe scelte grafiche generano l’estetica PS1 (affine mapping, bassa risoluzione, quantizzazione colore, dithering, vertex snap, ecc.).

---

## Requisiti (Windows 11)

- **Visual Studio 2022 Build Tools**  
  - Workload: *Desktop development with C++*  
  - Include: **MSVC v143**, **Windows 10/11 SDK**, **C++ CMake tools for Windows**
- **Vulkan SDK** (LunarG) — durante l’installazione lascia attivo: glslc/shaderc, vkconfig, vulkaninfo
    - **GLFW**: https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    - **GLM**: https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
- **VS Code** + estensioni:
  - *CMake Tools* (ms-vscode.cmake-tools)
  - *C/C++* (ms-vscode.cpptools)
- (Opz.) **RenderDoc** per debug GPU

> Non serve installare Ninja separatamente: CMake Tools userà quello integrato in Visual Studio.


---

## Struttura del repository

```
.vscode/
    settings.json
    launch.json
/assets/            # modelli e texture (opz. Git LFS)
/build/             # build files di CMake
    /debug/
    /release/
/docs/              # documenti, immagini, note
/lib/               # librerie vendored (sorgenti)
    /glfw/
    /glm/
    ...
/shaders/           # *.vert, *.frag (GLSL -> SPIR-V in futuro)
/src/
CMakeLists.txt
CMakePresets.json
```

---

## Come configurare e lanciare (VS Code)

1. **Apri la cartella** del progetto in VS Code.
2. In basso (barra CMake) seleziona il preset:
   - `msvc-debug` (Debug)
   - `msvc-release` (Release)
3. **Build** → **Run/Debug**.  

In alternativa da terminale:
```bat
cmake --preset msvc-debug
cmake --build --preset build-debug
.\build\debug\RendererVulkanPS1.exe

cmake --preset msvc-release
cmake --build --preset build-release
.\build\release\RendererVulkanPS1.exe
