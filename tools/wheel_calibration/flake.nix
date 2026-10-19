{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-26.05";
  };

  outputs = {self , nixpkgs, ...}: let
    system = "x86_64-linux";
  in {
    devShells."${system}".default = let
      pkgs = import nixpkgs { inherit system; };
    in pkgs.mkShell {

      # Here is where you will add all the libraries required by your native modules
      # You can use the following one-liner to find out which ones you need.
      # Just make sure you have `gcc` installed.
      # `find .venv/ -type f -name "*.so" | xargs ldd | grep "not found" | sort | uniq`
      NIX_LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
        pkgs.stdenv.cc.cc # libstdc++
        pkgs.stdenv.cc.cc.lib
        pkgs.zlib # libz (for numpy)
        pkgs.libx11 # matplotlib
        #libstdc++.so.6
        #libz.so.1
      ];

      NIX_LD = pkgs.lib.fileContents "${pkgs.stdenv.cc}/nix-support/dynamic-linker";
      LD_LIBRARY_PATH = "${pkgs.stdenv.cc.cc.lib}/lib";

      packages = with pkgs; [
        uv
      ];
    };
  };
}
