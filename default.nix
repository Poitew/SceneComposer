let 
    pkgs = import <nixpkgs> { };

    imgui-docking = pkgs.imgui.overrideAttrs (oldAttrs: {
        version = "1.92.5-docking";
        src = pkgs.fetchFromGitHub {
            owner = "ocornut";
            repo = "imgui";
            rev = "docking";
            hash = "sha256-Y7vquGOYs2+H37T7JP8EVGM0WfkozSrSiWAnwl/vuMk="; 
        };

        buildInputs = (oldAttrs.buildInputs or []) ++ (with pkgs.xorg; [
            libX11
            libXcursor
            libXinerama
            libXext
            libXrandr
        ]);
    });
in 
    pkgs.stdenv.mkDerivation rec {
        name = "composer";
        src = ./.;

        nativeBuildInputs =  with pkgs; [
            wrapGAppsHook3
            python313
            python313Packages.glad2
        ];

        buildInputs = with pkgs; [
            gtk3

            glfw
            glm
            stb
            imgui-docking
            assimp
            nativefiledialog-extended

            openxr-loader
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