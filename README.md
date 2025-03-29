# Run_Local_AI_Model
This shows how one can run a AI model .gguf file locally on Linux

## Installation

**I'm running Ubuntu 24.04 and Amazon Linux in WSL with NVIDIA RTX 2050 and Intel i5 12th Gen processor**


**Create virtual environemnt**
```
python -m venv python_virtual_env
```

Use following to activate virtual environment each time WSL restart 

```
source python_virtual_env/bin/activate
```

# Install dependencies

```
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121

```

# Install DKMS for Ubuntu 
DKMS (Dynamic Kernel Module Support) package is missing on your system. DKMS is required to build and install kernel modules, such as the NVIDIA driver.

```
sudo apt update
sudo apt install dkms
```

Verify

```
dkms --version
```

# Build ccache for Amazon linux
installing ccache can speed up compilation by caching build results. Here's how to install ccache on Amazon Linux
```
 yum groupinstall "Development Tools" -y
 yum install cmake git -y
```

**Download ccache Source Code:**
```
wget https://github.com/ccache/ccache/releases/download/v4.8.3/ccache-4.8.3.tar.gz
tar -xzf ccache-4.8.3.tar.gz
cd ccache-4.8.3
```

**Build and Install ccache:**

By default installed in **/usr/local/bin**. If you want to prefer diffrent path use **cmake .. -DCMAKE_INSTALL_PREFIX=/path/you_prefer**


```
mkdir build
cd build
cmake ..
make
make install   
```

**Verify Installation:**
```
ccache --version
```


# Install CUDA toolkit

for my laptop ( with RTX 2050 and Intel i5 12th Gen ) , CUDA toolkit 12.x is supported

**For Ubuntu**
```
wget https://developer.download.nvidia.com/compute/cuda/12.8.1/local_installers/cuda_12.8.1_570.124.06_linux.run
sudo sh cuda_12.8.1_570.124.06_linux.run

```

You should see this message

          ===========
          = Summary =
          ===========
          
          Driver:   Not Selected
          Toolkit:  Installed in /usr/local/cuda-12.8/
          
          Please make sure that
           -   PATH includes /usr/local/cuda-12.8/bin
           -   LD_LIBRARY_PATH includes /usr/local/cuda-12.8/lib64, or, add /usr/local/cuda-12.8/lib64 to /etc/ld.so.conf and run ldconfig as root
          
          **To uninstall the CUDA Toolkit, run cuda-uninstaller in /usr/local/cuda-12.8/bin**
          ***WARNING: Incomplete installation! This installation did not install the CUDA Driver. A driver of version at least 570.00 is required for CUDA 12.8 functionality to work.
          To install the driver using this installer, run the following command, replacing <CudaInstaller> with the name of this run file:
              sudo <CudaInstaller>.run --silent --driver
          
          Logfile is /var/log/cuda-installer.log




**For Amazon linux**

```
wget https://developer.download.nvidia.com/compute/cuda/12.8.0/local_installers/cuda-repo-amzn2023-12-8-local-12.8.0_570.86.10-1.x86_64.rpm
rpm -i cuda-repo-amzn2023-12-8-local-12.8.0_570.86.10-1.x86_64.rpm
dnf clean all
dnf -y install cuda-toolkit-12-8
```



**Verify Installtion**
```
nvcc --version
```

**If command not found error occurs follow steps**

**Step 1:** Locate nvcc

The nvcc compiler is part of the CUDA Toolkit and is usually installed in /usr/local/cuda-12.8/bin. Let's verify this:
```
ls /usr/local/cuda-12.8/bin/nvcc
```

If the file exists, proceed to the next step. If not, double-check the CUDA installation path.

**Step 2:** Add CUDA to Your PATH

To make nvcc accessible, you need to add the CUDA binary directory to your PATH. Run the following commands:

```
export PATH=/usr/local/cuda-12.8/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda-12.8/lib64:$LD_LIBRARY_PATH
```

**Step 3:** Verify nvcc

Now, check if nvcc is available:

```
nvcc --version
```

You should see output like:

     nvcc: NVIDIA (R) Cuda compiler driver
     Copyright (c) 2005-2023 NVIDIA Corporation
     Built on ...
     Cuda compilation tools, release 12.8, V12.8.x
     Build cuda_12.8.r12.8/compiler.35404655_0

**Step 4**: Make the Changes Permanent

To ensure the PATH and LD_LIBRARY_PATH changes persist across sessions, add the export commands to your shell configuration file.
For bash:

**Edit ~/.bashrc:**

```
nano ~/.bashrc
```

Add these lines at the end:

```
export PATH=/usr/local/cuda-12.8/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda-12.8/lib64:$LD_LIBRARY_PATH
```

Save and exit (Ctrl+O, Enter, Ctrl+X), then reload the configuration:

```
source ~/.bashrc
```

**After restarting wsl and running 'nvcc --version' still shows error then just use 'source ~/.bashrc' again after making sure bashrc file has changes we made**

# Build llama.cpp locally

**To get the Code:**

```bash
git clone https://github.com/ggml-org/llama.cpp
cd llama.cpp
mkdir buildLlama
cd buildLlama
```

The following sections describe how to build with different backends and options.

### GPU Build

Build llama.cpp using `CMake`:

By default  the CUDA compiler (nvcc) is targeting multiple GPU architectures during the build process. This is normal when building CUDA-enabled applications, as it ensures compatibility with a wide range of NVIDIA GPUs

Why Multiple Architectures?:

    The build process generates code for multiple architectures to ensure compatibility with a wide range of GPUs.

    This is known as fatbin (fat binary) generation.

Is This a Problem?

    No, this is not a problem. It’s a standard part of the CUDA build process. However, if you want to optimize the build for your specific GPU,
    you can limit the architectures to the ones supported by your GPU.

**Step 1:** Find Your GPU’s Compute Capability
Run the following command to check your GPU’s compute capability
```
nvidia-smi --query-gpu=compute_cap --format=csv
```

Example output 
> compute_cap
> 8.6


**Step 2:** Specify the Compute Capability

```bash
cmake .. -B build -DGGML_CUDA=ON -DCMAKE_CUDA_ARCHITECTURES=86
cmake --build build --config Release -j 4
```

**Step 3**: install it

```
cmake --install build --prefix <path_to_install>
```


#### Install Python Bindings: (optional)
Don't use this, because of problems in sub process of python. **INSTEAD use Linux script** 
If you want to use llama.cpp in Python, install the Python bindings:
```
CUDACXX=/usr/local/cuda-12.8/bin/nvcc CMAKE_ARGS="-DGGML_CUDA=on -DCMAKE_CUDA_ARCHITECTURES=86" pip install llama-cpp-python --no-cache-dir
```

#### Run

We will create a WSL process to run AI model through powershell

From base directory run

```
.\Powershell_script\Run_AI_memory.ps1
```



For Python binding (**Not used** )
Run using

```
python Load_AI_Model_gguf.py
```

If python binding is properly installed with CUDA support then after running we get name of GPU 

![image](https://github.com/user-attachments/assets/1997667e-4f7a-45a5-b5d3-4279b51d4a57)

