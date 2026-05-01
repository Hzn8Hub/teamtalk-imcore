#!/usr/bin/env python3
"""Format C/C++ sources and proto files with clang-format using repo .clang-format."""

import os
import shutil
import subprocess
import sys
from pathlib import Path
from typing import List


FORMAT_DIRS = ("src", "include", "test", "proto")
SOURCE_SUFFIXES = {".cc", ".cpp", ".cxx", ".h", ".hpp", ".hh", ".proto"}


def collect_source_files(root: Path) -> List[str]:
    files: List[str] = []

    for relative_dir in FORMAT_DIRS:
        base_dir = root / relative_dir
        if not base_dir.is_dir():
            continue

        for current_root, dirnames, filenames in os.walk(base_dir):
            # Skip third-party or generated trees to avoid unrelated diffs.
            dirnames[:] = [dirname for dirname in dirnames if dirname not in ("gen", "ttidl")]

            for filename in filenames:
                file_path = Path(current_root) / filename
                if file_path.suffix in SOURCE_SUFFIXES:
                    files.append(str(file_path))

    files.sort()
    return files


def main() -> int:
    root = Path(__file__).resolve().parent
    clang_format = os.environ.get("CLANG_FORMAT", "clang-format")

    if shutil.which(clang_format) is None:
        print(
            f"error: '{clang_format}' not found. Install LLVM clang-format or set CLANG_FORMAT.",
            file=sys.stderr,
        )
        return 1

    files = collect_source_files(root)
    if not files:
        print(f"no source files matched under {' '.join(FORMAT_DIRS)}")
        return 0

    # 分离 proto 文件和 C++ 文件
    proto_files = [f for f in files if f.endswith('.proto')]
    cpp_files = [f for f in files if not f.endswith('.proto')]
    
    formatted_count = 0
    
    # 格式化 C++ 文件
    if cpp_files:
        subprocess.run([clang_format, "-i", "--style=file", *cpp_files], check=True)
        formatted_count += len(cpp_files)
        print(f"formatted {len(cpp_files)} C/C++ file(s) with .clang-format")
    
    # 格式化 proto 文件（使用 clang-format 的 proto 支持）
    if proto_files:
        subprocess.run([clang_format, "-i", "--style=file", *proto_files], check=True)
        formatted_count += len(proto_files)
        print(f"formatted {len(proto_files)} proto file(s) with .clang-format")
    
    if formatted_count == 0:
        print(f"no source files matched under {' '.join(FORMAT_DIRS)}")
    
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
