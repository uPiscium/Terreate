{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
      gccVersion = pkgs.gcc.cc.version;
      gccTarget = pkgs.stdenv.hostPlatform.config;
    in
    {
      devShells.x86_64-linux.default = pkgs.mkShell.override {
        stdenv = pkgs.clangStdenv;
      } {
        buildInputs = with pkgs; [
          cmake
          ninja

          alsa-lib
          hidapi
          jack2
          libusb1
          libxkbcommon
          shaderc
          vulkan-headers
          vulkan-loader
          vulkan-tools
          vulkan-tools-lunarg
          vulkan-validation-layers
          wayland
          wayland-protocols
          wayland-scanner
          xorg.libX11
          xorg.libxcb
          xorg.libXcursor
          xorg.libXi
          xorg.libXi
          xorg.libXrandr
          xorg.libXinerama
        ];
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (with pkgs; [
          alsa-lib
          hidapi
          jack2
          libusb1
          libxkbcommon
          vulkan-headers
          vulkan-loader
          vulkan-tools
          vulkan-tools-lunarg
          vulkan-validation-layers
          wayland
          wayland-protocols
          wayland-scanner
          xorg.libX11
          xorg.libxcb
          xorg.libXcursor
          xorg.libXi
          xorg.libXrandr
          xorg.libXinerama
          libxkbcommon
        ]);
        VULKAN_SDK = "${pkgs.vulkan-headers}";
        VK_LAYER_PATH = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d";
        shellHook = ''
          export CPLUS_INCLUDE_PATH="${pkgs.gcc.cc}/include/c++/${gccVersion}/${gccTarget}:${pkgs.gcc.cc}/include/c++/${gccVersion}:${pkgs.glibc.dev}/include:$CPLUS_INCLUDE_PATH"
          export C_INCLUDE_PATH="${pkgs.glibc.dev}/include:$C_INCLUDE_PATH"
        '';
      };
    };
}
