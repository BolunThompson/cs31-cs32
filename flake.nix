{
  description = "CS 31";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in
    {
      devShells.x86_64-linux.default = pkgs.mkShell {
          name = "CS 31 Shell";
          nativeBuildInputs = with pkgs; with llvmPackages_19; [
            clang
          ];
          packages = with pkgs; with llvmPackages_19; [
            lldb
            rr
            binutils
            git
            clang-tools
          ];
        };
    };
}
