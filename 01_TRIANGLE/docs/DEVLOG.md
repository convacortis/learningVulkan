# Dev Log — VulkanTut / Engine

A running record of problems hit, solutions found, and small wins. For future-me, interview prep, and sanity.

---

## 2026-03-22

### Problem: GLFW window not displaying on Fedora
**What happened:** Window object was created, main loop was running (confirmed via log spam), no crash — but nothing appeared on screen.

**Root cause:** FetchContent pulled GLFW from source and built it with no display backend. `glfwGetPlatform()` returned `0x00060003` (Null platform) — not X11 (`0x00060002`) or Wayland (`0x00060004`). CMake couldn't find the required headers automatically so GLFW silently compiled as headless.

**Fix:** Force X11 backend via CMake cache flags before `FetchContent_MakeAvailable(glfw)`:
```cmake
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
```
Then clean rebuild: `rm -rf build/* && cmake .. && cmake --build .`

**Why FORCE matters:** Without it, cached values from a previous configure run silently win. FORCE overwrites regardless of cache state.

**Also installed:** `libX11-devel libXrandr-devel libXi-devel libXinerama-devel libXcursor-devel` via dnf.

---

### Win: Vulkan instance created and destroyed cleanly
First real Vulkan code — `VkApplicationInfo`, `VkInstanceCreateInfo`, `vkCreateInstance`, `vkDestroyInstance` in cleanup. No validation errors. GLFW required extensions passed through correctly via `glfwGetRequiredInstanceExtensions`.

**Note to self:** `sType` must be `VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO` — accidentally used `VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO` and it still worked, but that's UB. Don't do that.

---
