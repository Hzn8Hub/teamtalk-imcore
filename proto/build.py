#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Protocol Buffer 构建脚本
支持 Windows 和 Linux 平台
每次执行自动构建并同步到 include 和 src 目录
"""

import os
import sys
import shutil
import glob
import platform
import subprocess
from pathlib import Path


class ProtoBuildTool:
    def __init__(self):
        # 获取脚本所在目录
        self.script_dir = Path(__file__).parent.absolute()
        
        # 配置路径
        self.src_dir = self.script_dir
        self.gen_dir = self.script_dir / "gen"
        self.dest_header_dir = self.script_dir.parent / "include" / "teamtalk" / "imcore" / "ttidl"
        self.dest_source_dir = self.script_dir.parent / "src" / "teamtalk" / "imcore" / "ttidl"
        
        # protoc 路径（相对于脚本目录）
        protoc_bin = "protoc.exe" if platform.system() == "Windows" else "protoc"
        # teamtalk-imcore/proto -> teamtalk/.sdk/protobuf/bin/protoc
        self.protoc = self.script_dir.parent.parent / ".sdk" / "protobuf" / "bin" / protoc_bin
        
    def check_protoc(self):
        """检查 protoc 是否存在"""
        if not self.protoc.exists():
            print(f"错误: protoc 不存在!")
            print(f"请检查路径: {self.protoc}")
            return False
        return True
    
    def find_proto_files(self):
        """查找所有 proto 文件"""
        proto_files = []
        
        # 根目录下的 proto 文件
        proto_files.extend(glob.glob(str(self.src_dir / "*.proto")))
        
        return [Path(f) for f in proto_files]
    
    def run(self):
        """构建并同步 proto 文件"""
        print("=" * 60)
        print("Protocol Buffer 构建工具")
        print(f"平台: {platform.system()}")
        print("=" * 60)
        
        # 检查 protoc
        if not self.check_protoc():
            return False
        
        # 显示 protoc 路径
        print(f"\nprotoc 路径: {self.protoc}")
        print(f"工作目录: {self.script_dir}")
        
        # 查找 proto 文件
        proto_files = self.find_proto_files()
        if not proto_files:
            print("警告: 没有找到 proto 文件")
            return False
        
        print(f"\n找到 {len(proto_files)} 个 proto 文件:")
        for f in proto_files:
            print(f"  - {f.relative_to(self.script_dir)}")
        
        # 创建临时输出目录
        cpp_out_dir = self.gen_dir / "cpp"
        cpp_out_dir.mkdir(parents=True, exist_ok=True)
        
        # 生成 C++ 代码
        print("\n" + "=" * 60)
        print("生成 C++ 代码...")
        print("=" * 60)
        for proto_file in proto_files:
            print(f"  处理: {proto_file.name}")
            cmd = f'"{self.protoc}" -I="{self.src_dir}" --cpp_out="{cpp_out_dir}" "{proto_file}"'
            ret = os.system(cmd)
            if ret != 0:
                print(f"错误: 生成 C++ 代码失败 ({proto_file.name})")
                return False
        
        print("生成完成!")
        
        # 同步到目标目录
        print("\n" + "=" * 60)
        print("同步生成的文件...")
        print("=" * 60)
        
        # 清理旧文件
        if self.dest_header_dir.exists():
            shutil.rmtree(self.dest_header_dir)
        if self.dest_source_dir.exists():
            shutil.rmtree(self.dest_source_dir)
        
        # 创建目标目录
        self.dest_header_dir.mkdir(parents=True, exist_ok=True)
        self.dest_source_dir.mkdir(parents=True, exist_ok=True)
        
        # 查找生成的文件
        header_files = list(cpp_out_dir.glob("*.pb.h"))
        source_files = list(cpp_out_dir.glob("*.pb.cc"))
        
        # 复制头文件
        print(f"\n复制头文件到: {self.dest_header_dir}")
        for header in header_files:
            dest = self.dest_header_dir / header.name
            shutil.copy2(header, dest)
            print(f"  ✓ {header.name}")
        
        # 复制源文件
        print(f"\n复制源文件到: {self.dest_source_dir}")
        for source in source_files:
            dest = self.dest_source_dir / source.name
            shutil.copy2(source, dest)
            print(f"  ✓ {source.name}")
        
        print(f"\n同步完成!")
        print(f"  头文件: {len(header_files)} 个")
        print(f"  源文件: {len(source_files)} 个")
        
        # 清理临时目录
        if self.gen_dir.exists():
            shutil.rmtree(self.gen_dir)
            print(f"\n已清理临时目录")
        
        return True


def main():
    tool = ProtoBuildTool()
    
    success = tool.run()
    
    if success:
        print("\n" + "=" * 60)
        print("所有任务完成!")
        print("=" * 60)
    else:
        print("\n构建失败!")
        sys.exit(1)


if __name__ == "__main__":
    main()
