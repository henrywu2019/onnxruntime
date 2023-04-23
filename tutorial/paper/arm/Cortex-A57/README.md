fuheng@fuheng-desktop:~/onnxruntime/tutorial/paper/fma$ bash run_release_ort.sh 
/home/fuheng/onnxruntime/build/Linux/Release//libonnxruntime_common.a(cpuid_info.cc.o): In function `onnxruntime::CPUIDInfo::ArmLinuxInit()':
cpuid_info.cc:(.text._ZN11onnxruntime9CPUIDInfo12ArmLinuxInitEv+0x28): undefined reference to `cpuinfo_initialize'
cpuid_info.cc:(.text._ZN11onnxruntime9CPUIDInfo12ArmLinuxInitEv+0x80): undefined reference to `cpuinfo_get_uarchs_count'
cpuid_info.cc:(.text._ZN11onnxruntime9CPUIDInfo12ArmLinuxInitEv+0x84): undefined reference to `cpuinfo_isa'
cpuid_info.cc:(.text._ZN11onnxruntime9CPUIDInfo12ArmLinuxInitEv+0x8c): undefined reference to `cpuinfo_isa'
cpuid_info.cc:(.text._ZN11onnxruntime9CPUIDInfo12ArmLinuxInitEv+0xa8): undefined reference to `cpuinfo_get_cores_count'
cpuid_info.cc:(.text._ZN11onnxruntime9CPUIDInfo12ArmLinuxInitEv+0x13c): undefined reference to `cpuinfo_get_processor'
collect2: error: ld returned 1 exit status


