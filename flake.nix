{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in
    {
      devShells.x86_64-linux.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          pkg-config
          cmake
          wayland
          alsa-lib
          hidapi
          ibus
          dbus
          jack2
          libdecor
          libGL
          libpulseaudio
          libusb1
          libsysprof-capture
          libdrm
          xorg.libX11
          xorg.libXcursor
          xorg.libXinerama
          xorg.libXrandr
          xorg.libXi
          xorg.libXext
          xorg.libXrender
          xorg.libXtst
          # libxss
          libxkbcommon
          mesa
          ninja
          pipewire
          sndio
          # vulkan-driver
          # vulkan-headers
          wayland
          wayland-protocols
        ];
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (with pkgs; [
          pkg-config
          cmake
          wayland
          alsa-lib
          hidapi
          ibus
          dbus
          jack2
          libdecor
          libGL
          libpulseaudio
          libusb1
          libsysprof-capture
          libdrm
          xorg.libX11
          xorg.libXcursor
          xorg.libXinerama
          xorg.libXrandr
          xorg.libXi
          xorg.libXext
          xorg.libXrender
          xorg.libXtst
          # libxss
          libxkbcommon
          mesa
          ninja
          pipewire
          sndio
          # vulkan-driver
          # vulkan-headers
          wayland
          wayland-protocols
        ]);
      };
    };
}
