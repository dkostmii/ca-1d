{ stdenv, pkgs, ... }:

stdenv.mkDerivation {
  pname = "ca-1d";
  version = "0.0.1";
  buildInputs = [  ];

  src = ./.;

  buildPhase = ''
    make
  '';

  installPhase = ''
    mkdir -p $out/bin
    cp build/release/ca-1d $out/bin/
  '';

  meta = with pkgs.lib; {
    description = "Elementary cellular automata CLI, written in C.";
    license = licenses.gpl3;
    platforms = platforms.all;
    homepage = "https://github.com/dkostmii/ca-1d";
  };
}
