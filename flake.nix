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
      };
    };
}
