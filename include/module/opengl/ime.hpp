/*
 * GLFW Wayland IME Integration (zwp_text_input_v3)
 * Status: Prototype
 * Requirements:
 * - Wayland display environment (e.g., Hyprland)
 * - GLFW (linked with X11 disabled and Wayland enabled)
 * - fcitx5 running
 */
#include "text-input-unstable-v3-client-protocol.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>

struct wl_display *wl_display_ptr = NULL;
struct wl_registry *registry = NULL;
struct zwp_text_input_manager_v3 *text_input_mgr = NULL;
struct zwp_text_input_v3 *text_input = NULL;
struct wl_surface *wl_surface_ptr = NULL;

void *glfw_native_get_wl_display();
void *glfw_native_get_wl_surface(GLFWwindow *window);

// --- Event Callbacks ---

static void handle_preedit_string(void *data,
                                  struct zwp_text_input_v3 *text_input,
                                  const char *text, int32_t cursor_begin,
                                  int32_t cursor_end) {
  printf("[Preedit] %s\n", text);
}

static void handle_commit_string(void *data,
                                 struct zwp_text_input_v3 *text_input,
                                 const char *text) {
  printf("[Commit] %s\n", text);
}

static void handle_delete_surrounding_text(void *data,
                                           struct zwp_text_input_v3 *text_input,
                                           uint32_t before_length,
                                           uint32_t after_length) {
  printf("[Delete Surrounding] before=%u, after=%u\n", before_length,
         after_length);
}

static void handle_done(void *data, struct zwp_text_input_v3 *text_input) {
  printf("[IME Done]\n");
}

static const struct zwp_text_input_v3_listener ime_listener = {
    .preedit_string = handle_preedit_string,
    .commit_string = handle_commit_string,
    .delete_surrounding_text = handle_delete_surrounding_text,
    .done = handle_done,
};

// --- Registry Setup ---

static void registry_global(void *data, struct wl_registry *registry,
                            uint32_t name, const char *interface,
                            uint32_t version) {
  if (strcmp(interface, "zwp_text_input_manager_v3") == 0) {
    text_input_mgr = wl_registry_bind(registry, name,
                                      &zwp_text_input_manager_v3_interface, 1);
  }
}

static void registry_global_remove(void *data, struct wl_registry *registry,
                                   uint32_t name) {
  (void)data;
  (void)registry;
  (void)name;
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

// --- Init IME ---

void init_wayland_ime(GLFWwindow *window) {
  wl_display_ptr = glfw_native_get_wl_display();
  wl_surface_ptr = glfw_native_get_wl_surface(window);

  if (!wl_display_ptr || !wl_surface_ptr) {
    fprintf(stderr, "[Error] GLFW native Wayland handles unavailable.\n");
    return;
  }

  registry = wl_display_get_registry(wl_display_ptr);
  wl_registry_add_listener(registry, &registry_listener, NULL);
  wl_display_roundtrip(wl_display_ptr);

  if (!text_input_mgr) {
    fprintf(stderr, "[Error] zwp_text_input_manager_v3 not available.\n");
    return;
  }

  text_input = zwp_text_input_manager_v3_get_text_input(text_input_mgr);
  zwp_text_input_v3_add_listener(text_input, &ime_listener, NULL);
  zwp_text_input_v3_enable(text_input, wl_surface_ptr);
}

int main() {
  if (!glfwInit()) {
    fprintf(stderr, "[Error] GLFW init failed.\n");
    return -1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window =
      glfwCreateWindow(640, 480, "GLFW IME Example", NULL, NULL);
  if (!window) {
    fprintf(stderr, "[Error] Window creation failed.\n");
    glfwTerminate();
    return -1;
  }

  init_wayland_ime(window);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    wl_display_dispatch_pending(wl_display_ptr);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

// Note: You'll need to provide the `text-input-unstable-v3.xml` and generate
// the corresponding protocol .h/.c files using wayland-scanner, and link with
// `wayland-client`, `glfw`, and ensure `GLFW_USE_WAYLAND` is defined during
// compilation.

// Also, link against libglfw compiled with Wayland enabled and X11 disabled.
