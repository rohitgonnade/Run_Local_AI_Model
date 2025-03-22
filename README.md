# Run_Local_AI_Model
This shows how one can run a AI model .gguf file locally on Linux

## Installation

**I'm running Ubuntu 24.04 in WSL with NVIDIA RTX 2050 and Intel i5 12th Gen processor**


**Create virtual environemnt**
```
python -m venv /path/to/new/virtual/environment
```

Use following to activate virtual environment each time WSL restart 

```
source python_virtual_env/bin/activate
```

# Install dependencies

```
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121

```

# Install DKMS
DKMS (Dynamic Kernel Module Support) package is missing on your system. DKMS is required to build and install kernel modules, such as the NVIDIA driver.

```
sudo apt update
sudo apt install dkms
```

Verify

```
dkms --version
```

# Install CUDA toolkit

for my laptop ( with RTX 2050 and Intel i5 12th Gen ) , CUDA toolkit 12.x is supported

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

# Install llama.cpp
**Please don't build llama-cpp locally.**  Yes you can run gguf model directly on terminal with this but to run it using Python we need to create subprocess which gives a lot of problems.

### Install Python Bindings with CUDA support:
If you want to use llama.cpp in Python, install the Python bindings:

```
CUDACXX=/usr/local/cuda-12.8/bin/nvcc CMAKE_ARGS="-DGGML_CUDA=on -DCMAKE_CUDA_ARCHITECTURES=86" pip install llama-cpp-python --no-cache-dir
```

_/usr/local/cuda-12.8/bin/nvcc_ -> path where nvcc is located which can be obtained using **which nvcc**

## Run

Run using

```
python Load_AI_Model_gguf.py
```

If python binding is properly installed with CUDA support then after running we get name of GPU 

![image](https://github.com/user-attachments/assets/1997667e-4f7a-45a5-b5d3-4279b51d4a57)

