# Traffic Lights Simulator System

A multi-threaded traffic light simulation system implemented in C that models multiple intersections with realistic traffic patterns and timing.

## Features

- Simultaneous control of multiple traffic light intersections
- Real-time traffic density simulation
- Thread-safe operations using mutex locks
- Configurable timing for different light states
- Dynamic traffic density updates
- Clear status display for each intersection

## Technical Specifications

### System Requirements

- GCC compiler (version 4.8.5 or higher)
- POSIX-compliant operating system (Linux, macOS, etc.)
- pthread library
- Minimum 512MB RAM
- Terminal with support for standard output

### Key Components

1. **Intersection Structure**
   - North-South and East-West signal states
   - Traffic density monitoring
   - Thread synchronization mechanisms
   - Unique intersection ID
   - Active status tracking

2. **Light States**
   - RED (30 seconds)
   - YELLOW (5 seconds)
   - GREEN (25 seconds)

3. **Traffic Patterns**
   - Dynamic traffic density simulation (0-10 scale)
   - Updates every 5 cycles
   - Independent density tracking for each direction

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/stephenombuya/Traffic-Lights-Simulator
   cd traffic-lights-simulator
   ```

2. Compile the program:
   ```bash
   gcc -o traffic_sim main.c -pthread
   ```

3. Run the simulator:
   ```bash
   ./traffic_sim
   ```

## Usage

The simulator automatically initializes with 3 intersections and runs for 2 minutes. Each intersection operates independently with its own thread.

### Output Format

```
Intersection 0 Status:
North-South: RED (Traffic Density: 7/10)
East-West: GREEN (Traffic Density: 4/10)
----------------------------------------
```

### Configuration

You can modify the following constants in the code:
- `MAX_INTERSECTIONS`: Maximum number of supported intersections
- `RED_DURATION`: Duration of red light in seconds
- `YELLOW_DURATION`: Duration of yellow light in seconds
- `GREEN_DURATION`: Duration of green light in seconds

## Code Structure

```
traffic_lights_simulator/
├── main.c                 # Main source code
├── README.md             # Documentation
└── .gitignore           # Git ignore file
```

### Key Functions

- `initializeIntersection()`: Sets up a new intersection
- `controlIntersection()`: Main control loop for each intersection
- `updateTrafficDensity()`: Simulates changing traffic patterns
- `displayIntersectionStatus()`: Outputs current intersection state
- `cleanupIntersection()`: Properly terminates intersection operations

## Threading Model

- Each intersection runs in its own thread
- Mutex locks prevent race conditions
- Thread-safe status updates and display
- Clean shutdown mechanism

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.


## Acknowledgments

- Inspired by real-world traffic control systems
- Built with modern C programming practices
- Designed for educational purposes

## Support

For support, please open an issue in the GitHub repository.
