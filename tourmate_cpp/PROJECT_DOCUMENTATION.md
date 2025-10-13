# 🚗 Tourmate Vehicle Management System

## 📋 Project Overview

**Tourmate** is a comprehensive, enterprise-grade vehicle management system developed in C++ for transport businesses. This academic project (CSE4002 WRIT1) provides a complete solution for fleet management, sales processing, customer relationship management, and business analytics.

### 🎯 Project Objectives

- **Fleet Management**: Complete vehicle lifecycle management with maintenance tracking
- **Sales Processing**: Advanced booking system with payment processing and customer management
- **Business Intelligence**: Real-time analytics and comprehensive reporting
- **Security**: Role-based access control with audit logging
- **Professional Interface**: Intuitive console-based user experience

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                     Main Application                            │
│                   (main_tourmate.cpp)                          │
└─────────────────────┬───────────────────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────────────────┐
│                  MainMenuSystem                                 │
│               (Unified Interface)                               │
└─────┬─────────────┬─────────────┬─────────────┬─────────────────┘
      │             │             │             │
┌─────▼─────┐ ┌─────▼─────┐ ┌─────▼─────┐ ┌─────▼─────┐
│   Auth    │ │ Vehicle   │ │   Sales   │ │ Utilities │
│ Manager   │ │ Manager   │ │ Manager   │ │ & Helpers │
└───────────┘ └───────────┘ └───────────┘ └───────────┘
```

## 🚀 Key Features

### 🔐 Authentication & Security
- **Multi-role Access Control**: Admin, Operator, Viewer roles
- **Secure Password Encryption**: SHA-256 with salt
- **Session Management**: Automatic timeout and security logging
- **Audit Trail**: Comprehensive operation logging

### 🚙 Vehicle Management
- **Complete CRUD Operations**: Add, view, update, delete vehicles
- **Fleet Tracking**: Real-time status monitoring and availability
- **Maintenance Management**: Service scheduling and history tracking
- **Advanced Search**: Multi-criteria vehicle filtering
- **Status Management**: Available, In Service, Maintenance, Retired

### 💰 Sales & Booking System
- **Customer Management**: Complete CRM with VIP status tracking
- **Booking Workflow**: Pending → Confirmed → In Progress → Completed
- **Payment Processing**: 5 payment methods (Cash, Credit Card, Debit Card, Digital Wallet, Bank Transfer)
- **Trip Types**: One Way, Round Trip, Multi Stop, Charter
- **Revenue Analytics**: Daily, weekly, monthly reporting

### 📊 Business Intelligence
- **Real-time Dashboard**: System overview and quick statistics
- **Revenue Reports**: Comprehensive financial analytics
- **Customer Analytics**: Behavior patterns and VIP identification
- **Fleet Utilization**: Vehicle performance metrics
- **Export Capabilities**: Data backup and reporting

### 🛡️ Data Management
- **CSV File Storage**: Structured data persistence
- **Backup & Recovery**: Automated data protection
- **Data Integrity**: Validation and consistency checks
- **Import/Export**: Flexible data management

## 💻 Technical Specifications

### Requirements
- **Compiler**: GCC 4.8+ with C++11 support
- **Operating System**: Windows 7/8/10/11 or Linux
- **Memory**: 512MB RAM minimum
- **Storage**: 100MB free space
- **Display**: Console/Terminal support

### Technologies Used
- **Language**: C++11 with modern features
- **Architecture**: Object-oriented design with SOLID principles
- **Data Storage**: CSV file format with structured schema
- **Error Handling**: Exception-based with comprehensive logging
- **Memory Management**: Smart pointers and RAII principles

## 🚀 Getting Started

### 1. Build the System
```bash
# Windows
build.bat

# Linux/Mac (create equivalent shell script)
g++ -std=c++11 -I"src/headers" src/source/*.cpp main_tourmate.cpp -o tourmate
```

### 2. Run the Application
```bash
# Windows - Full setup with sample data
run_tourmate.bat

# Direct execution
build/tourmate.exe
```

### 3. Default Login Credentials
- **Admin**: `admin` / `Admin123` (Full system access)
- **Operator**: `operator` / `Oper123` (Operations management)
- **Viewer**: `viewer` / `View123` (Read-only access)

## 🎮 User Guide

### Main Navigation
1. **Login** with provided credentials
2. **Main Dashboard** displays system overview
3. **Module Selection** based on user role
4. **Operations** within each module
5. **Logout** or system exit

### Vehicle Management
- **Add Vehicles**: Register new fleet members
- **View Fleet**: Monitor all vehicles and status
- **Search & Filter**: Find specific vehicles
- **Maintenance**: Track service schedules
- **Reports**: Fleet utilization analytics

### Sales Management
- **Customer Management**: Add and manage customer profiles
- **Create Bookings**: New customer reservations
- **Manage Bookings**: Track booking lifecycle
- **Payment Processing**: Handle transactions
- **Reports**: Revenue and customer analytics

## 🧪 Testing

### Available Test Suites
- **Core Test**: Basic data structures and utilities
- **Authentication Test**: Login, roles, and security
- **Vehicle Test**: Fleet management operations
- **Sales Test**: Booking and customer management

### Running Tests
```bash
# Individual test suites
build/tourmate_core_test.exe
build/tourmate_auth_test.exe
build/tourmate_vehicle_test.exe
build/tourmate_sales_test.exe

# Generate sample data for testing
build/generate_sample_data.exe
```

## 🎓 Academic Project Details

### Course Information
- **Course**: CSE4002 WRIT1
- **Project Type**: Academic Coursework
- **Language**: C++11
- **Paradigm**: Object-Oriented Programming

### Learning Objectives
- **Software Architecture**: Modular design principles
- **C++ Programming**: Modern C++11 features and best practices
- **Data Management**: File I/O and data persistence
- **User Interface**: Console-based application development
- **Project Management**: Complete software development lifecycle

---

**🚗 Tourmate - Transforming Transport Management**

*Your trusted partner in vehicle fleet management and business growth.*