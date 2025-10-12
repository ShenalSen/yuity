# Tourmate Vehicle Management System - Makefile
# Author: Student
# Date: October 12, 2025
# Purpose: Build configuration for C++ project

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -g
SRCDIR = src/source
HEADERDIR = src/headers
BUILDDIR = build
DATADIR = data
TARGET = $(BUILDDIR)/tourmate

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

# Header dependencies
HEADERS = $(wildcard $(HEADERDIR)/*.h)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(DATADIR)
	@mkdir -p $(DATADIR)/backup

# Link object files to create executable
$(TARGET): $(OBJECTS)
	@echo "Linking object files..."
	$(CXX) $(OBJECTS) -o $@
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -I$(HEADERDIR) -c $< -o $@

# Clean build directory
clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILDDIR)/*
	@echo "Clean complete."

# Full clean (including data files)
cleanall: clean
	@echo "Cleaning data files..."
	rm -f $(DATADIR)/*.csv
	rm -f $(DATADIR)/backup/*.csv
	@echo "Full clean complete."

# Install executable to system (optional)
install: $(TARGET)
	@echo "Installing tourmate..."
	cp $(TARGET) /usr/local/bin/
	@echo "Installation complete."

# Uninstall from system
uninstall:
	@echo "Uninstalling tourmate..."
	rm -f /usr/local/bin/tourmate
	@echo "Uninstall complete."

# Run the program
run: $(TARGET)
	@echo "Running Tourmate Vehicle Management System..."
	cd $(BUILDDIR) && ./tourmate

# Debug build
debug: CXXFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Release build
release: CXXFLAGS += -DRELEASE -O3 -DNDEBUG
release: clean $(TARGET)

# Create distribution package
dist: clean
	@echo "Creating distribution package..."
	tar -czf tourmate-$(shell date +%Y%m%d).tar.gz src/ data/ docs/ Makefile README.md
	@echo "Distribution package created."

# Initialize data files with headers
init-data:
	@echo "Initializing data files..."
	@echo "VehicleID,LicensePlate,Model,Year,Type,SeatingCapacity,Status,RegistrationDate,LastServiceDate" > $(DATADIR)/vehicles.csv
	@echo "SalesID,VehicleID,CustomerName,CustomerContact,StartLocation,EndLocation,TripDate,TripTime,Duration,FareAmount,PaymentStatus,SalesRep" > $(DATADIR)/sales.csv
	@echo "UserID,Username,Password,Role,LastLogin,AccountStatus" > $(DATADIR)/users.csv
	@echo "CompanyName,Address,Phone,Email,Website,EstablishedYear,TotalVehicles,SystemVersion" > $(DATADIR)/company.csv
	@echo "admin,admin123,Admin,2024-01-01 00:00:00,Active" >> $(DATADIR)/users.csv
	@echo "Data files initialized with headers and default admin user."

# Backup data files
backup:
	@echo "Creating data backup..."
	@mkdir -p $(DATADIR)/backup
	@cp $(DATADIR)/*.csv $(DATADIR)/backup/ 2>/dev/null || true
	@echo "Backup complete."

# Restore data files from backup
restore:
	@echo "Restoring data from backup..."
	@cp $(DATADIR)/backup/*.csv $(DATADIR)/ 2>/dev/null || true
	@echo "Restore complete."

# Check code style (requires cppcheck)
check:
	@echo "Running code style check..."
	@which cppcheck > /dev/null && cppcheck --enable=all --std=c++11 $(SRCDIR)/ || echo "cppcheck not found, skipping style check"

# Format code (requires clang-format)
format:
	@echo "Formatting code..."
	@which clang-format > /dev/null && find $(SRCDIR) $(HEADERDIR) -name "*.cpp" -o -name "*.h" | xargs clang-format -i || echo "clang-format not found, skipping formatting"

# Show help
help:
	@echo "Tourmate Vehicle Management System - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all         - Build the complete project (default)"
	@echo "  clean       - Remove build files"
	@echo "  cleanall    - Remove build and data files"
	@echo "  run         - Build and run the program"
	@echo "  debug       - Build with debug symbols"
	@echo "  release     - Build optimized release version"
	@echo "  init-data   - Initialize CSV data files with headers"
	@echo "  backup      - Backup current data files"
	@echo "  restore     - Restore data files from backup"
	@echo "  dist        - Create distribution package"
	@echo "  install     - Install to system (Linux/Mac)"
	@echo "  uninstall   - Remove from system"
	@echo "  check       - Run code style check (requires cppcheck)"
	@echo "  format      - Format code (requires clang-format)"
	@echo "  help        - Show this help message"
	@echo ""
	@echo "Example usage:"
	@echo "  make              # Build the project"
	@echo "  make init-data    # Setup data files"
	@echo "  make run          # Build and run"
	@echo "  make clean        # Clean build files"

# Declare phony targets
.PHONY: all clean cleanall install uninstall run debug release dist init-data backup restore check format help directories

# Show build information
info:
	@echo "=== Build Information ==="
	@echo "Compiler: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "Source Directory: $(SRCDIR)"
	@echo "Header Directory: $(HEADERDIR)"
	@echo "Build Directory: $(BUILDDIR)"
	@echo "Target: $(TARGET)"
	@echo "Sources: $(SOURCES)"
	@echo "========================="