{
  pkgs ? import <nixpkgs> { },
}:
let
  inherit (pkgs) lib;
in

pkgs.mkShell {
  packages = with pkgs; [
    # Toolchain
    gcc14
    cmake
    ninja
    pkg-config
    git

    # OpenGL
    libGL
    libGLU
    mesa

    # X11 (GLFW)
    libx11
    libxrandr
    libxinerama
    libxcursor
    libxi
    libxext

    # Wayland (GLFW)
    wayland
    wayland-scanner
    wayland-protocols
    libxkbcommon

    # CPR / networking
    curl.dev
    openssl.dev
    zlib
  ];

  # Make Nix store .so files visible to the dynamic linker at runtime
  LD_LIBRARY_PATH = lib.makeLibraryPath (
    with pkgs;
    [
      wayland
      libGL
      libxkbcommon
    ]
  );

  shellHook = ''
    echo "Lazap dev shell ready!"
    echo "Build with:"
    echo "  cmake -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DCPR_USE_SYSTEM_CURL=ON -DCURL_USE_LIBPSL=OFF"
    echo "  cmake --build build -j\$(nproc)"
  '';
}
