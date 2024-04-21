{
  description = "新歓ロボキット";

  inputs = {
    # https://github.com/NixOS/nixpkgs/pull/237313
    nixpkgs.url = "github:ppenguin/nixpkgs/refactor-platformio-fix-ide";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        inherit (pkgs) lib;
        pythonWithPlatformio = pkgs.python3.withPackages (ps: with ps; [ platformio ]);
      in
      {
        devShells.default = with pkgs; mkShell {
          packages = [ platformio-core pythonWithPlatformio clang-tools_16 mdbook jq ];
        };
      });
}
