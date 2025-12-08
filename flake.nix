{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
      gccVersion = pkgs.gcc.cc.version;
      gccTarget = pkgs.stdenv.hostPlatform.config;
    in
    {
      # devShells.x86_64-linux.default = pkgs.mkShell {
      devShells.x86_64-linux.default = pkgs.mkShell.override {
        stdenv = pkgs.clangStdenv;
      } {
        buildInputs = with pkgs; [
          cmake
          sdl3
          shaderc
          vulkan-loader
        ];
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (with pkgs; [
          alsa-lib
          glibc
          hidapi
          jack2
          libusb1
          sdl3
          vulkan-headers
          vulkan-loader
          vulkan-tools
          vulkan-tools-lunarg
          vulkan-validation-layers
          wayland
          wayland-protocols
          xorg.libX11
          xorg.libXcursor
          xorg.libXi
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
