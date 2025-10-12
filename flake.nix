{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in
    {
      devShells.x86_64-linux.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          cmake
          sdl3
          shaderc
          vulkan-loader
        ];
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (with pkgs; [
          alsa-lib
          # dbus
          hidapi
          # ibus
          jack2
          # libdecor
          # libdrm
          # libGL
          # libpulseaudio
          # libsysprof-capture
          libusb1
          # libxkbcommon
          # mesa
          # pipewire
          # pkg-config
          sdl3
          # sndio
          vulkan-headers
          vulkan-loader
          vulkan-tools
          vulkan-tools-lunarg
          vulkan-validation-layers
          wayland
          wayland-protocols
          xorg.libX11
          xorg.libXcursor
          # xorg.libXext
          xorg.libXi
          # xorg.libXinerama
          # xorg.libXrandr
          # xorg.libXrender
          # xorg.libXtst
        ]);
        VULKAN_SDK = "${pkgs.vulkan-headers}";
        VK_LAYER_PATH = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d";
      };
    };
}
