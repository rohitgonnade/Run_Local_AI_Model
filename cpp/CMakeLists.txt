cmake_minimum_required(VERSION 3.16)
project(ChatLlamaApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include llama headers
include_directories(/Rohit/installed_Llamma_cpp/include
  )

link_directories(/Rohit/installed_Llamma_cpp/lib64)
file(GLOB LLAMALIBS "/Rohit/installed_Llamma_cpp/lib64/*.so")
# Define your main program
add_executable(chat_llama main.cpp)
target_link_libraries(chat_llama ${LLAMALIBS})

