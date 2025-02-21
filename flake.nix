{
  description = "CS 31";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-darwin" ];
    in {
      devShell = builtins.listToAttrs (map (system: {
        name = system;
        value = let
          pkgs = import nixpkgs { inherit system; };
          llvm = pkgs.llvmPackages_19;
        in pkgs.mkShell {
          name = "CS 31 Shell";
          nativeBuildInputs = with pkgs; with llvm; [
            clang
            meson
            ninja
          ];
          packages = with pkgs; with llvm; (if system == "aarch64-darwin" then [
            helix
            lldb
            binutils
            git
            clang-tools
          ] else [
            helix
            lldb
            rr
            binutils
            git
            clang-tools
            valgrind
          ]);
        };
      }) systems);
    };
}
