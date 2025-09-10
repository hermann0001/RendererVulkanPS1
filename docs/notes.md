# Renderer Vulkan PS1 - Study Notes

## Roadmap
- Build: CMake + Ninja, preset Debug/Release, warning-as-errors in Debug.
- Window & surface: GLFW window + vkCreateSurfaceKHR.
- Instance + Debug: validation layers + VK_EXT_debug_utils messenger.
- GPU picking: discrete > integrated; features base (samplerAnisotropy opzionale).
- Queues: graphics + present (stessa o distinta family).
- Swapchain: format, extent, present mode (inizia con FIFO).
- Image views: per ogni swapchain image.
- Commands & Sync: pool, n frame-in-flight (2 o 3), semaphores/fences.
- Clear pass: command buffer minimal (acquire → begin → clear → end → submit → present).
- VMA: integrazione per buffer/immagini.
- Descriptor allocator: semplice bump allocator o pooling per tipi comuni.
- Pipeline builder: shader stages + fixed-function (viewport/scissor, raster, blend, depth).
- Shader toolchain: target .spv in build (CMake custom commands).
- PS1 features (fase 2): affine mapping, vertex snapping, 5-6-5/5-5-5 dithering, LOD/clip “grezzo”.

## Moduli
A strati dal più basso al più alto

### RV::Core
tipi base, RAII generici, configurazione build, ID/handles, piccoli container.

### RV::Utils
logger, assert, profili, piccoli helpers (hash, file io).

### RV::Platform
finestra & input (GLFW), timer, filesystem path.


### RV::RHI (Vulkan)
incapsula Vulkan con RAII, mantenendo API “thin” ma sicura.

Instance, Device, Queues, Swapchain, CommandPool, CommandBuffer, Sync (fences/semaphores), DescriptorPool/Set, Pipeline, ShaderModule, RenderPass/Framebuffer, Buffer, Image, Sampler.

Allocator basato su VMA per buffer/texture (fortemente consigliato). 

Bootstrap opzionale con vk-bootstrap per ridurre boilerplate di instance/device/swapchain. 

Swapchain segue struttura “present queue di immagini” come nella Vulkan Tutorial. 

Principi: ogni wrapper è RAII (distruttori rilasciano risorse), builders per pipeline/descriptor, zero logica “di gioco” qui.
Ispirazione: vkguide “vulkan_init_code” e architettura dei pass. 

### RV::Assets
caricamento e caching di Mesh, Texture, Shader (+ reflection info), Material.
Classi: AssetManager, ShaderManager, MeshLoader(OBJ/GLTF minimal), TextureLoader.

### RV::Scene
rappresentazione dei dati da disegnare (DOD semplice).
Classi/Strutture: Transform, Camera, Light, MeshInstance, SceneGraph molto leggero o anche solo liste flat + indici.

### RV::Renderer
orchestration dei render pass e scheduling dei comandi.
Classi:

FrameGraph/RenderGraph light (facoltativo) o, in MVP, Renderer con 2–3 pass fissi.

Pass: OpaquePass, TransparentPass, ShadowPass (come nel vkguide). 
Vulkan Guide

FrameContext (cmd buffers, per-frame descriptors, sync).

Culling (CPU), Uploader (staging + copy).

### RV::PS1
il “carattere” PS1, isolato e sostituibile.
Contenuti:

Raster impostazioni: affine-mapped textures (niente perspective-correct) per il tipico “wobble”, quantizzazione colore (ad es. 15/16-bit) e ordered dither su post.

Vertex jitter/snapping (quantizza spazio clip/schermo per look low-poly).

Pipeline PS1: uno GraphicsPipeline forward minimale con blending/alpha stile “anni ‘90”, niente PBR.

MaterialPS1: parametri semplici (tint, texture index, dither on/off).

Tenere questi effetti in un modulo a parte ti consente di cambiare stile (PS2-like, toon, ecc.) senza toccare Renderer/RHI.

### RV::UI
debug overlay (ImGui), toggles runtime (wireframe, dither, jitter), profiler.
Note: integrata su swapchain passando command buffers e render pass compatibili (pattern comune nei sample). 

### RV (Application)
lifecycle dell’app: create → run → shutdown.
Flow tipico: init Platform → RHI (via vk-bootstrap opzionale) → Assets → Renderer(+PS1) → loop (poll, update, record & submit, present). 

## 09/09/25 - Setup

Setup del progetto in VSCode
- Setup di **CMake** (build system generator) il quale ci aiuta a creare i file di progetto da dare poi al compilatore
- **Ninja** (build tool) è il software che compila i file lanciando il compilatore impostato (msvc/cl.exe , clang, g++)
- Link di librerie e SDK
    - Vulkan SDK -> api grafica
    - GLFW -> gestione finestre, input e context grafico
    - GLM -> matematica c++ per grafica 3D

Ho combattuto un po' con VSCode per il setup di IntelliSense ma ce l'ho fatta. Il problema ovviamente era un `$INCLUDE` (env. var.) che puntava ad una cartella vuota.

Lo stesso però non mi sembra convincere molto lo sviluppo **Vulkan/CMake/C++/MSVC** su VSCode, vedrò se passare a Visual Studio

## 10/09/25 - Logger
- Costruzione del sistema di logging, controllo granulare attraverso le macro sul livello di logging voluto. 
- Il logger sfrutta **C++20** per formattare le stringhe comodamente in stile `log("result is = {}", 2+2)` via `std::format...`
- Il logger utilizza *lock mutex* per thread safety, ogni operazione può continuare solo quando il thread acquisisce il lock sul mutex.

Vorrei e lo segno come **TODO** che la scrittura su file sia eseguita in batch quando il logger viene chiuso, cosi da non rallentare il software con scritture su disco


# Glossario

**RAII**: *Resource Acquisition Is Initialization* è un principio di programmazione, dove la gestione delle risorse viene legata al ciclo di vita di un oggetto. Quando un oggetto viene creato, acquisisce una risorsa, e quando l'oggetto esce dallo scope o viene distrutto, la riosrsa viene rilasciata automaticamente.