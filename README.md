### ringbuffer

Implementation of a simple ring buffer for a single producer and multiple consumers.

Each thread has its own index. Calling `ringbuffer_insert` (write) and `ringbuffer_peek` (read)
increments the caller's index. Thread safety is guaranteed by the fact that the producer can
never overtake the slowest consumer, and none of the producers are not allowed to overtake the producer.

Written while learning C and based on LMAX Disruptor (https://lmax-exchange.github.io/disruptor/).

#### playing around with the code

Running the tests can be done by running the build script 

``./run_ringbuffer_tests.sh``

The executable will be put in a `build/` folder under the main directory of the project.

Note that you will need [Catch](https://github.com/catchorg/Catch2) (more specifically the `catch.hpp` file) and 
put it under `test/Catch`.
