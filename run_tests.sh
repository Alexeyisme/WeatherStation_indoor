#!/bin/bash

# WeatherStation Indoor Test Runner
# This script provides easy testing commands for the project

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if PlatformIO is installed
check_platformio() {
    if ! command -v pio &> /dev/null; then
        print_error "PlatformIO is not installed or not in PATH"
        print_status "Please install PlatformIO: https://platformio.org/install"
        exit 1
    fi
    print_success "PlatformIO found: $(pio --version)"
}

# Function to install dependencies
install_dependencies() {
    print_status "Installing project dependencies..."
    pio lib install
    print_success "Dependencies installed successfully"
}

# Function to run all tests
run_all_tests() {
    print_status "Running all tests..."
    pio test -e test -v
    if [ $? -eq 0 ]; then
        print_success "All tests passed!"
    else
        print_error "Some tests failed!"
        exit 1
    fi
}

# Function to run specific test
run_specific_test() {
    local test_name=$1
    if [ -z "$test_name" ]; then
        print_error "Please specify a test name"
        print_status "Available tests:"
        echo "  - test_sensor_manager"
        echo "  - test_ble_manager"
        echo "  - test_wifi_manager"
        echo "  - test_display_manager"
        echo "  - test_time_manager"
        echo "  - test_integration"
        echo "  - test_api"
        exit 1
    fi
    
    print_status "Running test: $test_name"
    pio test -e test --filter "$test_name" -v
    if [ $? -eq 0 ]; then
        print_success "Test $test_name passed!"
    else
        print_error "Test $test_name failed!"
        exit 1
    fi
}

# Function to run tests with coverage
run_tests_with_coverage() {
    print_status "Running tests with coverage analysis..."
    pio test -e test --coverage -v
    if [ $? -eq 0 ]; then
        print_success "Coverage analysis completed!"
    else
        print_error "Coverage analysis failed!"
        exit 1
    fi
}

# Function to run tests on hardware
run_hardware_tests() {
    local port=$1
    if [ -z "$port" ]; then
        print_error "Please specify a serial port"
        print_status "Example: ./run_tests.sh hardware /dev/ttyUSB0"
        exit 1
    fi
    
    print_status "Running tests on hardware at $port..."
    pio test -e test --upload-port "$port" --monitor-port "$port" -v
    if [ $? -eq 0 ]; then
        print_success "Hardware tests completed!"
    else
        print_error "Hardware tests failed!"
        exit 1
    fi
}

# Function to clean test environment
clean_tests() {
    print_status "Cleaning test environment..."
    pio run -e test -t clean
    print_success "Test environment cleaned"
}

# Function to show test help
show_help() {
    echo "WeatherStation Indoor Test Runner"
    echo "================================="
    echo ""
    echo "Usage: $0 [COMMAND] [OPTIONS]"
    echo ""
    echo "Commands:"
    echo "  all                    Run all tests"
    echo "  specific <test_name>   Run specific test"
    echo "  coverage               Run tests with coverage analysis"
    echo "  hardware <port>        Run tests on hardware device"
    echo "  install                Install test dependencies"
    echo "  clean                  Clean test environment"
    echo "  help                   Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 all                           # Run all tests"
    echo "  $0 specific test_sensor_manager  # Run sensor manager tests"
    echo "  $0 coverage                      # Run tests with coverage"
    echo "  $0 hardware /dev/ttyUSB0         # Run tests on hardware"
    echo "  $0 install                       # Install dependencies"
    echo "  $0 clean                         # Clean test environment"
    echo ""
    echo "Available Tests:"
    echo "  - test_sensor_manager    SensorManager unit tests"
    echo "  - test_ble_manager       BLEManager unit tests"
    echo "  - test_wifi_manager      WiFiManager unit tests"
    echo "  - test_display_manager   DisplayManager unit tests"
    echo "  - test_time_manager      TimeManager unit tests"
    echo "  - test_integration       Integration tests"
    echo "  - test_api               API and JSON tests"
}

# Main script logic
main() {
    # Check if PlatformIO is available
    check_platformio
    
    # Parse command line arguments
    case "${1:-help}" in
        "all")
            install_dependencies
            run_all_tests
            ;;
        "specific")
            install_dependencies
            run_specific_test "$2"
            ;;
        "coverage")
            install_dependencies
            run_tests_with_coverage
            ;;
        "hardware")
            install_dependencies
            run_hardware_tests "$2"
            ;;
        "install")
            install_dependencies
            ;;
        "clean")
            clean_tests
            ;;
        "help"|"-h"|"--help")
            show_help
            ;;
        *)
            print_error "Unknown command: $1"
            show_help
            exit 1
            ;;
    esac
}

# Run main function with all arguments
main "$@"
