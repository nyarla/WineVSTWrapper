{ pkgs ? import <nixpkgs> { } }:

with pkgs;

mkShell rec {
  name = "wine-vst-wrapper";
  nativeBuildInputs = with pkgsCross.mingwW64; [ buildPackages.gcc ];
  NIX_INCLUDE_PATH = "${pkgsCross.mingwW64.windows.mingw_w64_headers}/include";
}
