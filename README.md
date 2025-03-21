# Run_Local_AI_Model
This shows how one can run a AI model .gguf file locally on Linux

## Installation

**I'm running Amazon linux 2023 in WSL with NVIDIA RTX 2050 and Intel i5 12th Gen processor**


**Create virtual environemnt**
```
python -m venv /path/to/new/virtual/environment
```

Use following to activate virtual environment each time WSL restart 

```
source python_virtual_env/bin/activate
```


# Build ccache Amazon Linux
installing ccache can speed up compilation by caching build results. Here's how to install ccache on Amazon Linux 

### Build ccache from Source

**Install Dependencies**:
```
sudo yum groupinstall "Development Tools" -y
sudo yum install cmake git -y
```

**Download ccache Source Code:**
```
wget https://github.com/ccache/ccache/releases/download/v4.8.3/ccache-4.8.3.tar.gz
tar -xzf ccache-4.8.3.tar.gz
cd ccache-4.8.3
```

**Build and Install ccache:**
```
mkdir build
cd build
cmake ..
make
sudo make install
```

**Verify Installation:**
```
ccache --version
```

# Install CUDA toolkit

for my laptop ( with RTX 2050 and Intel i5 12th Gen ) , CUDA toolkit 12.x is supported

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


### Install Python Bindings:
If you want to use llama.cpp in Python, install the Python bindings:
```
pip install llama-cpp-python
```

## Run

Run using

```
python Load_AI_Model_gguf.py
```
