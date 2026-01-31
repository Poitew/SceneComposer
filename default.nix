let 
    pkgs = import <nixpkgs> { };
in 
    pkgs.stdenv.mkDerivation rec {
        name = "composer";
        src = ./.;

        nativeBuildInputs =  with pkgs; [
            wrapGAppsHook3
        ];

        imgui-docking = pkgs.imgui.overrideAttrs (oldAttrs: {
            version = "1.92.5-docking";
            src = pkgs.fetchFromGitHub {
                owner = "ocornut";
                repo = "imgui";
                rev = "docking";
                hash = "sha256-vSmZO40k5gO5Db2iKa8uNnYHtGzfmGAPHnhU22E4/rg="; 
            };

            buildInputs = (oldAttrs.buildInputs or []) ++ (with pkgs.xorg; [
                libX11
                libXcursor
                libXinerama
                libXext
                libXrandr
            ]);
        });

        buildInputs = with pkgs; [
                gcc
                clang-tools

                gtk3

                glfw
                glm
                python313Packages.glad2
                stb
                imgui-docking
                assimp
                nativefiledialog-extended
            ];

        buildPhase = ''
            runHook preBuild
            make clean
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