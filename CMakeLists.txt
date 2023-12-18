cmake_minimum_required(VERSION 3.26)
project(idz4)

set(CMAKE_CXX_STANDARD 17)

add_executable(idz4 main.cpp
        gardener/Gardener.h
        field/Field.cpp
        field/Field.h
        field/FieldIterable.cpp
        field/FieldIterable.h
        task_v1/Task.cpp
        task_v1/Task.h
        io/Output.cpp
        io/Output.h
        task_v2/TrafficGuard/GardenerQueue.cpp
        task_v2/TrafficGuard/GardenerQueue.h
        task_v2/TrafficGuard/TrafficGuard.cpp
        task_v2/TrafficGuard/TrafficGuard.h
        task_v2/Task.cpp
        task_v2/Task.h
        task_v1/solution.h
        task_v1/solution.cpp
        task_v2/solution.h
        task_v2/solution.cpp
        io/input.h
        io/input.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(idz4 Threads::Threads)
