{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
  let
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
  in
  {
    devShells.x86_64-linux.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        cmake
        gdb
        clang
        ninja
        zip
        unzip
        curl
        git
        libusb1
        hidapi
        spdlog
        clang-tools

        # Wrapper
        makeWrapper
        bashInteractive

        # QT6
        qt6.full
        qt6.qtbase
        qt6.wrapQtAppsHook
      ];
      nativeBuildInputs = [
        pkgs.pkg-config
      ];
      shellHook = ''
        export QT_QPA_PLATFORM=wayland
        bashdir=$(mktemp -d)
        makeWrapper "$(type -p bash)" "$bashdir/bash" "''${qtWrapperArgs[@]}"
        exec "$bashdir/bash"
      '';
    };
  };
}
