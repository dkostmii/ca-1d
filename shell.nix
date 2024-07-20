{ pkgs ? import <nixpkgs> {} }:

let
  ca-1d = import ./default.nix pkgs;
in 
pkgs.mkShell {
  buildInputs = [
    pkgs.python311
    pkgs.python311Packages.colored
    ca-1d
  ];
}
