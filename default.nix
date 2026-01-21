let 
    pkgs = import <nixpkgs> { };
in 
    pkgs.stdenv.mkDerivation rec {
        name = "composer";
        src = ./.;

        nativeBuildInputs =  with pkgs; [
            wrapGAppsHook3
        ];

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

        buildPhase = ''
            runHook preBuild
            make
            runHook postBuild
        '';

        installPhase = ''
            runHook preInstall

            mkdir -p $out/bin
            cp ./bin/composer.out $out/bin 

            runHook postInstall
        '';
    }