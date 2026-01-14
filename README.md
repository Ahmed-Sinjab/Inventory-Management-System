# Inventory Management System - CS3307A Final Project

**Authors:** Omar Hossain, Ahmed Sinjab  
**Course:** CS3307A - Object-Oriented Design and Analysis  
**Instructor:** Dr. Umair Rehman  
**Term:** Fall 2025

---

## Project Overview

A comprehensive inventory management system built with C++ and Qt 6, demonstrating object-oriented design principles, design patterns, and professional software development practices. The system manages products, suppliers, and users with full CRUD operations, data persistence, and authenticated access control.

### Key Features

- **Product Management**: Add, remove, view, and search products across 5 types (RawMaterial, NonPerishable, Perishable, Food, Chemical)
- **Supplier Tracking**: Manage suppliers with quality ratings, delivery reliability, and product associations
- **Inventory Operations**: Buy/sell products with automatic stock validation and low-stock alerts
- **User Authentication**: Secure login system with User and Admin roles
- **Data Persistence**: Save/load inventory state using JSON format
- **Factory Pattern**: Centralized product creation with automatic ID prefixing
- **Comprehensive Testing**: 58 unit tests with 100% pass rate using GoogleTest and Google Mock

---

## System Architecture

### Three-Layer Architecture

```
┌─────────────────────────────────────┐
│     Presentation Layer (Qt UI)      │
│  - LoginDialog                      │
│  - Dialog (Main Window)             │
└─────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────┐
│   Business Logic Layer (Core)       │
│  - Product Hierarchy                │
│  - Inventory (Facade)               │
│  - ProductFactory                   │
│  - Supplier                         │
│  - User/Admin                       │
└─────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────┐
│   Data Persistence Layer            │
│  - FileManager (JSON I/O)           │
│  - In-memory STL containers         │
└─────────────────────────────────────┘
```

### Product Hierarchy

```
Product (interface)
├── RawMaterial
├── NonPerishable
└── Perishable
    ├── Food
    └── Chemical
```

---

## Getting Started

### Prerequisites

**Required:**

- Qt 6.9.3 or higher
- CMake 3.16 or higher
- C++17 compatible compiler

**Recommended:**

- Qt Creator IDE
- Git

### Installation Steps

#### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/oop-assignment-2-omar-and-ahmed.git
cd oop-assignment-2-omar-and-ahmed
```

#### 2. Open in Qt Creator

1. Launch Qt Creator
2. **File → Open File or Project**
3. Navigate to project folder and select `CMakeLists.txt`
4. Select your Qt kit (Desktop Qt 6.9.3 MinGW 64-bit)
5. Click **Configure Project**

#### 3. Build the Project

**In Qt Creator**

- Click the **Build** button (hammer icon) or press `Ctrl+B`
- Wait for compilation to complete

#### 4. Run the Application

**Option A: In Qt Creator**

- Select `assignment2` from the run target dropdown (bottom left)
- Click the **Run** button (green play icon) or press `Ctrl+R`

**Option B: Command Line**

```bash
cd build/Desktop_Qt_6_9_3_MinGW_64_bit-Debug
./assignment2.exe  # Windows
./assignment2      # Linux/macOS
```

---

## Running Tests

### Build Tests

**In Qt Creator:**

1. **Build → Build All Projects** (`Ctrl+Shift+B`)
2. This builds both `assignment2` and `inventory_tests`

### Run Tests

**In Qt Creator:**

1. Select `inventory_tests` from the run target dropdown
2. Click **Run** (`Ctrl+R`)

---

## Known Issues & Limitations

### Current Limitations

1. **Single User Access**: No concurrent multi-user support
2. **In-Memory Storage**: Data only persists when explicitly saved
3. **No Undo/Redo**: Operations cannot be reversed
4. **Limited Validation**: Some edge cases may not be fully validated
5. **No Analytics**: Historical data tracking not implemented
6. **Basic UI**: Minimal styling and visual polish

### Future Enhancements

- [ ] Database integration (SQLite or PostgreSQL)
- [ ] Multi-user concurrent access with locking
- [ ] Advanced analytics and reporting dashboards
- [ ] Historical data tracking and audit logs
- [ ] Automated reorder system based on usage patterns
- [ ] Export to Excel/CSV
- [ ] Email notifications for low stock
- [ ] Barcode scanner integration
- [ ] Mobile app version

---

### Link to Loom Video

https://youtu.be/veLnmdeNsGk
