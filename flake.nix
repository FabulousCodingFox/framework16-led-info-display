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
        catch2_3
        makeWrapper
        bashInteractive
      ];
      nativeBuildInputs = [
        pkgs.pkg-config
      ];
    };
  };
}
