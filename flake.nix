{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in
    {
      devShells.x86_64-linux.default = pkgs.mkShell {
        packages = with pkgs; [
          cmake
          glfw
          freetype
          vulkan-headers
          vulkan-loader
          vulkan-validation-layers
          vulkan-tools
          shaderc
          vulkan-tools-lunarg
        ];
        buildInputs = with pkgs; [
          # pkgs.libxkbcommon
          # pkgs.pkg-config
          # pkgs.shaderc
          # pkgs.spirv-tools
          # pkgs.vulkan-tools
          # pkgs.vulkan-validation-layers
          # pkgs.wayland
          # pkgs.xorg.libX11
          # pkgs.xorg.libXcursor
          # pkgs.xorg.libXinerama
          # pkgs.xorg.libXrandr
          # pkgs.xorg.libXi
          glfw
          freetype
        ];
        # LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (with pkgs; [
        #   # libglvnd
        #   # libxkbcommon
        #   # openal
        #   # vulkan-tools
        #   # vulkan-validation-layers
        #   # wayland
        #   # xorg.libXinerama
        # ]);
        LD_LIBRARY_PATH = "${pkgs.glfw}/lib:${pkgs.freetype}/lib:${pkgs.vulkan-loader}/lib:${pkgs.vulkan-validation-layers}/lib";
        VULKAN_SDK = "${pkgs.vulkan-headers}";
        VK_LAYER_PATH = "${pkgs.vulkan-validation-layers}/share/vulkan/explicit_layer.d";
      };
    };
}

