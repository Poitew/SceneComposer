{
    description = "3D Scene Composer written in C++";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    };

    outputs = { self, nixpkgs }: let 
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
    in {
        devShells.${system}.default = pkgs.mkShell {
            strictDeps = true;

            buildInputs = with pkgs; [
                gcc
                clang-tools

                gtk3

                glfw
                glm
                python313Packages.glad2
                stb
                imgui
                assimp
                nativefiledialog-extended
            ];

            shellHook = ''
                export XDG_DATA_DIRS=$GSETTINGS_SCHEMAS_PATH

                if [ ! -d glad/include ]; then
                    mkdir -p glad
                    python -m glad --api gl:core=4.6 --out-path glad c
                fi

                if [ ! -f .clang-format ]; then
                    echo "Creating .clang-format..."
                    clang-format -style=Google -dump-config > .clang-format
                fi
            '';
            
        };
    };
}